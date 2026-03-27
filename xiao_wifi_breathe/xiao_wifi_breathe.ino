#include <ESPmDNS.h>
#include <Preferences.h>
#include <WebServer.h>
#include <WiFi.h>
#include <math.h>
#include <string.h>

#include "web_app.h"

#if __has_include("wifi_config.h")
#include "wifi_config.h"
#else
constexpr char kWifiSsid[] = "";
constexpr char kWifiPassword[] = "";
#endif

#ifndef LED_BUILTIN
#define LED_BUILTIN 15
#endif

namespace {

constexpr char kHostname[] = "xiao";
constexpr char kMdnsAddress[] = "xiao.local";
constexpr uint16_t kHttpPort = 80;
constexpr uint8_t kLedPin = LED_BUILTIN;
constexpr bool kLedActiveLow = true;
constexpr uint8_t kPwmMax = 255;
constexpr uint32_t kWifiReconnectIntervalMs = 15000;
constexpr char kPreferenceNamespace[] = "xiao-app";
constexpr char kPatternPreferenceKey[] = "pattern";

struct PatternStep {
  uint8_t brightness;
  uint16_t durationMs;
};

enum class PatternMode : uint8_t {
  kSequence,
  kCosineWave,
  kTriangleWave,
};

struct PatternDefinition {
  const char *id;
  const char *label;
  PatternMode mode;
  const PatternStep *steps;
  size_t stepCount;
  uint32_t cycleMs;
};

const PatternStep kSosSteps[] = {
  {255, 180}, {0, 180}, {255, 180}, {0, 180}, {255, 180}, {0, 540}, {255, 540}, {0, 180}, {255, 540}, {0, 180},
  {255, 540}, {0, 540}, {255, 180}, {0, 180}, {255, 180}, {0, 180}, {255, 180}, {0, 1260},
};

const PatternStep kSlowBlinkSteps[] = {
  {255, 1000},
  {0, 1000},
};

const PatternStep kFastBlinkSteps[] = {
  {255, 150},
  {0, 150},
};

const PatternStep kHeartbeatSteps[] = {
  {255, 120},
  {0, 120},
  {255, 300},
  {0, 900},
};

const PatternStep kBeaconSteps[] = {
  {255, 80},
  {0, 1920},
};

const PatternStep kDoubleBlinkSteps[] = {
  {255, 120},
  {0, 180},
  {255, 120},
  {0, 1080},
};

const PatternStep kTripleBlinkSteps[] = {
  {255, 100},
  {0, 140},
  {255, 100},
  {0, 140},
  {255, 100},
  {0, 1120},
};

const PatternStep kStrobeSteps[] = {
  {255, 50}, {0, 50}, {255, 50}, {0, 50}, {255, 50}, {0, 50}, {255, 50}, {0, 50},
  {255, 50}, {0, 50}, {255, 50}, {0, 50}, {255, 50}, {0, 50}, {255, 50}, {0, 700},
};

const PatternDefinition kPatterns[] = {
  {"breathing", "Breathing", PatternMode::kCosineWave, nullptr, 0, 2200},
  {"sos", "SOS", PatternMode::kSequence, kSosSteps, sizeof(kSosSteps) / sizeof(kSosSteps[0]), 0},
  {"slow-blink", "Slow Blink", PatternMode::kSequence, kSlowBlinkSteps, sizeof(kSlowBlinkSteps) / sizeof(kSlowBlinkSteps[0]), 0},
  {"fast-blink", "Fast Blink", PatternMode::kSequence, kFastBlinkSteps, sizeof(kFastBlinkSteps) / sizeof(kFastBlinkSteps[0]), 0},
  {"heartbeat", "Heartbeat", PatternMode::kSequence, kHeartbeatSteps, sizeof(kHeartbeatSteps) / sizeof(kHeartbeatSteps[0]), 0},
  {"beacon", "Beacon", PatternMode::kSequence, kBeaconSteps, sizeof(kBeaconSteps) / sizeof(kBeaconSteps[0]), 0},
  {"double-blink", "Double Blink", PatternMode::kSequence, kDoubleBlinkSteps, sizeof(kDoubleBlinkSteps) / sizeof(kDoubleBlinkSteps[0]), 0},
  {"triple-blink", "Triple Blink", PatternMode::kSequence, kTripleBlinkSteps, sizeof(kTripleBlinkSteps) / sizeof(kTripleBlinkSteps[0]), 0},
  {"ramp-pulse", "Ramp Pulse", PatternMode::kTriangleWave, nullptr, 0, 1600},
  {"strobe", "Strobe", PatternMode::kSequence, kStrobeSteps, sizeof(kStrobeSteps) / sizeof(kStrobeSteps[0]), 0},
};

WebServer webServer(kHttpPort);
Preferences preferences;

const PatternDefinition *activePattern = &kPatterns[0];
size_t activeStepIndex = 0;
uint32_t activeStepStartedAtMs = 0;
uint32_t lastWifiAttemptMs = 0;
wl_status_t lastWifiStatus = WL_IDLE_STATUS;
bool wifiWarningPrinted = false;
bool webServerStarted = false;
bool mdnsStarted = false;

bool hasWifiCredentials() {
  return kWifiSsid[0] != '\0' && kWifiPassword[0] != '\0';
}

uint8_t gammaCorrect(float normalizedBrightness) {
  normalizedBrightness = constrain(normalizedBrightness, 0.0f, 1.0f);
  const float corrected = powf(normalizedBrightness, 2.2f);
  return static_cast<uint8_t>(corrected * kPwmMax + 0.5f);
}

void setLedBrightness(uint8_t brightness) {
  const uint8_t pwmValue = kLedActiveLow ? (kPwmMax - brightness) : brightness;
  analogWrite(kLedPin, pwmValue);
}

const PatternDefinition *findPatternById(const String &patternId) {
  for (const PatternDefinition &pattern : kPatterns) {
    if (patternId == pattern.id) {
      return &pattern;
    }
  }

  return nullptr;
}

void resetPatternState() {
  activeStepIndex = 0;
  activeStepStartedAtMs = millis();
}

bool selectPatternById(const String &patternId, bool persistSelection) {
  const PatternDefinition *pattern = findPatternById(patternId);
  if (pattern == nullptr) {
    return false;
  }

  activePattern = pattern;
  resetPatternState();

  if (persistSelection) {
    preferences.putString(kPatternPreferenceKey, activePattern->id);
  }

  return true;
}

void loadSavedPattern() {
  const String storedPattern = preferences.getString(kPatternPreferenceKey, kPatterns[0].id);
  if (!selectPatternById(storedPattern, false)) {
    selectPatternById(kPatterns[0].id, true);
  }
}

String jsonEscape(const String &value) {
  String escaped;
  escaped.reserve(value.length() + 8);

  for (size_t index = 0; index < value.length(); ++index) {
    const char current = value.charAt(index);
    switch (current) {
      case '\\':
        escaped += F("\\\\");
        break;
      case '"':
        escaped += F("\\\"");
        break;
      case '\n':
        escaped += F("\\n");
        break;
      case '\r':
        escaped += F("\\r");
        break;
      case '\t':
        escaped += F("\\t");
        break;
      default:
        escaped += current;
        break;
    }
  }

  return escaped;
}

String currentIpString() {
  return WiFi.status() == WL_CONNECTED ? WiFi.localIP().toString() : String("");
}

String buildStateJson() {
  String json;
  json.reserve(1024);

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

void sendJson(int statusCode, const String &body) {
  webServer.sendHeader(F("Cache-Control"), F("no-store"));
  webServer.send(statusCode, F("application/json; charset=utf-8"), body);
}

void sendJsonError(int statusCode, const String &message) {
  sendJson(statusCode, String(F("{\"error\":\"")) + jsonEscape(message) + F("\"}"));
}

String extractJsonStringField(const String &body, const char *fieldName) {
  const String quotedField = String('"') + fieldName + '"';
  const int fieldIndex = body.indexOf(quotedField);
  if (fieldIndex < 0) {
    return String("");
  }

  const int colonIndex = body.indexOf(':', fieldIndex + quotedField.length());
  if (colonIndex < 0) {
    return String("");
  }

  const int startQuoteIndex = body.indexOf('"', colonIndex + 1);
  if (startQuoteIndex < 0) {
    return String("");
  }

  const int endQuoteIndex = body.indexOf('"', startQuoteIndex + 1);
  if (endQuoteIndex < 0) {
    return String("");
  }

  return body.substring(startQuoteIndex + 1, endQuoteIndex);
}

void handleRoot() {
  webServer.sendHeader(F("Cache-Control"), F("no-store"));
  webServer.send_P(200, PSTR("text/html; charset=utf-8"), kWebAppHtml, strlen_P(kWebAppHtml));
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

void handleNotFound() {
  sendJsonError(404, F("Not found."));
}

void configureHttpServer() {
  webServer.on(F("/"), HTTP_GET, handleRoot);
  webServer.on(F("/api/state"), HTTP_GET, handleState);
  webServer.on(F("/api/pattern"), HTTP_POST, handlePatternChange);
  webServer.onNotFound(handleNotFound);
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
      Serial.print(F("Wi-Fi connected. IP: "));
      Serial.println(WiFi.localIP());
      break;
    case WL_NO_SSID_AVAIL:
      Serial.println(F("Wi-Fi error: SSID not found."));
      break;
    case WL_CONNECT_FAILED:
      Serial.println(F("Wi-Fi error: authentication failed."));
      break;
    case WL_CONNECTION_LOST:
      Serial.println(F("Wi-Fi status: connection lost."));
      break;
    case WL_DISCONNECTED:
      Serial.println(F("Wi-Fi status: disconnected."));
      break;
    default:
      break;
  }
}

void beginWifiConnection() {
  if (!hasWifiCredentials()) {
    if (!wifiWarningPrinted) {
      Serial.println(F("Wi-Fi credentials are empty. Edit wifi_config.h to enable the web app."));
      wifiWarningPrinted = true;
    }
    return;
  }

  if (WiFi.getMode() != WIFI_STA) {
    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
  }

  Serial.print(F("Connecting to Wi-Fi SSID: "));
  Serial.println(kWifiSsid);
  WiFi.begin(kWifiSsid, kWifiPassword);
  lastWifiAttemptMs = millis();
}

void ensureWifiConnected() {
  if (!hasWifiCredentials()) {
    return;
  }

  if (WiFi.status() == WL_CONNECTED) {
    return;
  }

  const uint32_t now = millis();
  if (lastWifiAttemptMs == 0 || now - lastWifiAttemptMs >= kWifiReconnectIntervalMs) {
    beginWifiConnection();
  }
}

void updateWifiServices() {
  const wl_status_t currentStatus = WiFi.status();
  if (currentStatus != lastWifiStatus) {
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

void updateSequencePattern(uint32_t now) {
  while (now - activeStepStartedAtMs >= activePattern->steps[activeStepIndex].durationMs) {
    activeStepStartedAtMs += activePattern->steps[activeStepIndex].durationMs;
    activeStepIndex = (activeStepIndex + 1) % activePattern->stepCount;
  }

  setLedBrightness(activePattern->steps[activeStepIndex].brightness);
}

void updateWavePattern(uint32_t now) {
  const uint32_t elapsedMs = (now - activeStepStartedAtMs) % activePattern->cycleMs;
  const float progress = static_cast<float>(elapsedMs) / static_cast<float>(activePattern->cycleMs);

  if (activePattern->mode == PatternMode::kCosineWave) {
    const float normalized = 0.5f - (0.5f * cosf(2.0f * PI * progress));
    setLedBrightness(gammaCorrect(normalized));
    return;
  }

  const float triangle = progress < 0.5f ? progress * 2.0f : (1.0f - progress) * 2.0f;
  setLedBrightness(gammaCorrect(triangle));
}

void updateLedPattern() {
  const uint32_t now = millis();
  if (activePattern->mode == PatternMode::kSequence) {
    updateSequencePattern(now);
    return;
  }

  updateWavePattern(now);
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

}  // namespace

void setup() {
  Serial.begin(115200);
  delay(250);

  pinMode(kLedPin, OUTPUT);
  setLedBrightness(0);
  enableBoardWifiHardware();

  preferences.begin(kPreferenceNamespace, false);
  loadSavedPattern();

  configureHttpServer();
  beginWifiConnection();
}

void loop() {
  updateLedPattern();
  updateWifiServices();

  if (webServerStarted) {
    webServer.handleClient();
  }
}
