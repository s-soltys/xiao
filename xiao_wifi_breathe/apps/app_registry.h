#pragma once

using AppAvailabilityFn = bool (*)();
using AppRouteRegisterFn = void (*)();

struct AppDefinition {
  const char *id;
  const char *label;
  const char *description;
  const char *routeBase;
  const char *stateRoute;
  const char *actionRoute;
  bool visibleInUi;
  AppAvailabilityFn isAvailable;
  AppRouteRegisterFn registerRoutes;
};

const AppDefinition kRegisteredApps[] = {
  {
    "led",
    "Status LED",
    "Power LED patterns and Morse controls.",
    "/api/state",
    "/api/state",
    "/api/pattern",
    false,
    ledAppAvailable,
    registerLedAppRoutes,
  },
  {
    "matrix",
    "RGB Matrix",
    "Control the 6x10 WS2812B panel on A0/D0 with animated effects.",
    "/api/matrix",
    "/api/matrix",
    "/api/matrix",
    true,
    matrixAppAvailable,
    registerMatrixAppRoutes,
  },
  {
    "mood",
    "Mood App",
    "Show one handmade emoji-style mood icon on the RGB matrix.",
    "/api/mood",
    "/api/mood",
    "/api/mood",
    true,
    moodAppAvailable,
    registerMoodAppRoutes,
  },
  {
    "message",
    "Message App",
    "Send a message that loops right-to-left across the RGB matrix.",
    "/api/message",
    "/api/message",
    "/api/message",
    true,
    messageAppAvailable,
    registerMessageAppRoutes,
  },
  {
    "bluetooth",
    "Bluetooth Scanner",
    "Scan nearby BLE advertisers and inspect signal details.",
    "/api/bluetooth",
    "/api/bluetooth",
    "/api/bluetooth/scan",
    true,
    bluetoothAppAvailable,
    registerBluetoothAppRoutes,
  },
  {
    "device",
    "Device Info",
    "Chip telemetry, internal temperature, memory, flash, and radio status.",
    "/api/device",
    "/api/device",
    "",
    true,
    deviceAppAvailable,
    registerDeviceAppRoutes,
  },
};

bool isAppAvailable(const AppDefinition &app) {
  return app.isAvailable == nullptr ? true : app.isAvailable();
}

String buildAppsJson() {
  String json;
  json.reserve(2048);
  json += F("{\"apps\":[");

  bool first = true;
  for (const AppDefinition &app : kRegisteredApps) {
    if (!app.visibleInUi) {
      continue;
    }

    if (!first) {
      json += ',';
    }
    first = false;

    json += F("{\"id\":\"");
    json += app.id;
    json += F("\",\"label\":\"");
    json += jsonEscape(String(app.label));
    json += F("\",\"description\":\"");
    json += jsonEscape(String(app.description));
    json += F("\",\"routeBase\":\"");
    json += app.routeBase;
    json += F("\",\"stateRoute\":\"");
    json += app.stateRoute;
    json += F("\",\"actionRoute\":\"");
    json += app.actionRoute;
    json += F("\",\"available\":");
    json += isAppAvailable(app) ? F("true") : F("false");
    json += F("}");
  }

  json += F("]}");
  return json;
}

void handleRoot() {
  sendWebAppHtml(webServer);
}

void handleAppsIndex() {
  sendJson(200, buildAppsJson());
}

void handleNotFound() {
  sendJsonError(404, F("Not found."));
}

void configureHttpServer() {
  webServer.on(F("/"), HTTP_GET, handleRoot);
  webServer.on(F("/api/apps"), HTTP_GET, handleAppsIndex);

  for (const AppDefinition &app : kRegisteredApps) {
    if (app.registerRoutes != nullptr) {
      app.registerRoutes();
    }
  }

  webServer.onNotFound(handleNotFound);
}
