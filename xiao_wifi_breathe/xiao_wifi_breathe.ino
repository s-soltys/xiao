#include <ESPmDNS.h>
#include <Preferences.h>
#include <WebServer.h>
#include <WiFi.h>
#include <ctype.h>
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
constexpr uint16_t kMorseUnitMs = 180;
constexpr size_t kMaxMorseTextLength = 64;
constexpr size_t kMaxMorseSteps = 512;
constexpr char kPreferenceNamespace[] = "xiao-app";
constexpr char kPatternPreferenceKey[] = "pattern";
constexpr char kMorseTextPreferenceKey[] = "morseText";
constexpr char kMorsePatternId[] = "morse-text";
constexpr char kMorsePatternLabel[] = "Custom Morse";

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
PatternStep morseSteps[kMaxMorseSteps];
PatternDefinition morsePattern = {kMorsePatternId, kMorsePatternLabel, PatternMode::kSequence, morseSteps, 0, 0};

const PatternDefinition *activePattern = &kPatterns[0];
String morseText;
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
  if (patternId == kMorsePatternId) {
    return morsePattern.stepCount > 0 ? &morsePattern : nullptr;
  }

  for (const PatternDefinition &pattern : kPatterns) {
    if (patternId == pattern.id) {
      return &pattern;
    }
  }

  return nullptr;
}

const char *lookupMorseCode(char character) {
  switch (character) {
    case 'A':
      return ".-";
    case 'B':
      return "-...";
    case 'C':
      return "-.-.";
    case 'D':
      return "-..";
    case 'E':
      return ".";
    case 'F':
      return "..-.";
    case 'G':
      return "--.";
    case 'H':
      return "....";
    case 'I':
      return "..";
    case 'J':
      return ".---";
    case 'K':
      return "-.-";
    case 'L':
      return ".-..";
    case 'M':
      return "--";
    case 'N':
      return "-.";
    case 'O':
      return "---";
    case 'P':
      return ".--.";
    case 'Q':
      return "--.-";
    case 'R':
      return ".-.";
    case 'S':
      return "...";
    case 'T':
      return "-";
    case 'U':
      return "..-";
    case 'V':
      return "...-";
    case 'W':
      return ".--";
    case 'X':
      return "-..-";
    case 'Y':
      return "-.--";
    case 'Z':
      return "--..";
    case '0':
      return "-----";
    case '1':
      return ".----";
    case '2':
      return "..---";
    case '3':
      return "...--";
    case '4':
      return "....-";
    case '5':
      return ".....";
    case '6':
      return "-....";
    case '7':
      return "--...";
    case '8':
      return "---..";
    case '9':
      return "----.";
    case '.':
      return ".-.-.-";
    case ',':
      return "--..--";
    case '?':
      return "..--..";
    case '!':
      return "-.-.--";
    case '-':
      return "-....-";
    case '/':
      return "-..-.";
    case '@':
      return ".--.-.";
    case '(':
      return "-.--.";
    case ')':
      return "-.--.-";
    default:
      return nullptr;
  }
}

String normalizeMorseText(const String &rawText) {
  String normalized;
  normalized.reserve(min(rawText.length(), kMaxMorseTextLength));
  bool previousWasSpace = true;

  for (size_t index = 0; index < rawText.length(); ++index) {
    char current = static_cast<char>(toupper(static_cast<unsigned char>(rawText.charAt(index))));
    if (isspace(static_cast<unsigned char>(current))) {
      if (!previousWasSpace && !normalized.isEmpty()) {
        normalized += ' ';
        previousWasSpace = true;
      }
      continue;
    }

    if (lookupMorseCode(current) == nullptr) {
      continue;
    }

    if (normalized.length() >= kMaxMorseTextLength) {
      break;
    }

    normalized += current;
    previousWasSpace = false;
  }

  while (!normalized.isEmpty() && normalized.charAt(normalized.length() - 1) == ' ') {
    normalized.remove(normalized.length() - 1);
  }

  return normalized;
}

bool appendMorseStep(uint8_t brightness, uint16_t durationMs) {
  if (morsePattern.stepCount >= kMaxMorseSteps) {
    return false;
  }

  morseSteps[morsePattern.stepCount++] = {brightness, durationMs};
  return true;
}

bool buildMorsePattern(const String &normalizedText) {
  morsePattern.stepCount = 0;

  for (size_t index = 0; index < normalizedText.length(); ++index) {
    const char current = normalizedText.charAt(index);
    if (current == ' ') {
      continue;
    }

    const char *code = lookupMorseCode(current);
    if (code == nullptr) {
      continue;
    }

    const size_t codeLength = strlen(code);
    for (size_t symbolIndex = 0; symbolIndex < codeLength; ++symbolIndex) {
      const uint16_t onDuration = code[symbolIndex] == '-' ? (kMorseUnitMs * 3) : kMorseUnitMs;
      if (!appendMorseStep(255, onDuration)) {
        return false;
      }

      if (symbolIndex + 1 < codeLength && !appendMorseStep(0, kMorseUnitMs)) {
        return false;
      }
    }

    bool nextIsWordBreak = false;
    size_t nextIndex = index + 1;
    while (nextIndex < normalizedText.length() && normalizedText.charAt(nextIndex) == ' ') {
      nextIsWordBreak = true;
      ++nextIndex;
    }

    if (nextIndex < normalizedText.length()) {
      if (!appendMorseStep(0, nextIsWordBreak ? (kMorseUnitMs * 7) : (kMorseUnitMs * 3))) {
        return false;
      }
    }
  }

  if (morsePattern.stepCount == 0) {
    return false;
  }

  return appendMorseStep(0, kMorseUnitMs * 7);
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

bool activateMorseText(const String &rawText, bool persistSelection) {
  const String normalizedText = normalizeMorseText(rawText);
  if (normalizedText.isEmpty()) {
    return false;
  }

  if (!buildMorsePattern(normalizedText)) {
    return false;
  }

  morseText = normalizedText;
  activePattern = &morsePattern;
  resetPatternState();

  if (persistSelection) {
    preferences.putString(kPatternPreferenceKey, kMorsePatternId);
    preferences.putString(kMorseTextPreferenceKey, morseText);
  }

  return true;
}

void loadSavedPattern() {
  morseText = normalizeMorseText(preferences.getString(kMorseTextPreferenceKey, ""));
  const String storedPattern = preferences.getString(kPatternPreferenceKey, kPatterns[0].id);

  if (storedPattern == kMorsePatternId && activateMorseText(morseText, false)) {
    return;
  }

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

void handleNotFound() {
  sendJsonError(404, F("Not found."));
}

void configureHttpServer() {
  webServer.on(F("/"), HTTP_GET, handleRoot);
  webServer.on(F("/api/state"), HTTP_GET, handleState);
  webServer.on(F("/api/pattern"), HTTP_POST, handlePatternChange);
  webServer.on(F("/api/morse"), HTTP_POST, handleMorseChange);
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
