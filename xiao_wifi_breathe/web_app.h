#pragma once

#include "ui/web_app_document_start.h"
#include "ui/web_app_shared_components.h"
#include "ui/web_app_mood_panel.h"
#include "ui/web_app_message_panel.h"
#include "ui/web_app_matrix_panel.h"
#include "ui/web_app_bluetooth_panel.h"
#include "ui/web_app_device_panel.h"
#include "ui/web_app_app_shell.h"
#include "ui/web_app_document_end.h"

inline void sendHtmlChunk(WebServer &server, PGM_P chunk) {
  server.sendContent_P(chunk, strlen_P(chunk));
}

inline void sendWebAppHtml(WebServer &server) {
  server.sendHeader(F("Cache-Control"), F("no-store"));
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, F("text/html; charset=utf-8"), String());
  sendHtmlChunk(server, kWebAppDocumentStart);
  sendHtmlChunk(server, kWebAppSharedComponents);
  sendHtmlChunk(server, kWebAppMoodPanel);
  sendHtmlChunk(server, kWebAppMessagePanel);
  sendHtmlChunk(server, kWebAppMatrixPanel);
  sendHtmlChunk(server, kWebAppBluetoothPanel);
  sendHtmlChunk(server, kWebAppDevicePanel);
  sendHtmlChunk(server, kWebAppAppShell);
  sendHtmlChunk(server, kWebAppDocumentEnd);
  server.sendContent("");
}
