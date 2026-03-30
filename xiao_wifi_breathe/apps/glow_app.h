#pragma once

bool glowAppAvailable() {
  return matrixReady;
}

bool matrixGlowActive() {
  return activeMatrixPattern != nullptr && strcmp(activeMatrixPattern->id, kMatrixSolidPatternId) == 0;
}

String buildGlowJson() {
  String json;
  json.reserve(768);

  json += F("{\"available\":");
  json += matrixReady ? F("true") : F("false");
  json += F(",\"enabled\":");
  json += matrixEnabled ? F("true") : F("false");
  json += F(",\"brightness\":");
  json += String(matrixBrightness);
  json += F(",\"animationSpeed\":");
  json += String(matrixAnimationSpeed);
  json += F(",\"color\":\"");
  json += matrixColorHex;
  json += F("\",\"glowActive\":");
  json += matrixGlowActive() ? F("true") : F("false");
  json += F(",\"selectedPatternId\":\"");
  json += activeMatrixPattern->id;
  json += F("\",\"selectedPatternLabel\":\"");
  json += activeMatrixPattern->label;
  json += F("\"}");

  return json;
}

void handleGlowState() {
  sendJson(200, buildGlowJson());
}

void handleGlowUpdate() {
  String nextColor;
  if (webServer.hasArg(F("plain"))) {
    nextColor = extractJsonStringField(webServer.arg(F("plain")), "color");
  }

  if (nextColor.isEmpty() && webServer.hasArg(F("color"))) {
    nextColor = webServer.arg(F("color"));
  }

  if (nextColor.isEmpty()) {
    sendJsonError(400, F("Request body must include a #RRGGBB color value."));
    return;
  }

  if (!updateMatrixColor(nextColor, false)) {
    sendJsonError(400, F("Matrix color must be a #RRGGBB hex value."));
    return;
  }

  if (!selectMatrixPatternById(kMatrixSolidPatternId, false)) {
    sendJsonError(500, F("Solid glow pattern is unavailable."));
    return;
  }

  persistMatrixState();
  applyMatrixFrameNow();

  sendJson(200, buildGlowJson());
}

void registerGlowAppRoutes() {
  webServer.on(F("/api/glow"), HTTP_GET, handleGlowState);
  webServer.on(F("/api/glow"), HTTP_POST, handleGlowUpdate);
}
