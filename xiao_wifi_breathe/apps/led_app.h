#pragma once

bool ledAppAvailable() {
  return true;
}

String buildStateJson() {
  String json;
  json.reserve(1280);

  json += F("{\"hostname\":\"");
  json += kMdnsAddress;
  json += F("\",\"connected\":");
  json += WiFi.status() == WL_CONNECTED ? F("true") : F("false");
  json += F(",\"ssid\":\"");
  json += jsonEscape(String(kWifiSsid));
  json += F("\",\"ip\":\"");
  json += jsonEscape(currentIpString());
  json += F("\",\"selectedPatternId\":\"");
  json += activePattern->id;
  json += F("\",\"selectedPatternLabel\":\"");
  json += activePattern->label;
  json += F("\",\"morseText\":\"");
  json += jsonEscape(morseText);
  json += F("\",\"patterns\":[");

  for (size_t index = 0; index < sizeof(kPatterns) / sizeof(kPatterns[0]); ++index) {
    const PatternDefinition &pattern = kPatterns[index];
    if (index > 0) {
      json += ',';
    }

    json += F("{\"id\":\"");
    json += pattern.id;
    json += F("\",\"label\":\"");
    json += pattern.label;
    json += F("\"}");
  }

  json += F("]}");
  return json;
}

void handleState() {
  sendJson(200, buildStateJson());
}

void handlePatternChange() {
  String patternId;
  if (webServer.hasArg(F("plain"))) {
    patternId = extractJsonStringField(webServer.arg(F("plain")), "id");
  }

  if (patternId.isEmpty() && webServer.hasArg(F("id"))) {
    patternId = webServer.arg(F("id"));
  }

  if (patternId.isEmpty()) {
    sendJsonError(400, F("Request body must include a non-empty pattern id."));
    return;
  }

  if (!selectPatternById(patternId, true)) {
    sendJsonError(400, String(F("Unknown pattern id: ")) + patternId);
    return;
  }

  sendJson(200, buildStateJson());
}

void handleMorseChange() {
  String nextMorseText;
  if (webServer.hasArg(F("plain"))) {
    nextMorseText = extractJsonStringField(webServer.arg(F("plain")), "text");
  }

  if (nextMorseText.isEmpty() && webServer.hasArg(F("text"))) {
    nextMorseText = webServer.arg(F("text"));
  }

  if (!activateMorseText(nextMorseText, true)) {
    sendJsonError(400, F("Morse text must contain supported letters, digits, spaces, or . , ? ! - / @ ( )."));
    return;
  }

  sendJson(200, buildStateJson());
}

void registerLedAppRoutes() {
  webServer.on(F("/api/state"), HTTP_GET, handleState);
  webServer.on(F("/api/pattern"), HTTP_POST, handlePatternChange);
  webServer.on(F("/api/morse"), HTTP_POST, handleMorseChange);
}
