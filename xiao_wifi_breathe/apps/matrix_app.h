#pragma once

bool matrixAppAvailable() {
  return matrixReady;
}

String buildMatrixJson() {
  String json;
  json.reserve(4352);

  json += F("{\"available\":");
  json += matrixReady ? F("true") : F("false");
  json += F(",\"error\":\"");
  json += jsonEscape(matrixError);
  json += F("\",\"dataPin\":\"A0/D0\",");
  json += F("\"gpio\":");
  json += String(kMatrixPin);
  json += F(",\"rows\":");
  json += String(kMatrixRows);
  json += F(",\"columns\":");
  json += String(kMatrixColumns);
  json += F(",\"pixelCount\":");
  json += String(kMatrixLedCount);
  json += F(",\"selectedPatternId\":\"");
  json += activeMatrixPattern->id;
  json += F("\",\"selectedPatternLabel\":\"");
  json += activeMatrixPattern->label;
  json += F("\",\"brightness\":");
  json += String(matrixBrightness);
  json += F(",\"color\":\"");
  json += matrixColorHex;
  json += F("\",\"mappingId\":\"");
  json += activeMatrixMapping->id;
  json += F("\",\"mappingLabel\":\"");
  json += activeMatrixMapping->label;
  json += F("\",\"moodId\":\"");
  json += jsonEscape(matrixMoodId);
  json += F("\",\"messageText\":\"");
  json += jsonEscape(matrixMessageText);
  json += F("\",\"mappings\":[");

  for (size_t index = 0; index < sizeof(kMatrixMappings) / sizeof(kMatrixMappings[0]); ++index) {
    if (index > 0) {
      json += ',';
    }

    json += F("{\"id\":\"");
    json += kMatrixMappings[index].id;
    json += F("\",\"label\":\"");
    json += kMatrixMappings[index].label;
    json += F("\"}");
  }

  json += F("],\"patterns\":[");

  for (size_t index = 0; index < sizeof(kMatrixPatterns) / sizeof(kMatrixPatterns[0]); ++index) {
    if (index > 0) {
      json += ',';
    }

    json += F("{\"id\":\"");
    json += kMatrixPatterns[index].id;
    json += F("\",\"label\":\"");
    json += kMatrixPatterns[index].label;
    json += F("\"}");
  }

  json += F("],\"frame\":[");
  bool firstPixel = true;
  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      if (!firstPixel) {
        json += ',';
      }
      firstPixel = false;
      json += '"';
      json += colorToHex(matrixPreviewColorAt(row, column));
      json += '"';
    }
  }

  json += F("]}");
  return json;
}

void handleMatrixState() {
  sendJson(200, buildMatrixJson());
}

void handleMatrixUpdate() {
  const String body = webServer.hasArg(F("plain")) ? webServer.arg(F("plain")) : String("");

  String nextPatternId = extractJsonStringField(body, "patternId");
  if (nextPatternId.isEmpty() && webServer.hasArg(F("patternId"))) {
    nextPatternId = webServer.arg(F("patternId"));
  }

  String nextColor = extractJsonStringField(body, "color");
  if (nextColor.isEmpty() && webServer.hasArg(F("color"))) {
    nextColor = webServer.arg(F("color"));
  }

  String nextMappingId = extractJsonStringField(body, "mappingId");
  if (nextMappingId.isEmpty() && webServer.hasArg(F("mappingId"))) {
    nextMappingId = webServer.arg(F("mappingId"));
  }

  int nextBrightness = extractJsonIntField(body, "brightness");
  if (nextBrightness == kJsonFieldMissing && webServer.hasArg(F("brightness"))) {
    nextBrightness = webServer.arg(F("brightness")).toInt();
  }

  if (nextPatternId.isEmpty() && nextColor.isEmpty() && nextMappingId.isEmpty() && nextBrightness == kJsonFieldMissing) {
    sendJsonError(400, F("Request body must include patternId, color, mappingId, or brightness."));
    return;
  }

  const MatrixPatternDefinition *candidatePattern = activeMatrixPattern;
  const MatrixMappingDefinition *candidateMapping = activeMatrixMapping;
  RgbColor candidateColor = matrixBaseColor;
  String candidateColorHex = matrixColorHex;

  if (!nextPatternId.isEmpty()) {
    candidatePattern = findMatrixPatternById(nextPatternId);
    if (candidatePattern == nullptr) {
      sendJsonError(400, String(F("Unknown matrix pattern id: ")) + nextPatternId);
      return;
    }
  }

  if (!nextColor.isEmpty() && !parseHexColor(nextColor, candidateColor, candidateColorHex)) {
    sendJsonError(400, F("Matrix color must be a #RRGGBB hex value."));
    return;
  }

  if (!nextMappingId.isEmpty()) {
    candidateMapping = findMatrixMappingById(nextMappingId);
    if (candidateMapping == nullptr) {
      sendJsonError(400, String(F("Unknown matrix mapping id: ")) + nextMappingId);
      return;
    }
  }

  if (nextBrightness != kJsonFieldMissing && (nextBrightness < 0 || nextBrightness > 255)) {
    sendJsonError(400, F("Matrix brightness must be between 0 and 255."));
    return;
  }

  activeMatrixPattern = candidatePattern;
  activeMatrixMapping = candidateMapping;
  matrixBaseColor = candidateColor;
  matrixColorHex = candidateColorHex;
  if (nextBrightness != kJsonFieldMissing) {
    matrixBrightness = static_cast<uint8_t>(nextBrightness);
  }

  matrixFrameDirty = true;
  persistMatrixState();
  applyMatrixFrameNow();

  sendJson(200, buildMatrixJson());
}

void registerMatrixAppRoutes() {
  webServer.on(F("/api/matrix"), HTTP_GET, handleMatrixState);
  webServer.on(F("/api/matrix"), HTTP_POST, handleMatrixUpdate);
}
