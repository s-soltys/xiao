#pragma once

bool deviceAppAvailable() {
  return true;
}

String buildDeviceJson() {
  String json;
  json.reserve(1024);

  json += F("{\"chipModel\":\"");
  json += jsonEscape(String(ESP.getChipModel()));
  json += F("\",\"chipRevision\":");
  json += String(ESP.getChipRevision());
  json += F(",\"chipCores\":");
  json += String(ESP.getChipCores());
  json += F(",\"sdkVersion\":\"");
  json += jsonEscape(String(ESP.getSdkVersion()));
  json += F("\",\"cpuFreqMHz\":");
  json += String(ESP.getCpuFreqMHz());
  json += F(",\"uptimeMs\":");
  json += String(millis());
  json += F(",\"temperatureC\":");
  json += floatToJson(temperatureRead());
  json += F(",\"heapSize\":");
  json += String(ESP.getHeapSize());
  json += F(",\"freeHeap\":");
  json += String(ESP.getFreeHeap());
  json += F(",\"minFreeHeap\":");
  json += String(ESP.getMinFreeHeap());
  json += F(",\"psramSize\":");
  json += String(ESP.getPsramSize());
  json += F(",\"flashChipSize\":");
  json += String(ESP.getFlashChipSize());
  json += F(",\"sketchSize\":");
  json += String(ESP.getSketchSize());
  json += F(",\"freeSketchSpace\":");
  json += String(ESP.getFreeSketchSpace());
  json += F(",\"efuseMac\":\"");
  json += formatMacAddress(ESP.getEfuseMac());
  json += F("\",\"wifiConnected\":");
  json += WiFi.status() == WL_CONNECTED ? F("true") : F("false");
  json += F(",\"wifiRssi\":");
  if (WiFi.status() == WL_CONNECTED) {
    json += String(WiFi.RSSI());
  } else {
    json += F("null");
  }
  json += F(",\"ip\":\"");
  json += jsonEscape(currentIpString());
  json += F("\",\"bleReady\":");
  json += bleReady ? F("true") : F("false");
  json += F(",\"bleStack\":\"");
  json += bleReady ? jsonEscape(BLEDevice::getBLEStackString()) : String("unavailable");
  json += F("\"}");

  return json;
}

void handleDeviceState() {
  sendJson(200, buildDeviceJson());
}

void registerDeviceAppRoutes() {
  webServer.on(F("/api/device"), HTTP_GET, handleDeviceState);
}
