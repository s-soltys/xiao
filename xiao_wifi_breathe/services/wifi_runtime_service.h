#pragma once

String wifiProfileSsidPreferenceKey(size_t index) {
  return String(kWifiSsidPreferenceKeyPrefix) + String(index);
}

String wifiProfilePasswordPreferenceKey(size_t index) {
  return String(kWifiPasswordPreferenceKeyPrefix) + String(index);
}

String normalizeWifiSsid(const String &rawValue) {
  String normalized = rawValue;
  normalized.trim();
  return normalized;
}

int findSavedWifiNetworkIndex(const String &ssid) {
  for (size_t index = 0; index < savedWifiNetworkCount; ++index) {
    if (savedWifiNetworks[index].ssid == ssid) {
      return static_cast<int>(index);
    }
  }

  return -1;
}

void syncWifiTargetProfileIndex() {
  if (wifiCurrentTargetIsFallback || wifiCurrentTargetSsid.isEmpty()) {
    wifiCurrentTargetProfileIndex = -1;
    return;
  }

  wifiCurrentTargetProfileIndex = findSavedWifiNetworkIndex(wifiCurrentTargetSsid);
}

void refreshWifiScanKnownFlags() {
  for (size_t index = 0; index < wifiScanResultCount; ++index) {
    wifiScanResults[index].known = findSavedWifiNetworkIndex(wifiScanResults[index].ssid) >= 0;
  }
}

void resetWifiAttemptCursor() {
  wifiAttemptCursor = 0;
}

int wifiCandidateCount() {
  return static_cast<int>(savedWifiNetworkCount) + (hasFallbackWifiCredentials() ? 1 : 0);
}

String currentWifiLiveSsid() {
  return WiFi.status() == WL_CONNECTED ? WiFi.SSID() : String("");
}

String currentWifiSource() {
  if (WiFi.status() == WL_CONNECTED || wifiConnectInProgress || wifiImmediateConnectPending) {
    if (wifiCurrentTargetProfileIndex >= 0 && !wifiCurrentTargetIsFallback) {
      return F("saved");
    }

    if (wifiCurrentTargetIsFallback) {
      return F("fallback");
    }
  }

  return F("none");
}

String currentWifiStatusLabel() {
  if (WiFi.status() == WL_CONNECTED) {
    return F("connected");
  }

  if (wifiConnectInProgress || wifiImmediateConnectPending) {
    return F("connecting");
  }

  if (wifiScanInProgress) {
    return F("scanning");
  }

  if (!wifiLastError.isEmpty()) {
    return F("error");
  }

  return F("disconnected");
}

String wifiEncryptionTypeLabel(wifi_auth_mode_t encryptionType) {
  switch (encryptionType) {
    case WIFI_AUTH_OPEN:
      return F("open");
    case WIFI_AUTH_WEP:
      return F("wep");
    case WIFI_AUTH_WPA_PSK:
      return F("wpa-psk");
    case WIFI_AUTH_WPA2_PSK:
      return F("wpa2-psk");
    case WIFI_AUTH_WPA_WPA2_PSK:
      return F("wpa-wpa2-psk");
#ifdef WIFI_AUTH_WPA2_ENTERPRISE
    case WIFI_AUTH_WPA2_ENTERPRISE:
      return F("wpa2-enterprise");
#endif
#ifdef WIFI_AUTH_WPA3_PSK
    case WIFI_AUTH_WPA3_PSK:
      return F("wpa3-psk");
#endif
#ifdef WIFI_AUTH_WPA2_WPA3_PSK
    case WIFI_AUTH_WPA2_WPA3_PSK:
      return F("wpa2-wpa3-psk");
#endif
#ifdef WIFI_AUTH_WAPI_PSK
    case WIFI_AUTH_WAPI_PSK:
      return F("wapi-psk");
#endif
    default:
      return F("unknown");
  }
}

void persistSavedWifiNetworks() {
  preferences.putUChar(kWifiProfileCountPreferenceKey, static_cast<uint8_t>(savedWifiNetworkCount));

  for (size_t index = 0; index < kMaxSavedWifiNetworks; ++index) {
    const String ssidKey = wifiProfileSsidPreferenceKey(index);
    const String passwordKey = wifiProfilePasswordPreferenceKey(index);

    if (index < savedWifiNetworkCount) {
      preferences.putString(ssidKey.c_str(), savedWifiNetworks[index].ssid);
      preferences.putString(passwordKey.c_str(), savedWifiNetworks[index].password);
    } else {
      preferences.remove(ssidKey.c_str());
      preferences.remove(passwordKey.c_str());
    }
  }
}

void loadSavedWifiNetworks() {
  savedWifiNetworkCount = 0;

  const size_t storedCount = preferences.getUChar(kWifiProfileCountPreferenceKey, 0);
  for (size_t index = 0; index < storedCount && index < kMaxSavedWifiNetworks; ++index) {
    const String ssid = normalizeWifiSsid(preferences.getString(wifiProfileSsidPreferenceKey(index).c_str(), ""));
    if (ssid.isEmpty() || findSavedWifiNetworkIndex(ssid) >= 0) {
      continue;
    }

    savedWifiNetworks[savedWifiNetworkCount].ssid = ssid;
    savedWifiNetworks[savedWifiNetworkCount].password = preferences.getString(wifiProfilePasswordPreferenceKey(index).c_str(), "");
    ++savedWifiNetworkCount;
  }

  persistSavedWifiNetworks();
  syncWifiTargetProfileIndex();
}

void insertOrUpdateSavedWifiNetwork(const String &ssid, const String &password) {
  const String normalizedSsid = normalizeWifiSsid(ssid);
  if (normalizedSsid.isEmpty()) {
    return;
  }

  SavedWifiNetwork network = {normalizedSsid, password};
  const int existingIndex = findSavedWifiNetworkIndex(normalizedSsid);

  if (existingIndex >= 0) {
    for (int index = existingIndex; index > 0; --index) {
      savedWifiNetworks[index] = savedWifiNetworks[index - 1];
    }
    savedWifiNetworks[0] = network;
  } else {
    const size_t nextCount = savedWifiNetworkCount < kMaxSavedWifiNetworks ? savedWifiNetworkCount + 1 : savedWifiNetworkCount;
    for (size_t index = nextCount; index > 1; --index) {
      savedWifiNetworks[index - 1] = savedWifiNetworks[index - 2];
    }
    savedWifiNetworks[0] = network;
    savedWifiNetworkCount = nextCount;
  }

  persistSavedWifiNetworks();
  refreshWifiScanKnownFlags();
  syncWifiTargetProfileIndex();
  resetWifiAttemptCursor();
}

bool forgetSavedWifiNetwork(const String &ssid) {
  const int existingIndex = findSavedWifiNetworkIndex(ssid);
  if (existingIndex < 0) {
    return false;
  }

  for (size_t index = static_cast<size_t>(existingIndex); index + 1 < savedWifiNetworkCount; ++index) {
    savedWifiNetworks[index] = savedWifiNetworks[index + 1];
  }

  if (savedWifiNetworkCount > 0) {
    --savedWifiNetworkCount;
    savedWifiNetworks[savedWifiNetworkCount].ssid = "";
    savedWifiNetworks[savedWifiNetworkCount].password = "";
  }

  persistSavedWifiNetworks();
  refreshWifiScanKnownFlags();
  syncWifiTargetProfileIndex();
  resetWifiAttemptCursor();
  return true;
}

bool reorderSavedWifiNetworks(const String *orderedSsids, size_t orderedCount) {
  if (orderedCount != savedWifiNetworkCount) {
    return false;
  }

  SavedWifiNetwork reordered[kMaxSavedWifiNetworks];
  bool used[kMaxSavedWifiNetworks] = {false};

  for (size_t orderedIndex = 0; orderedIndex < orderedCount; ++orderedIndex) {
    const int currentIndex = findSavedWifiNetworkIndex(orderedSsids[orderedIndex]);
    if (currentIndex < 0 || used[currentIndex]) {
      return false;
    }

    reordered[orderedIndex] = savedWifiNetworks[currentIndex];
    used[currentIndex] = true;
  }

  for (size_t index = 0; index < orderedCount; ++index) {
    savedWifiNetworks[index] = reordered[index];
  }

  persistSavedWifiNetworks();
  refreshWifiScanKnownFlags();
  syncWifiTargetProfileIndex();
  resetWifiAttemptCursor();
  return true;
}

void syncWifiTargetFromCurrentConnection() {
  const String connectedSsid = currentWifiLiveSsid();
  if (connectedSsid.isEmpty()) {
    return;
  }

  if (connectedSsid == wifiCurrentTargetSsid) {
    if (!wifiCurrentTargetIsFallback) {
      syncWifiTargetProfileIndex();
    }
    return;
  }

  wifiCurrentTargetSsid = connectedSsid;
  wifiCurrentTargetProfileIndex = findSavedWifiNetworkIndex(connectedSsid);
  wifiCurrentTargetIsFallback = wifiCurrentTargetProfileIndex < 0 && hasFallbackWifiCredentials() && connectedSsid == String(kWifiSsid);
}

void enableBoardWifiHardware() {
#ifdef WIFI_ENABLE
  pinMode(WIFI_ENABLE, OUTPUT);
  digitalWrite(WIFI_ENABLE, LOW);
#endif

#ifdef WIFI_ANT_CONFIG
  pinMode(WIFI_ANT_CONFIG, OUTPUT);
  digitalWrite(WIFI_ANT_CONFIG, LOW);
#endif
}

void ensureWifiStaMode() {
  if (WiFi.getMode() != WIFI_STA) {
    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
  }
}

void startHttpServerIfNeeded() {
  if (webServerStarted || WiFi.status() != WL_CONNECTED) {
    return;
  }

  webServer.begin();
  webServerStarted = true;

  Serial.print(F("HTTP server ready at http://"));
  Serial.print(kMdnsAddress);
  Serial.println('/');
}

void stopMdnsIfNeeded() {
  if (!mdnsStarted) {
    return;
  }

  MDNS.end();
  mdnsStarted = false;
}

void startMdnsIfNeeded() {
  if (mdnsStarted || WiFi.status() != WL_CONNECTED) {
    return;
  }

  if (!MDNS.begin(kHostname)) {
    Serial.println(F("mDNS start failed."));
    return;
  }

  MDNS.setInstanceName(F("XIAO Pattern Console"));
  MDNS.addService("http", "tcp", kHttpPort);
  mdnsStarted = true;

  Serial.print(F("mDNS active at http://"));
  Serial.print(kMdnsAddress);
  Serial.println('/');
}

void logWifiStatus(wl_status_t status) {
  switch (status) {
    case WL_CONNECTED:
      wifiLastError = "";
      wifiConnectInProgress = false;
      syncWifiTargetFromCurrentConnection();
      Serial.print(F("Wi-Fi connected. IP: "));
      Serial.println(WiFi.localIP());
      break;
    case WL_NO_SSID_AVAIL:
      wifiConnectInProgress = false;
      wifiLastError = wifiCurrentTargetSsid.isEmpty() ? String(F("SSID not found.")) : String(F("SSID not found: ")) + wifiCurrentTargetSsid;
      Serial.println(F("Wi-Fi error: SSID not found."));
      break;
    case WL_CONNECT_FAILED:
      wifiConnectInProgress = false;
      wifiLastError = wifiCurrentTargetSsid.isEmpty() ? String(F("Authentication failed.")) : String(F("Authentication failed for ")) + wifiCurrentTargetSsid;
      Serial.println(F("Wi-Fi error: authentication failed."));
      break;
    case WL_CONNECTION_LOST:
      wifiConnectInProgress = false;
      wifiLastError = wifiCurrentTargetSsid.isEmpty() ? String(F("Connection lost.")) : String(F("Connection lost: ")) + wifiCurrentTargetSsid;
      Serial.println(F("Wi-Fi status: connection lost."));
      break;
    case WL_DISCONNECTED:
      Serial.println(F("Wi-Fi status: disconnected."));
      break;
    default:
      break;
  }
}

bool startWifiConnectionTo(const String &ssid, const String &password, int profileIndex, bool useFallback) {
  if (ssid.isEmpty()) {
    return false;
  }

  ensureWifiStaMode();
  WiFi.disconnect(false, false);

  Serial.print(F("Connecting to Wi-Fi SSID: "));
  Serial.println(ssid);

  if (password.isEmpty()) {
    WiFi.begin(ssid.c_str());
  } else {
    WiFi.begin(ssid.c_str(), password.c_str());
  }

  wifiCurrentTargetSsid = ssid;
  wifiCurrentTargetProfileIndex = profileIndex;
  wifiCurrentTargetIsFallback = useFallback;
  wifiConnectInProgress = true;
  wifiConnectStartedAtMs = millis();
  lastWifiAttemptMs = wifiConnectStartedAtMs;
  wifiLastError = "";
  wifiWarningPrinted = false;
  return true;
}

bool beginSavedWifiConnectionByIndex(int index) {
  if (index < 0 || index >= static_cast<int>(savedWifiNetworkCount)) {
    return false;
  }

  return startWifiConnectionTo(savedWifiNetworks[index].ssid, savedWifiNetworks[index].password, index, false);
}

bool beginFallbackWifiConnection() {
  if (!hasFallbackWifiCredentials()) {
    return false;
  }

  return startWifiConnectionTo(String(kWifiSsid), String(kWifiPassword), -1, true);
}

bool beginNextWifiConnectionAttempt() {
  const int candidateTotal = wifiCandidateCount();
  if (candidateTotal <= 0) {
    if (!wifiWarningPrinted) {
      Serial.println(F("No Wi-Fi networks configured. Add one in the Wi-Fi app or wifi_config.h."));
      wifiWarningPrinted = true;
    }
    return false;
  }

  if (wifiAttemptCursor < 0 || wifiAttemptCursor >= candidateTotal) {
    wifiAttemptCursor = 0;
  }

  const int candidateIndex = wifiAttemptCursor;
  wifiAttemptCursor = (wifiAttemptCursor + 1) % candidateTotal;

  if (candidateIndex < static_cast<int>(savedWifiNetworkCount)) {
    return beginSavedWifiConnectionByIndex(candidateIndex);
  }

  return beginFallbackWifiConnection();
}

void queueImmediateWifiConnection(int profileIndex) {
  if (profileIndex < 0 || profileIndex >= static_cast<int>(savedWifiNetworkCount)) {
    return;
  }

  wifiImmediateConnectProfileIndex = profileIndex;
  wifiImmediateConnectPending = true;
  wifiCurrentTargetSsid = savedWifiNetworks[profileIndex].ssid;
  wifiCurrentTargetProfileIndex = profileIndex;
  wifiCurrentTargetIsFallback = false;
  wifiConnectStartedAtMs = millis();
  wifiLastError = "";
}

void processPendingWifiConnect() {
  if (!wifiImmediateConnectPending) {
    return;
  }

  wifiImmediateConnectPending = false;
  if (!beginSavedWifiConnectionByIndex(wifiImmediateConnectProfileIndex)) {
    wifiLastError = F("Unable to start the requested Wi-Fi connection.");
    return;
  }

  const int candidateTotal = wifiCandidateCount();
  wifiAttemptCursor = candidateTotal > 0 ? (wifiImmediateConnectProfileIndex + 1) % candidateTotal : 0;
}

void clearWifiScanResults() {
  wifiScanResultCount = 0;
  for (size_t index = 0; index < kMaxWifiScanResults; ++index) {
    wifiScanResults[index].ssid = "";
    wifiScanResults[index].rssi = 0;
    wifiScanResults[index].encryptionType = WIFI_AUTH_OPEN;
    wifiScanResults[index].known = false;
  }
}

bool startWifiScan() {
  ensureWifiStaMode();

  const int scanStatus = WiFi.scanComplete();
  if (scanStatus == WIFI_SCAN_RUNNING) {
    wifiScanInProgress = true;
    return true;
  }

  if (scanStatus >= 0) {
    WiFi.scanDelete();
  }

  clearWifiScanResults();
  const int startResult = WiFi.scanNetworks(true, true);
  if (startResult == WIFI_SCAN_FAILED) {
    wifiScanInProgress = false;
    wifiLastError = F("Unable to start a Wi-Fi scan.");
    return false;
  }

  wifiScanInProgress = true;
  wifiScanStartedAtMs = millis();
  wifiLastError = "";
  return true;
}

void captureWifiScanResults() {
  const int scanStatus = WiFi.scanComplete();
  if (scanStatus == WIFI_SCAN_RUNNING) {
    wifiScanInProgress = true;
    return;
  }

  if (!wifiScanInProgress && scanStatus < 0) {
    return;
  }

  if (scanStatus == WIFI_SCAN_FAILED) {
    wifiScanInProgress = false;
    wifiScanCompletedAtMs = millis();
    wifiLastError = F("Wi-Fi scan failed.");
    WiFi.scanDelete();
    return;
  }

  clearWifiScanResults();
  if (scanStatus > 0) {
    for (int networkIndex = 0; networkIndex < scanStatus; ++networkIndex) {
      const String ssid = normalizeWifiSsid(WiFi.SSID(networkIndex));
      if (ssid.isEmpty()) {
        continue;
      }

      const int existingIndex = findSavedWifiNetworkIndex(ssid);
      int targetIndex = -1;
      for (size_t index = 0; index < wifiScanResultCount; ++index) {
        if (wifiScanResults[index].ssid == ssid) {
          targetIndex = static_cast<int>(index);
          break;
        }
      }

      if (targetIndex >= 0) {
        if (WiFi.RSSI(networkIndex) > wifiScanResults[targetIndex].rssi) {
          wifiScanResults[targetIndex].rssi = WiFi.RSSI(networkIndex);
          wifiScanResults[targetIndex].encryptionType = WiFi.encryptionType(networkIndex);
        }
        wifiScanResults[targetIndex].known = existingIndex >= 0;
        continue;
      }

      if (wifiScanResultCount >= kMaxWifiScanResults) {
        continue;
      }

      wifiScanResults[wifiScanResultCount].ssid = ssid;
      wifiScanResults[wifiScanResultCount].rssi = WiFi.RSSI(networkIndex);
      wifiScanResults[wifiScanResultCount].encryptionType = WiFi.encryptionType(networkIndex);
      wifiScanResults[wifiScanResultCount].known = existingIndex >= 0;
      ++wifiScanResultCount;
    }
  }

  wifiScanInProgress = false;
  wifiScanCompletedAtMs = millis();
  WiFi.scanDelete();
}

void beginWifiConnection() {
  beginNextWifiConnectionAttempt();
}

void ensureWifiConnected() {
  if (WiFi.status() == WL_CONNECTED) {
    return;
  }

  const uint32_t now = millis();
  if (wifiConnectInProgress && now - wifiConnectStartedAtMs >= kWifiConnectTimeoutMs) {
    wifiConnectInProgress = false;
    if (wifiLastError.isEmpty() && !wifiCurrentTargetSsid.isEmpty()) {
      wifiLastError = String(F("Connection timed out for ")) + wifiCurrentTargetSsid;
    }
  }

  if (wifiConnectInProgress || wifiImmediateConnectPending) {
    return;
  }

  if (lastWifiAttemptMs == 0 || now - lastWifiAttemptMs >= kWifiReconnectIntervalMs) {
    beginWifiConnection();
  }
}

void updateWifiServices() {
  captureWifiScanResults();
  processPendingWifiConnect();

  const wl_status_t currentStatus = WiFi.status();
  if (currentStatus != lastWifiStatus) {
    if (lastWifiStatus == WL_CONNECTED && currentStatus != WL_CONNECTED) {
      resetWifiAttemptCursor();
    }

    logWifiStatus(currentStatus);
    if (currentStatus != WL_CONNECTED) {
      stopMdnsIfNeeded();
    }
    lastWifiStatus = currentStatus;
  }

  if (currentStatus == WL_CONNECTED) {
    startHttpServerIfNeeded();
    startMdnsIfNeeded();
    return;
  }

  ensureWifiConnected();
}
