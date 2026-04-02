#pragma once

bool wifiAppAvailable() {
  return true;
}

String buildWifiJson() {
  String json;
  json.reserve(4096);

  const bool connected = WiFi.status() == WL_CONNECTED;
  const bool connecting = wifiConnectInProgress || wifiImmediateConnectPending;
  const String liveSsid = currentWifiLiveSsid();

  json += F("{\"available\":true,\"status\":\"");
  json += currentWifiStatusLabel();
  json += F("\",\"connected\":");
  json += connected ? F("true") : F("false");
  json += F(",\"connecting\":");
  json += connecting ? F("true") : F("false");
  json += F(",\"scanning\":");
  json += wifiScanInProgress ? F("true") : F("false");
  json += F(",\"hostname\":\"");
  json += kMdnsAddress;
  json += F("\",\"ssid\":\"");
  json += jsonEscape(liveSsid);
  json += F("\",\"targetSsid\":\"");
  json += jsonEscape(wifiCurrentTargetSsid);
  json += F("\",\"ip\":\"");
  json += jsonEscape(currentIpString());
  json += F("\",\"activeSource\":\"");
  json += currentWifiSource();
  json += F("\",\"lastError\":\"");
  json += jsonEscape(wifiLastError);
  json += F("\",\"fallbackConfigured\":");
  json += hasFallbackWifiCredentials() ? F("true") : F("false");
  json += F(",\"scanStartedAtMs\":");
  json += String(wifiScanStartedAtMs);
  json += F(",\"scanCompletedAtMs\":");
  json += String(wifiScanCompletedAtMs);
  json += F(",\"connectStartedAtMs\":");
  json += String(wifiConnectStartedAtMs);
  json += F(",\"savedNetworks\":[");

  for (size_t index = 0; index < savedWifiNetworkCount; ++index) {
    if (index > 0) {
      json += ',';
    }

    json += F("{\"ssid\":\"");
    json += jsonEscape(savedWifiNetworks[index].ssid);
    json += F("\",\"priority\":");
    json += String(index + 1);
    json += F(",\"connected\":");
    json += connected && !wifiCurrentTargetIsFallback && wifiCurrentTargetProfileIndex == static_cast<int>(index) ? F("true") : F("false");
    json += F("}");
  }

  json += F("],\"scanResults\":[");

  for (size_t index = 0; index < wifiScanResultCount; ++index) {
    if (index > 0) {
      json += ',';
    }

    json += F("{\"ssid\":\"");
    json += jsonEscape(wifiScanResults[index].ssid);
    json += F("\",\"rssi\":");
    json += String(wifiScanResults[index].rssi);
    json += F(",\"encryptionType\":\"");
    json += wifiEncryptionTypeLabel(wifiScanResults[index].encryptionType);
    json += F("\",\"known\":");
    json += wifiScanResults[index].known ? F("true") : F("false");
    json += F("}");
  }

  json += F("]}");
  return json;
}

void handleWifiState() {
  sendJson(200, buildWifiJson());
}

void handleWifiScan() {
  if (!startWifiScan()) {
    sendJsonError(500, wifiLastError.isEmpty() ? String(F("Unable to start Wi-Fi scan.")) : wifiLastError);
    return;
  }

  sendJson(200, buildWifiJson());
}

void handleWifiConnect() {
  String ssid;
  String password;

  if (webServer.hasArg(F("plain"))) {
    const String body = webServer.arg(F("plain"));
    ssid = extractJsonStringField(body, "ssid");
    password = extractJsonStringField(body, "password");
  }

  if (ssid.isEmpty() && webServer.hasArg(F("ssid"))) {
    ssid = webServer.arg(F("ssid"));
  }

  if (!webServer.hasArg(F("plain")) && webServer.hasArg(F("password"))) {
    password = webServer.arg(F("password"));
  }

  ssid = normalizeWifiSsid(ssid);
  if (ssid.isEmpty()) {
    sendJsonError(400, F("Request body must include a non-empty ssid."));
    return;
  }

  insertOrUpdateSavedWifiNetwork(ssid, password);
  queueImmediateWifiConnection(0);
  sendJson(200, buildWifiJson());
}

void handleWifiReorder() {
  String orderedSsids[kMaxSavedWifiNetworks];
  size_t orderedCount = 0;
  bool hasSsidsField = false;

  if (webServer.hasArg(F("plain"))) {
    const String body = webServer.arg(F("plain"));
    hasSsidsField = body.indexOf(F("\"ssids\"")) >= 0;
    orderedCount = extractJsonStringArrayField(body, "ssids", orderedSsids, kMaxSavedWifiNetworks);
  }

  if (!hasSsidsField) {
    sendJsonError(400, F("Request body must include an ssids array."));
    return;
  }

  if (!reorderSavedWifiNetworks(orderedSsids, orderedCount)) {
    sendJsonError(400, F("The ssids array must contain each saved network exactly once in the new priority order."));
    return;
  }

  sendJson(200, buildWifiJson());
}

void handleWifiForget() {
  String ssid;
  if (webServer.hasArg(F("plain"))) {
    ssid = extractJsonStringField(webServer.arg(F("plain")), "ssid");
  }

  if (ssid.isEmpty() && webServer.hasArg(F("ssid"))) {
    ssid = webServer.arg(F("ssid"));
  }

  ssid = normalizeWifiSsid(ssid);
  if (ssid.isEmpty()) {
    sendJsonError(400, F("Request body must include a non-empty ssid."));
    return;
  }

  if (!forgetSavedWifiNetwork(ssid)) {
    sendJsonError(400, String(F("Unknown saved Wi-Fi network: ")) + ssid);
    return;
  }

  sendJson(200, buildWifiJson());
}

void registerWifiAppRoutes() {
  webServer.on(F("/api/wifi"), HTTP_GET, handleWifiState);
  webServer.on(F("/api/wifi/scan"), HTTP_POST, handleWifiScan);
  webServer.on(F("/api/wifi/connect"), HTTP_POST, handleWifiConnect);
  webServer.on(F("/api/wifi/reorder"), HTTP_POST, handleWifiReorder);
  webServer.on(F("/api/wifi/forget"), HTTP_POST, handleWifiForget);
}
