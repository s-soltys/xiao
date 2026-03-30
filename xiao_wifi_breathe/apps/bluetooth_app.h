#pragma once

bool bluetoothAppAvailable() {
  return bleReady;
}

String buildBluetoothJson() {
  String json;
  json.reserve(4096);

  json += F("{\"available\":");
  json += bleReady ? F("true") : F("false");
  json += F(",\"scanning\":");
  json += bleScanInProgress ? F("true") : F("false");
  json += F(",\"durationSeconds\":");
  json += String(kBleScanDurationSeconds);
  json += F(",\"lastStartedAtMs\":");
  json += String(bleScanStartedAtMs);
  json += F(",\"lastCompletedAtMs\":");
  json += String(bleScanCompletedAtMs);
  json += F(",\"error\":\"");
  json += jsonEscape(bleScanError);
  json += F("\",\"results\":[");

  if (bleScanMutex != nullptr && xSemaphoreTake(bleScanMutex, pdMS_TO_TICKS(250)) == pdTRUE) {
    for (size_t index = 0; index < bleResultCount; ++index) {
      if (index > 0) {
        json += ',';
      }

      const BleScanResultEntry &entry = bleResults[index];
      json += F("{\"address\":\"");
      json += jsonEscape(entry.address);
      json += F("\",\"name\":\"");
      json += jsonEscape(entry.name);
      json += F("\",\"serviceUuid\":\"");
      json += jsonEscape(entry.serviceUuid);
      json += F("\",\"rssi\":");
      json += String(entry.rssi);
      json += F(",\"hasTxPower\":");
      json += entry.hasTxPower ? F("true") : F("false");
      json += F(",\"txPower\":");
      json += String(entry.txPower);
      json += F(",\"connectable\":");
      json += entry.connectable ? F("true") : F("false");
      json += F(",\"scannable\":");
      json += entry.scannable ? F("true") : F("false");
      json += F(",\"legacy\":");
      json += entry.legacy ? F("true") : F("false");
      json += F(",\"addressType\":\"");
      json += bleAddressTypeLabel(entry.addressType);
      json += F("\"}");
    }

    xSemaphoreGive(bleScanMutex);
  }

  json += F("]}");
  return json;
}

void handleBluetoothState() {
  sendJson(200, buildBluetoothJson());
}

void handleBluetoothScan() {
  if (!startBleScan()) {
    sendJsonError(500, bleScanError.isEmpty() ? String(F("Unable to start Bluetooth scan.")) : bleScanError);
    return;
  }

  sendJson(200, buildBluetoothJson());
}

void registerBluetoothAppRoutes() {
  webServer.on(F("/api/bluetooth"), HTTP_GET, handleBluetoothState);
  webServer.on(F("/api/bluetooth/scan"), HTTP_POST, handleBluetoothScan);
}
