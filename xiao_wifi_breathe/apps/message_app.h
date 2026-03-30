#pragma once

bool messageAppAvailable() {
  return matrixReady;
}

String buildMessageJson() {
  String json;
  json.reserve(512);

  json += F("{\"available\":");
  json += matrixReady ? F("true") : F("false");
  json += F(",\"selectedPatternId\":\"");
  json += activeMatrixPattern->id;
  json += F("\",\"text\":\"");
  json += jsonEscape(matrixMessageText);
  json += F("\",\"maxLength\":");
  json += String(kMaxMatrixMessageLength);
  json += F(",\"supportedCharacters\":\"A-Z, 0-9, space, . , ! ? -\",");
  json += F("\"scrollDirection\":\"right-to-left\"}");

  return json;
}

void handleMessageState() {
  sendJson(200, buildMessageJson());
}

void handleMessageChange() {
  String nextMessage;
  if (webServer.hasArg(F("plain"))) {
    nextMessage = extractJsonStringField(webServer.arg(F("plain")), "text");
  }

  if (nextMessage.isEmpty() && webServer.hasArg(F("text"))) {
    nextMessage = webServer.arg(F("text"));
  }

  if (!activateMatrixMessage(nextMessage, true)) {
    sendJsonError(400, F("Message text must contain A-Z, 0-9, spaces, or . , ! ? -."));
    return;
  }

  applyMatrixFrameNow();
  sendJson(200, buildMessageJson());
}

void registerMessageAppRoutes() {
  webServer.on(F("/api/message"), HTTP_GET, handleMessageState);
  webServer.on(F("/api/message"), HTTP_POST, handleMessageChange);
}
