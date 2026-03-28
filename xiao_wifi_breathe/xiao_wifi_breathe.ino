#include <Adafruit_NeoPixel.h>
#include <BLEDevice.h>
#include <ESPmDNS.h>
#include <Preferences.h>
#include <WebServer.h>
#include <WiFi.h>
#include <ctype.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <math.h>
#include <stdio.h>
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
constexpr uint32_t kBleScanDurationSeconds = 5;
constexpr size_t kMaxBleDevices = 20;
constexpr uint8_t kMatrixPin = A0;
constexpr uint8_t kMatrixRows = 6;
constexpr uint8_t kMatrixColumns = 10;
constexpr size_t kMatrixLedCount = kMatrixRows * kMatrixColumns;
constexpr uint16_t kMatrixFrameIntervalMs = 40;
constexpr uint8_t kMatrixDefaultBrightness = 48;
constexpr char kMatrixDefaultColorHex[] = "#22c55e";
constexpr char kPreferenceNamespace[] = "xiao-app";
constexpr char kPatternPreferenceKey[] = "pattern";
constexpr char kMorseTextPreferenceKey[] = "morseText";
constexpr char kMatrixPatternPreferenceKey[] = "matrixPattern";
constexpr char kMatrixBrightnessPreferenceKey[] = "matrixBright";
constexpr char kMatrixColorPreferenceKey[] = "matrixColor";
constexpr char kMorsePatternId[] = "morse-text";
constexpr char kMorsePatternLabel[] = "Custom Morse";
constexpr int kJsonFieldMissing = -1000;

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

struct BleScanResultEntry {
  String address;
  String name;
  String serviceUuid;
  int rssi;
  bool hasTxPower;
  int txPower;
  bool connectable;
  bool scannable;
  bool legacy;
  uint8_t addressType;
};

struct RgbColor {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

struct MatrixPatternDefinition {
  const char *id;
  const char *label;
  bool animated;
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

const MatrixPatternDefinition kMatrixPatterns[] = {
  {"off", "Off", false},
  {"solid", "Solid Glow", false},
  {"rainbow", "Rainbow Sweep", true},
  {"scanner", "Scanner", true},
  {"chase", "Theater Chase", true},
  {"pulse", "Color Pulse", true},
  {"checker", "Checker Flip", true},
  {"sparkle", "Sparkle", true},
  {"plasma", "Plasma Drift", true},
  {"prism", "Prism Stripes", true},
  {"ripple", "Neon Ripple", true},
  {"meteor", "Meteor Trail", true},
  {"pinwheel", "Pinwheel", true},
  {"aurora", "Aurora Veil", true},
  {"confetti", "Confetti Burst", true},
  {"static", "Rainbow Static", true},
  {"glitch", "Glitch Blocks", true},
  {"lava", "Lava Lamp", true},
  {"spiral", "Spiral Runner", true},
  {"pong", "Pong Rally", true},
  {"marquee", "Border March", true},
  {"equalizer", "Equalizer", true},
  {"radar", "Radar Sweep", true},
  {"orbit", "Dual Orbit", true},
  {"wavefront", "Wavefront", true},
  {"cross", "Cross Pulse", true},
  {"helix", "Helix Bands", true},
  {"tiles", "Tile Shift", true},
};

WebServer webServer(kHttpPort);
Preferences preferences;
BLEScan *bleScan = nullptr;
SemaphoreHandle_t bleScanMutex = nullptr;
Adafruit_NeoPixel matrixStrip(kMatrixLedCount, kMatrixPin, NEO_GRB + NEO_KHZ800);
PatternStep morseSteps[kMaxMorseSteps];
PatternDefinition morsePattern = {kMorsePatternId, kMorsePatternLabel, PatternMode::kSequence, morseSteps, 0, 0};
BleScanResultEntry bleResults[kMaxBleDevices];
RgbColor matrixFrame[kMatrixLedCount];

const PatternDefinition *activePattern = &kPatterns[0];
const MatrixPatternDefinition *activeMatrixPattern = &kMatrixPatterns[2];
String morseText;
String matrixColorHex = String(kMatrixDefaultColorHex);
String matrixError;
uint8_t matrixBrightness = kMatrixDefaultBrightness;
size_t activeStepIndex = 0;
size_t bleResultCount = 0;
uint32_t activeStepStartedAtMs = 0;
uint32_t lastWifiAttemptMs = 0;
uint32_t bleScanStartedAtMs = 0;
uint32_t bleScanCompletedAtMs = 0;
uint32_t lastMatrixFrameAtMs = 0;
wl_status_t lastWifiStatus = WL_IDLE_STATUS;
bool wifiWarningPrinted = false;
bool webServerStarted = false;
bool mdnsStarted = false;
bool bleReady = false;
bool bleScanInProgress = false;
bool matrixReady = false;
bool matrixFrameDirty = true;
String bleScanError;
RgbColor matrixBaseColor = {0x22, 0xc5, 0x5e};

bool hasWifiCredentials() {
  return kWifiSsid[0] != '\0' && kWifiPassword[0] != '\0';
}

uint8_t gammaCorrect(float normalizedBrightness) {
  normalizedBrightness = constrain(normalizedBrightness, 0.0f, 1.0f);
  const float corrected = powf(normalizedBrightness, 2.2f);
  return static_cast<uint8_t>(corrected * kPwmMax + 0.5f);
}

RgbColor makeColor(uint8_t red, uint8_t green, uint8_t blue) {
  return {red, green, blue};
}

RgbColor scaleColor(const RgbColor &color, uint8_t scale) {
  return {
    static_cast<uint8_t>((static_cast<uint16_t>(color.red) * scale) / 255),
    static_cast<uint8_t>((static_cast<uint16_t>(color.green) * scale) / 255),
    static_cast<uint8_t>((static_cast<uint16_t>(color.blue) * scale) / 255),
  };
}

RgbColor blendColors(const RgbColor &first, const RgbColor &second, uint8_t secondWeight) {
  const uint16_t firstWeight = 255 - secondWeight;
  return {
    static_cast<uint8_t>((static_cast<uint16_t>(first.red) * firstWeight + static_cast<uint16_t>(second.red) * secondWeight) / 255),
    static_cast<uint8_t>((static_cast<uint16_t>(first.green) * firstWeight + static_cast<uint16_t>(second.green) * secondWeight) / 255),
    static_cast<uint8_t>((static_cast<uint16_t>(first.blue) * firstWeight + static_cast<uint16_t>(second.blue) * secondWeight) / 255),
  };
}

RgbColor colorWheel(uint8_t position) {
  position = 255 - position;
  if (position < 85) {
    return makeColor(255 - position * 3, 0, position * 3);
  }
  if (position < 170) {
    position -= 85;
    return makeColor(0, position * 3, 255 - position * 3);
  }

  position -= 170;
  return makeColor(position * 3, 255 - position * 3, 0);
}

uint32_t hash32(uint32_t value) {
  value ^= value >> 16;
  value *= 0x7feb352dUL;
  value ^= value >> 15;
  value *= 0x846ca68bUL;
  value ^= value >> 16;
  return value;
}

uint8_t sine8(float radians) {
  const float normalized = 0.5f + (0.5f * sinf(radians));
  return static_cast<uint8_t>(normalized * 255.0f + 0.5f);
}

float triangleUnit(float progress) {
  progress -= floorf(progress);
  return progress < 0.5f ? (progress * 2.0f) : ((1.0f - progress) * 2.0f);
}

size_t matrixPixelIndex(uint8_t row, uint8_t column) {
  const size_t baseIndex = static_cast<size_t>(row) * kMatrixColumns;
  if ((row & 1U) == 0) {
    return baseIndex + column;
  }

  return baseIndex + (kMatrixColumns - 1 - column);
}

void clearMatrixFrame() {
  for (size_t index = 0; index < kMatrixLedCount; ++index) {
    matrixFrame[index] = makeColor(0, 0, 0);
  }
}

void fillMatrixFrame(const RgbColor &color) {
  for (size_t index = 0; index < kMatrixLedCount; ++index) {
    matrixFrame[index] = color;
  }
}

void setMatrixPixel(uint8_t row, uint8_t column, const RgbColor &color) {
  if (row >= kMatrixRows || column >= kMatrixColumns) {
    return;
  }

  matrixFrame[matrixPixelIndex(row, column)] = color;
}

void setMatrixLinearPixel(size_t index, const RgbColor &color) {
  if (index >= kMatrixLedCount) {
    return;
  }

  matrixFrame[index] = color;
}

RgbColor warmColor(uint8_t heat) {
  const RgbColor deepRed = makeColor(96, 0, 0);
  const RgbColor orange = makeColor(255, 72, 0);
  const RgbColor yellow = makeColor(255, 210, 24);
  if (heat < 128) {
    return blendColors(deepRed, orange, static_cast<uint8_t>(heat * 2));
  }

  return blendColors(orange, yellow, static_cast<uint8_t>((heat - 128) * 2));
}

void spiralStepToCoord(size_t step, uint8_t &row, uint8_t &column) {
  step %= kMatrixLedCount;

  int top = 0;
  int bottom = kMatrixRows - 1;
  int left = 0;
  int right = kMatrixColumns - 1;
  size_t index = 0;

  while (left <= right && top <= bottom) {
    for (int currentColumn = left; currentColumn <= right; ++currentColumn) {
      if (index == step) {
        row = static_cast<uint8_t>(top);
        column = static_cast<uint8_t>(currentColumn);
        return;
      }
      ++index;
    }
    ++top;

    for (int currentRow = top; currentRow <= bottom; ++currentRow) {
      if (index == step) {
        row = static_cast<uint8_t>(currentRow);
        column = static_cast<uint8_t>(right);
        return;
      }
      ++index;
    }
    --right;

    if (top <= bottom) {
      for (int currentColumn = right; currentColumn >= left; --currentColumn) {
        if (index == step) {
          row = static_cast<uint8_t>(bottom);
          column = static_cast<uint8_t>(currentColumn);
          return;
        }
        ++index;
      }
      --bottom;
    }

    if (left <= right) {
      for (int currentRow = bottom; currentRow >= top; --currentRow) {
        if (index == step) {
          row = static_cast<uint8_t>(currentRow);
          column = static_cast<uint8_t>(left);
          return;
        }
        ++index;
      }
      ++left;
    }
  }

  row = 0;
  column = 0;
}

size_t borderPixelCount() {
  return (kMatrixColumns * 2U) + (kMatrixRows * 2U) - 4U;
}

void borderStepToCoord(size_t step, uint8_t &row, uint8_t &column) {
  const size_t perimeter = borderPixelCount();
  step %= perimeter;

  if (step < kMatrixColumns) {
    row = 0;
    column = static_cast<uint8_t>(step);
    return;
  }
  step -= kMatrixColumns;

  if (step < (kMatrixRows - 1)) {
    row = static_cast<uint8_t>(step + 1);
    column = kMatrixColumns - 1;
    return;
  }
  step -= (kMatrixRows - 1);

  if (step < (kMatrixColumns - 1)) {
    row = kMatrixRows - 1;
    column = static_cast<uint8_t>(kMatrixColumns - 2 - step);
    return;
  }
  step -= (kMatrixColumns - 1);

  row = static_cast<uint8_t>(kMatrixRows - 2 - step);
  column = 0;
}

void writeMatrixFrame() {
  if (!matrixReady) {
    return;
  }

  for (size_t pixelIndex = 0; pixelIndex < kMatrixLedCount; ++pixelIndex) {
    const RgbColor scaled = scaleColor(matrixFrame[pixelIndex], matrixBrightness);
    matrixStrip.setPixelColor(pixelIndex, matrixStrip.Color(scaled.red, scaled.green, scaled.blue));
  }

  matrixStrip.show();
  matrixError = String("");
}

const MatrixPatternDefinition *findMatrixPatternById(const String &patternId) {
  for (const MatrixPatternDefinition &pattern : kMatrixPatterns) {
    if (patternId == pattern.id) {
      return &pattern;
    }
  }

  return nullptr;
}

int hexNibble(char character) {
  if (character >= '0' && character <= '9') {
    return character - '0';
  }
  if (character >= 'a' && character <= 'f') {
    return character - 'a' + 10;
  }
  if (character >= 'A' && character <= 'F') {
    return character - 'A' + 10;
  }

  return -1;
}

String colorToHex(const RgbColor &color) {
  char buffer[8];
  snprintf(buffer, sizeof(buffer), "#%02x%02x%02x", color.red, color.green, color.blue);
  return String(buffer);
}

bool parseHexColor(const String &rawValue, RgbColor &parsedColor, String &normalizedHex) {
  String value = rawValue;
  if (value.startsWith("#")) {
    value.remove(0, 1);
  }

  if (value.length() != 6) {
    return false;
  }

  uint8_t components[3] = {0, 0, 0};
  for (uint8_t componentIndex = 0; componentIndex < 3; ++componentIndex) {
    const int highNibble = hexNibble(value.charAt(componentIndex * 2));
    const int lowNibble = hexNibble(value.charAt(componentIndex * 2 + 1));
    if (highNibble < 0 || lowNibble < 0) {
      return false;
    }

    components[componentIndex] = static_cast<uint8_t>((highNibble << 4) | lowNibble);
  }

  parsedColor = makeColor(components[0], components[1], components[2]);
  normalizedHex = colorToHex(parsedColor);
  return true;
}

String formatMacAddress(uint64_t mac) {
  char buffer[18];
  snprintf(
    buffer, sizeof(buffer), "%02X:%02X:%02X:%02X:%02X:%02X", static_cast<unsigned int>((mac >> 40) & 0xFF), static_cast<unsigned int>((mac >> 32) & 0xFF),
    static_cast<unsigned int>((mac >> 24) & 0xFF), static_cast<unsigned int>((mac >> 16) & 0xFF), static_cast<unsigned int>((mac >> 8) & 0xFF),
    static_cast<unsigned int>(mac & 0xFF)
  );
  return String(buffer);
}

String floatToJson(float value, uint8_t decimals = 1) {
  if (isnan(value) || isinf(value)) {
    return F("null");
  }

  return String(static_cast<double>(value), static_cast<unsigned int>(decimals));
}

const char *bleAddressTypeLabel(uint8_t addressType) {
  switch (addressType) {
    case 0:
      return "public";
    case 1:
      return "random";
    case 2:
      return "public-id";
    case 3:
      return "random-id";
    default:
      return "unknown";
  }
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

void clearBleScanResultsLocked() {
  for (size_t index = 0; index < kMaxBleDevices; ++index) {
    bleResults[index].address = String("");
    bleResults[index].name = String("");
    bleResults[index].serviceUuid = String("");
    bleResults[index].rssi = 0;
    bleResults[index].hasTxPower = false;
    bleResults[index].txPower = 0;
    bleResults[index].connectable = false;
    bleResults[index].scannable = false;
    bleResults[index].legacy = false;
    bleResults[index].addressType = 0;
  }
  bleResultCount = 0;
}

void setBleScanStatus(bool scanning, const String &errorMessage) {
  if (bleScanMutex == nullptr) {
    return;
  }

  if (xSemaphoreTake(bleScanMutex, pdMS_TO_TICKS(250)) == pdTRUE) {
    bleScanInProgress = scanning;
    bleScanError = errorMessage;
    if (scanning) {
      bleScanStartedAtMs = millis();
      clearBleScanResultsLocked();
    } else {
      bleScanCompletedAtMs = millis();
    }
    xSemaphoreGive(bleScanMutex);
  }
}

void handleBleScanComplete(BLEScanResults scanResults) {
  if (bleScanMutex != nullptr && xSemaphoreTake(bleScanMutex, portMAX_DELAY) == pdTRUE) {
    clearBleScanResultsLocked();

    const int totalResults = scanResults.getCount();
    const size_t cappedResults = totalResults > static_cast<int>(kMaxBleDevices) ? kMaxBleDevices : static_cast<size_t>(totalResults);
    for (size_t index = 0; index < cappedResults; ++index) {
      BLEAdvertisedDevice device = scanResults.getDevice(index);
      BleScanResultEntry &entry = bleResults[index];
      entry.address = device.getAddress().toString();
      entry.name = device.haveName() ? device.getName() : String("");
      entry.serviceUuid = device.haveServiceUUID() ? device.getServiceUUID().toString() : String("");
      entry.rssi = device.getRSSI();
      entry.hasTxPower = device.haveTXPower();
      entry.txPower = device.haveTXPower() ? device.getTXPower() : 0;
      entry.connectable = device.isConnectable();
      entry.scannable = device.isScannable();
      entry.legacy = device.isLegacyAdvertisement();
      entry.addressType = device.getAddressType();
      ++bleResultCount;
    }

    bleScanInProgress = false;
    bleScanCompletedAtMs = millis();
    bleScanError = String("");
    xSemaphoreGive(bleScanMutex);
  }

  if (bleScan != nullptr) {
    bleScan->clearResults();
  }
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

void persistMatrixState() {
  preferences.putString(kMatrixPatternPreferenceKey, activeMatrixPattern->id);
  preferences.putUChar(kMatrixBrightnessPreferenceKey, matrixBrightness);
  preferences.putString(kMatrixColorPreferenceKey, matrixColorHex);
}

bool selectMatrixPatternById(const String &patternId, bool persistSelection) {
  const MatrixPatternDefinition *pattern = findMatrixPatternById(patternId);
  if (pattern == nullptr) {
    return false;
  }

  activeMatrixPattern = pattern;
  matrixFrameDirty = true;
  lastMatrixFrameAtMs = 0;

  if (persistSelection) {
    persistMatrixState();
  }

  return true;
}

bool updateMatrixColor(const String &colorHex, bool persistSelection) {
  RgbColor parsedColor;
  String normalizedHex;
  if (!parseHexColor(colorHex, parsedColor, normalizedHex)) {
    return false;
  }

  matrixBaseColor = parsedColor;
  matrixColorHex = normalizedHex;
  matrixFrameDirty = true;
  lastMatrixFrameAtMs = 0;

  if (persistSelection) {
    persistMatrixState();
  }

  return true;
}

bool updateMatrixBrightness(int brightness, bool persistSelection) {
  if (brightness < 0 || brightness > 255) {
    return false;
  }

  matrixBrightness = static_cast<uint8_t>(brightness);
  matrixFrameDirty = true;
  lastMatrixFrameAtMs = 0;

  if (persistSelection) {
    persistMatrixState();
  }

  return true;
}

void loadSavedMatrixState() {
  updateMatrixColor(preferences.getString(kMatrixColorPreferenceKey, kMatrixDefaultColorHex), false);

  const uint8_t storedBrightness = preferences.getUChar(kMatrixBrightnessPreferenceKey, kMatrixDefaultBrightness);
  if (!updateMatrixBrightness(storedBrightness, false)) {
    updateMatrixBrightness(kMatrixDefaultBrightness, false);
  }

  const String storedPattern = preferences.getString(kMatrixPatternPreferenceKey, kMatrixPatterns[2].id);
  if (!selectMatrixPatternById(storedPattern, false)) {
    selectMatrixPatternById(kMatrixPatterns[2].id, false);
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

RgbColor matrixPreviewColorAt(uint8_t row, uint8_t column) {
  return scaleColor(matrixFrame[matrixPixelIndex(row, column)], matrixBrightness);
}

void renderMatrixOff() {
  clearMatrixFrame();
}

void renderMatrixSolid() {
  fillMatrixFrame(matrixBaseColor);
}

void renderMatrixRainbow(uint32_t now) {
  const uint8_t phase = static_cast<uint8_t>((now / 16U) & 0xFFU);
  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const uint8_t offset = static_cast<uint8_t>(phase + row * 18U + column * 10U);
      setMatrixPixel(row, column, colorWheel(offset));
    }
  }
}

void renderMatrixScanner(uint32_t now) {
  clearMatrixFrame();

  const uint8_t travel = (kMatrixColumns - 1) * 2;
  uint8_t column = static_cast<uint8_t>((now / 80U) % travel);
  if (column >= kMatrixColumns) {
    column = static_cast<uint8_t>(travel - column);
  }

  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t currentColumn = 0; currentColumn < kMatrixColumns; ++currentColumn) {
      const int distance = abs(static_cast<int>(currentColumn) - static_cast<int>(column));
      if (distance == 0) {
        setMatrixPixel(row, currentColumn, matrixBaseColor);
      } else if (distance == 1) {
        setMatrixPixel(row, currentColumn, scaleColor(matrixBaseColor, 104));
      } else if (distance == 2) {
        setMatrixPixel(row, currentColumn, scaleColor(matrixBaseColor, 36));
      }
    }
  }
}

void renderMatrixChase(uint32_t now) {
  const uint8_t phase = static_cast<uint8_t>((now / 140U) % 3U);
  const RgbColor dim = scaleColor(matrixBaseColor, 18);

  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const bool lit = ((row + column + phase) % 3U) == 0;
      setMatrixPixel(row, column, lit ? matrixBaseColor : dim);
    }
  }
}

void renderMatrixPulse(uint32_t now) {
  const float progress = static_cast<float>(now % 2200U) / 2200.0f;
  const float normalized = 0.12f + 0.88f * (0.5f - (0.5f * cosf(2.0f * PI * progress)));
  fillMatrixFrame(scaleColor(matrixBaseColor, static_cast<uint8_t>(normalized * 255.0f + 0.5f)));
}

void renderMatrixChecker(uint32_t now) {
  const uint8_t phase = static_cast<uint8_t>((now / 420U) & 1U);
  const RgbColor accent = scaleColor(matrixBaseColor, 36);

  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const bool lit = ((row + column + phase) & 1U) == 0;
      setMatrixPixel(row, column, lit ? matrixBaseColor : accent);
    }
  }
}

void renderMatrixSparkle(uint32_t now) {
  const uint32_t tick = now / 110U;
  const RgbColor background = scaleColor(matrixBaseColor, 22);
  const RgbColor medium = scaleColor(matrixBaseColor, 120);

  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const uint32_t noise = hash32(tick * 131U + row * 17U + column * 29U);
      const uint8_t selector = static_cast<uint8_t>(noise & 0xFFU);
      if (selector > 244) {
        setMatrixPixel(row, column, makeColor(255, 255, 255));
      } else if (selector > 220) {
        setMatrixPixel(row, column, medium);
      } else {
        setMatrixPixel(row, column, background);
      }
    }
  }
}

void renderMatrixPlasma(uint32_t now) {
  const float phase = static_cast<float>(now) / 340.0f;
  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const float wave = sinf(column * 0.85f + phase) + sinf(row * 1.10f - phase * 1.25f) + sinf((column + row) * 0.55f + phase * 0.70f);
      const float normalized = (wave + 3.0f) / 6.0f;
      const uint8_t hue = static_cast<uint8_t>(normalized * 255.0f + row * 13U + column * 7U);
      setMatrixPixel(row, column, blendColors(scaleColor(matrixBaseColor, 48), colorWheel(hue), 196));
    }
  }
}

void renderMatrixPrism(uint32_t now) {
  const float phase = static_cast<float>(now) / 260.0f;
  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const uint8_t stripe = sine8(column * 1.05f - row * 0.70f + phase);
      const uint8_t hue = static_cast<uint8_t>((now / 20U) + column * 26U + row * 18U);
      setMatrixPixel(row, column, blendColors(scaleColor(matrixBaseColor, 26), colorWheel(hue), stripe));
    }
  }
}

void renderMatrixRipple(uint32_t now) {
  const float centerX = (kMatrixColumns - 1) * 0.5f;
  const float centerY = (kMatrixRows - 1) * 0.5f;
  const float phase = static_cast<float>(now) / 150.0f;

  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const float deltaX = column - centerX;
      const float deltaY = row - centerY;
      const float distance = sqrtf(deltaX * deltaX + deltaY * deltaY);
      const uint8_t wave = sine8(distance * 2.8f - phase);
      const uint8_t hue = static_cast<uint8_t>((now / 14U) + distance * 34.0f);
      setMatrixPixel(row, column, blendColors(scaleColor(matrixBaseColor, 14), colorWheel(hue), wave));
    }
  }
}

void renderMatrixMeteor(uint32_t now) {
  fillMatrixFrame(scaleColor(matrixBaseColor, 8));

  const size_t headBase = static_cast<size_t>(now / 55U);
  for (uint8_t meteorIndex = 0; meteorIndex < 3; ++meteorIndex) {
    const size_t head = (headBase + meteorIndex * (kMatrixLedCount / 3)) % kMatrixLedCount;
    const RgbColor meteorColor = blendColors(matrixBaseColor, colorWheel(static_cast<uint8_t>(head * 5U + meteorIndex * 60U)), 132);

    for (uint8_t tail = 0; tail < 12; ++tail) {
      const size_t pixelIndex = (head + kMatrixLedCount - tail) % kMatrixLedCount;
      const uint8_t intensity = static_cast<uint8_t>(255 - tail * 20U);
      setMatrixLinearPixel(pixelIndex, scaleColor(meteorColor, intensity));
    }
  }
}

void renderMatrixPinwheel(uint32_t now) {
  const float centerX = (kMatrixColumns - 1) * 0.5f;
  const float centerY = (kMatrixRows - 1) * 0.5f;
  const float phase = static_cast<float>(now) / 420.0f;

  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const float deltaX = column - centerX;
      const float deltaY = row - centerY;
      const float angle = atan2f(deltaY, deltaX);
      const float radius = sqrtf(deltaX * deltaX + deltaY * deltaY);
      const uint8_t hue = static_cast<uint8_t>(((angle + PI) / (2.0f * PI)) * 255.0f + (now / 10U) + radius * 20.0f);
      const uint8_t pulse = static_cast<uint8_t>(96 + (sine8(radius * 2.2f - phase * 3.0f) / 2U));
      setMatrixPixel(row, column, scaleColor(colorWheel(hue), pulse));
    }
  }
}

void renderMatrixAurora(uint32_t now) {
  const float phase = static_cast<float>(now) / 520.0f;

  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const float curtain = 0.5f + (0.5f * sinf(column * 0.45f + phase + sinf(row * 0.55f + phase)));
      const float sweep = 0.5f + (0.5f * sinf(row * 0.90f - phase * 1.80f));
      const uint8_t hue = static_cast<uint8_t>((now / 22U) + column * 17U + row * 11U);
      const uint8_t colorMix = static_cast<uint8_t>(80 + curtain * 120.0f);
      const uint8_t intensity = static_cast<uint8_t>(28 + curtain * 110.0f + sweep * 80.0f);
      setMatrixPixel(row, column, scaleColor(blendColors(matrixBaseColor, colorWheel(hue), colorMix), intensity));
    }
  }
}

void renderMatrixConfetti(uint32_t now) {
  const uint32_t tick = now / 85U;
  const RgbColor background = scaleColor(matrixBaseColor, 10);

  for (size_t index = 0; index < kMatrixLedCount; ++index) {
    const uint32_t noise = hash32(tick * 313U + index * 101U);
    const uint8_t selector = static_cast<uint8_t>(noise & 0xFFU);
    if (selector > 242) {
      setMatrixLinearPixel(index, colorWheel(static_cast<uint8_t>((noise >> 8) & 0xFFU)));
    } else if (selector > 232) {
      setMatrixLinearPixel(index, makeColor(255, 255, 255));
    } else {
      setMatrixLinearPixel(index, background);
    }
  }
}

void renderMatrixStatic(uint32_t now) {
  const uint32_t tick = now / 45U;

  for (size_t index = 0; index < kMatrixLedCount; ++index) {
    const uint32_t noise = hash32(tick * 911U + index * 137U);
    const uint8_t hue = static_cast<uint8_t>(noise & 0xFFU);
    const uint8_t intensity = static_cast<uint8_t>(40 + ((noise >> 8) & 0xD7U));
    setMatrixLinearPixel(index, scaleColor(colorWheel(hue), intensity));
  }
}

void renderMatrixGlitch(uint32_t now) {
  const uint32_t tick = now / 120U;

  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    const uint32_t rowNoise = hash32(tick * 173U + row * 971U);
    const uint8_t rowShift = static_cast<uint8_t>(rowNoise % kMatrixColumns);

    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const uint32_t blockNoise = hash32(rowNoise + ((column + rowShift) / 2U) * 37U);
      const uint8_t selector = static_cast<uint8_t>(blockNoise & 0x0FU);
      if (selector == 0) {
        setMatrixPixel(row, column, makeColor(255, 255, 255));
      } else if (selector <= 2) {
        setMatrixPixel(row, column, makeColor(0, 0, 0));
      } else {
        const uint8_t hue = static_cast<uint8_t>((blockNoise >> 8) & 0xFFU);
        const uint8_t intensity = static_cast<uint8_t>(120 + ((blockNoise >> 16) & 0x7FU));
        setMatrixPixel(row, column, scaleColor(colorWheel(hue), intensity));
      }
    }
  }
}

void renderMatrixLava(uint32_t now) {
  const float phase = static_cast<float>(now) / 380.0f;
  const uint32_t tick = now / 160U;

  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const float wave = sinf(column * 0.70f + phase) + sinf(row * 1.20f - phase * 1.35f) + sinf((column - row) * 0.90f + phase * 0.55f);
      const uint8_t flicker = static_cast<uint8_t>(hash32(tick * 131U + row * 17U + column * 29U) & 0x3FU);
      const int heat = static_cast<int>(((wave + 3.0f) / 6.0f) * 200.0f) + flicker;
      setMatrixPixel(row, column, warmColor(constrain(heat, 0, 255)));
    }
  }
}

void renderMatrixSpiral(uint32_t now) {
  fillMatrixFrame(scaleColor(matrixBaseColor, 10));

  const size_t head = static_cast<size_t>(now / 75U) % kMatrixLedCount;
  for (uint8_t tail = 0; tail < 18; ++tail) {
    const size_t step = (head + kMatrixLedCount - tail) % kMatrixLedCount;
    uint8_t row = 0;
    uint8_t column = 0;
    spiralStepToCoord(step, row, column);
    const uint8_t hue = static_cast<uint8_t>((step * 17U) + (now / 18U));
    const RgbColor color = blendColors(matrixBaseColor, colorWheel(hue), 116);
    setMatrixPixel(row, column, scaleColor(color, static_cast<uint8_t>(255 - tail * 12U)));
  }
}

void renderMatrixPong(uint32_t now) {
  clearMatrixFrame();

  const float xProgress = triangleUnit(static_cast<float>(now % 2600U) / 2600.0f);
  const float yProgress = triangleUnit(static_cast<float>((now + 600U) % 1700U) / 1700.0f);
  const uint8_t ballColumn = static_cast<uint8_t>(xProgress * (kMatrixColumns - 1) + 0.5f);
  const uint8_t ballRow = static_cast<uint8_t>(yProgress * (kMatrixRows - 1) + 0.5f);

  const uint8_t paddleLeftCenter = static_cast<uint8_t>((0.5f + 0.5f * sinf(now / 360.0f)) * (kMatrixRows - 1) + 0.5f);
  const uint8_t paddleRightCenter = static_cast<uint8_t>((0.5f + 0.5f * sinf(now / 420.0f + 1.7f)) * (kMatrixRows - 1) + 0.5f);

  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    setMatrixPixel(row, kMatrixColumns / 2, scaleColor(matrixBaseColor, row == ballRow ? 50 : 16));
  }

  for (int offset = -1; offset <= 1; ++offset) {
    const int leftRow = static_cast<int>(paddleLeftCenter) + offset;
    const int rightRow = static_cast<int>(paddleRightCenter) + offset;
    if (leftRow >= 0 && leftRow < kMatrixRows) {
      setMatrixPixel(static_cast<uint8_t>(leftRow), 0, matrixBaseColor);
    }
    if (rightRow >= 0 && rightRow < kMatrixRows) {
      setMatrixPixel(static_cast<uint8_t>(rightRow), kMatrixColumns - 1, blendColors(matrixBaseColor, colorWheel(150), 90));
    }
  }

  setMatrixPixel(ballRow, ballColumn, makeColor(255, 255, 255));
  if (ballColumn > 0) {
    setMatrixPixel(ballRow, ballColumn - 1, scaleColor(makeColor(255, 255, 255), 80));
  }
}

void renderMatrixMarquee(uint32_t now) {
  fillMatrixFrame(scaleColor(matrixBaseColor, 8));

  const size_t perimeter = borderPixelCount();
  const size_t head = static_cast<size_t>(now / 95U) % perimeter;
  for (uint8_t segment = 0; segment < 10; ++segment) {
    uint8_t row = 0;
    uint8_t column = 0;
    borderStepToCoord((head + perimeter - segment) % perimeter, row, column);
    const uint8_t hue = static_cast<uint8_t>((segment * 24U) + (now / 22U));
    const RgbColor color = blendColors(matrixBaseColor, colorWheel(hue), 138);
    setMatrixPixel(row, column, scaleColor(color, static_cast<uint8_t>(255 - segment * 18U)));
  }
}

void renderMatrixEqualizer(uint32_t now) {
  clearMatrixFrame();

  for (uint8_t column = 0; column < kMatrixColumns; ++column) {
    const float phase = (now / 260.0f) + column * 0.55f;
    const uint8_t barHeight = static_cast<uint8_t>(1 + (0.5f + 0.5f * sinf(phase)) * (kMatrixRows - 1));
    const RgbColor barColor = blendColors(matrixBaseColor, colorWheel(static_cast<uint8_t>(column * 24U + now / 16U)), 120);

    for (uint8_t height = 0; height < kMatrixRows; ++height) {
      const uint8_t row = static_cast<uint8_t>(kMatrixRows - 1 - height);
      if (height < barHeight) {
        const uint8_t intensity = static_cast<uint8_t>(110 + ((height * 145U) / kMatrixRows));
        setMatrixPixel(row, column, scaleColor(barColor, intensity));
      } else {
        setMatrixPixel(row, column, scaleColor(matrixBaseColor, 10));
      }
    }
  }
}

void renderMatrixRadar(uint32_t now) {
  clearMatrixFrame();

  const float centerX = (kMatrixColumns - 1) * 0.5f;
  const float centerY = (kMatrixRows - 1) * 0.5f;
  const float sweep = fmodf(static_cast<float>(now) / 420.0f, 2.0f * PI);

  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const float deltaX = column - centerX;
      const float deltaY = row - centerY;
      const float angle = atan2f(deltaY, deltaX);
      float angleDelta = fabsf(angle - sweep);
      if (angleDelta > PI) {
        angleDelta = (2.0f * PI) - angleDelta;
      }

      const float distance = sqrtf(deltaX * deltaX + deltaY * deltaY);
      const uint8_t ring = sine8(distance * 3.0f - now / 190.0f);
      const uint8_t sweepStrength = angleDelta < 0.5f ? static_cast<uint8_t>((1.0f - (angleDelta / 0.5f)) * 255.0f) : 0;
      const uint8_t intensity = max<uint8_t>(ring / 6U, sweepStrength);
      setMatrixPixel(row, column, scaleColor(matrixBaseColor, intensity));
    }
  }
}

void renderMatrixOrbit(uint32_t now) {
  fillMatrixFrame(scaleColor(matrixBaseColor, 8));

  const float centerX = (kMatrixColumns - 1) * 0.5f;
  const float centerY = (kMatrixRows - 1) * 0.5f;
  const float orbitX = 4.0f;
  const float orbitY = 2.1f;
  const float angle = static_cast<float>(now) / 360.0f;

  for (uint8_t trail = 0; trail < 12; ++trail) {
    const float trailAngle = angle - trail * 0.26f;
    const uint8_t intensity = static_cast<uint8_t>(255 - trail * 18U);
    const int firstColumn = lroundf(centerX + cosf(trailAngle) * orbitX);
    const int firstRow = lroundf(centerY + sinf(trailAngle) * orbitY);
    const int secondColumn = lroundf(centerX + cosf(trailAngle + PI) * orbitX);
    const int secondRow = lroundf(centerY + sinf(trailAngle + PI) * orbitY);

    if (firstRow >= 0 && firstRow < kMatrixRows && firstColumn >= 0 && firstColumn < kMatrixColumns) {
      setMatrixPixel(static_cast<uint8_t>(firstRow), static_cast<uint8_t>(firstColumn), scaleColor(matrixBaseColor, intensity));
    }
    if (secondRow >= 0 && secondRow < kMatrixRows && secondColumn >= 0 && secondColumn < kMatrixColumns) {
      setMatrixPixel(
        static_cast<uint8_t>(secondRow),
        static_cast<uint8_t>(secondColumn),
        scaleColor(blendColors(matrixBaseColor, colorWheel(170), 130), intensity)
      );
    }
  }
}

void renderMatrixWavefront(uint32_t now) {
  clearMatrixFrame();

  const float travel = static_cast<float>((now / 70U) % (kMatrixRows + kMatrixColumns + 8U)) - 4.0f;
  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const float distance = fabsf((row + column) - travel);
      if (distance <= 0.5f) {
        setMatrixPixel(row, column, makeColor(255, 255, 255));
      } else if (distance <= 1.5f) {
        setMatrixPixel(row, column, blendColors(matrixBaseColor, colorWheel(static_cast<uint8_t>(column * 18U + now / 18U)), 120));
      } else if (distance <= 2.5f) {
        setMatrixPixel(row, column, scaleColor(matrixBaseColor, 44));
      } else {
        setMatrixPixel(row, column, scaleColor(matrixBaseColor, 8));
      }
    }
  }
}

void renderMatrixCross(uint32_t now) {
  clearMatrixFrame();

  const float radius = triangleUnit(static_cast<float>(now % 1800U) / 1800.0f) * 4.5f;
  const float centerX = (kMatrixColumns - 1) * 0.5f;
  const float centerY = (kMatrixRows - 1) * 0.5f;
  const RgbColor accent = blendColors(matrixBaseColor, colorWheel(static_cast<uint8_t>(now / 15U)), 110);

  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const float crossDistance = min(fabsf(column - centerX), fabsf(row - centerY));
      const float radialDistance = fabsf((fabsf(column - centerX) + fabsf(row - centerY)) - radius);

      if (crossDistance < 0.55f && radialDistance < 0.8f) {
        setMatrixPixel(row, column, makeColor(255, 255, 255));
      } else if (crossDistance < 0.55f) {
        setMatrixPixel(row, column, scaleColor(accent, 160));
      } else if (radialDistance < 0.9f) {
        setMatrixPixel(row, column, scaleColor(accent, 96));
      } else {
        setMatrixPixel(row, column, scaleColor(matrixBaseColor, 8));
      }
    }
  }
}

void renderMatrixHelix(uint32_t now) {
  clearMatrixFrame();

  for (uint8_t column = 0; column < kMatrixColumns; ++column) {
    const float phase = (now / 210.0f) + column * 0.62f;
    const float topWave = (0.5f + 0.5f * sinf(phase)) * (kMatrixRows - 1);
    const float bottomWave = (0.5f + 0.5f * sinf(phase + PI)) * (kMatrixRows - 1);

    for (uint8_t row = 0; row < kMatrixRows; ++row) {
      const float topDistance = fabsf(row - topWave);
      const float bottomDistance = fabsf(row - bottomWave);
      if (topDistance < 0.55f) {
        setMatrixPixel(row, column, matrixBaseColor);
      } else if (bottomDistance < 0.55f) {
        setMatrixPixel(row, column, blendColors(matrixBaseColor, colorWheel(170), 120));
      } else if (topDistance < 1.3f || bottomDistance < 1.3f) {
        setMatrixPixel(row, column, scaleColor(matrixBaseColor, 44));
      } else {
        setMatrixPixel(row, column, scaleColor(matrixBaseColor, 8));
      }
    }
  }
}

void renderMatrixTiles(uint32_t now) {
  const uint8_t phase = static_cast<uint8_t>((now / 260U) & 0x03U);

  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const uint8_t tileX = column / 2U;
      const uint8_t tileY = row / 2U;
      const uint8_t index = static_cast<uint8_t>((tileX + tileY + phase) & 0x03U);
      switch (index) {
        case 0:
          setMatrixPixel(row, column, matrixBaseColor);
          break;
        case 1:
          setMatrixPixel(row, column, blendColors(matrixBaseColor, colorWheel(64), 128));
          break;
        case 2:
          setMatrixPixel(row, column, blendColors(matrixBaseColor, colorWheel(160), 160));
          break;
        default:
          setMatrixPixel(row, column, scaleColor(matrixBaseColor, 26));
          break;
      }
    }
  }
}

void renderMatrixFrame(uint32_t now) {
  if (activeMatrixPattern == nullptr) {
    renderMatrixOff();
    return;
  }

  if (strcmp(activeMatrixPattern->id, "off") == 0) {
    renderMatrixOff();
  } else if (strcmp(activeMatrixPattern->id, "solid") == 0) {
    renderMatrixSolid();
  } else if (strcmp(activeMatrixPattern->id, "rainbow") == 0) {
    renderMatrixRainbow(now);
  } else if (strcmp(activeMatrixPattern->id, "scanner") == 0) {
    renderMatrixScanner(now);
  } else if (strcmp(activeMatrixPattern->id, "chase") == 0) {
    renderMatrixChase(now);
  } else if (strcmp(activeMatrixPattern->id, "pulse") == 0) {
    renderMatrixPulse(now);
  } else if (strcmp(activeMatrixPattern->id, "checker") == 0) {
    renderMatrixChecker(now);
  } else if (strcmp(activeMatrixPattern->id, "sparkle") == 0) {
    renderMatrixSparkle(now);
  } else if (strcmp(activeMatrixPattern->id, "plasma") == 0) {
    renderMatrixPlasma(now);
  } else if (strcmp(activeMatrixPattern->id, "prism") == 0) {
    renderMatrixPrism(now);
  } else if (strcmp(activeMatrixPattern->id, "ripple") == 0) {
    renderMatrixRipple(now);
  } else if (strcmp(activeMatrixPattern->id, "meteor") == 0) {
    renderMatrixMeteor(now);
  } else if (strcmp(activeMatrixPattern->id, "pinwheel") == 0) {
    renderMatrixPinwheel(now);
  } else if (strcmp(activeMatrixPattern->id, "aurora") == 0) {
    renderMatrixAurora(now);
  } else if (strcmp(activeMatrixPattern->id, "confetti") == 0) {
    renderMatrixConfetti(now);
  } else if (strcmp(activeMatrixPattern->id, "static") == 0) {
    renderMatrixStatic(now);
  } else if (strcmp(activeMatrixPattern->id, "glitch") == 0) {
    renderMatrixGlitch(now);
  } else if (strcmp(activeMatrixPattern->id, "lava") == 0) {
    renderMatrixLava(now);
  } else if (strcmp(activeMatrixPattern->id, "spiral") == 0) {
    renderMatrixSpiral(now);
  } else if (strcmp(activeMatrixPattern->id, "pong") == 0) {
    renderMatrixPong(now);
  } else if (strcmp(activeMatrixPattern->id, "marquee") == 0) {
    renderMatrixMarquee(now);
  } else if (strcmp(activeMatrixPattern->id, "equalizer") == 0) {
    renderMatrixEqualizer(now);
  } else if (strcmp(activeMatrixPattern->id, "radar") == 0) {
    renderMatrixRadar(now);
  } else if (strcmp(activeMatrixPattern->id, "orbit") == 0) {
    renderMatrixOrbit(now);
  } else if (strcmp(activeMatrixPattern->id, "wavefront") == 0) {
    renderMatrixWavefront(now);
  } else if (strcmp(activeMatrixPattern->id, "cross") == 0) {
    renderMatrixCross(now);
  } else if (strcmp(activeMatrixPattern->id, "helix") == 0) {
    renderMatrixHelix(now);
  } else if (strcmp(activeMatrixPattern->id, "tiles") == 0) {
    renderMatrixTiles(now);
  } else {
    renderMatrixOff();
  }
}

void applyMatrixFrameNow() {
  renderMatrixFrame(millis());
  writeMatrixFrame();
  matrixFrameDirty = false;
  lastMatrixFrameAtMs = millis();
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

String buildMatrixJson() {
  String json;
  json.reserve(3200);

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
  json += F("\",\"patterns\":[");

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

int extractJsonIntField(const String &body, const char *fieldName) {
  const String quotedField = String('"') + fieldName + '"';
  const int fieldIndex = body.indexOf(quotedField);
  if (fieldIndex < 0) {
    return kJsonFieldMissing;
  }

  const int colonIndex = body.indexOf(':', fieldIndex + quotedField.length());
  if (colonIndex < 0) {
    return kJsonFieldMissing;
  }

  int valueStart = colonIndex + 1;
  while (valueStart < body.length() && isspace(static_cast<unsigned char>(body.charAt(valueStart)))) {
    ++valueStart;
  }

  if (valueStart >= body.length()) {
    return kJsonFieldMissing;
  }

  int valueEnd = valueStart;
  if (body.charAt(valueEnd) == '-') {
    ++valueEnd;
  }
  while (valueEnd < body.length() && isdigit(static_cast<unsigned char>(body.charAt(valueEnd)))) {
    ++valueEnd;
  }

  if (valueEnd == valueStart || (valueEnd == valueStart + 1 && body.charAt(valueStart) == '-')) {
    return kJsonFieldMissing;
  }

  return body.substring(valueStart, valueEnd).toInt();
}

void handleRoot() {
  webServer.sendHeader(F("Cache-Control"), F("no-store"));
  webServer.send_P(200, PSTR("text/html; charset=utf-8"), kWebAppHtml, strlen_P(kWebAppHtml));
}

void handleState() {
  sendJson(200, buildStateJson());
}

void handleBluetoothState() {
  sendJson(200, buildBluetoothJson());
}

void handleDeviceState() {
  sendJson(200, buildDeviceJson());
}

void handleMatrixState() {
  sendJson(200, buildMatrixJson());
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

  int nextBrightness = extractJsonIntField(body, "brightness");
  if (nextBrightness == kJsonFieldMissing && webServer.hasArg(F("brightness"))) {
    nextBrightness = webServer.arg(F("brightness")).toInt();
  }

  if (nextPatternId.isEmpty() && nextColor.isEmpty() && nextBrightness == kJsonFieldMissing) {
    sendJsonError(400, F("Request body must include patternId, color, or brightness."));
    return;
  }

  const MatrixPatternDefinition *candidatePattern = activeMatrixPattern;
  RgbColor candidateColor = matrixBaseColor;
  String candidateColorHex = matrixColorHex;
  uint8_t candidateBrightness = matrixBrightness;

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

  if (nextBrightness != kJsonFieldMissing && (nextBrightness < 0 || nextBrightness > 255)) {
    sendJsonError(400, F("Matrix brightness must be between 0 and 255."));
    return;
  }

  activeMatrixPattern = candidatePattern;
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

bool startBleScan() {
  if (!bleReady || bleScan == nullptr) {
    setBleScanStatus(false, F("Bluetooth scanner unavailable."));
    return false;
  }

  if (bleScan->isScanning()) {
    return true;
  }

  setBleScanStatus(true, String(""));
  bleScan->clearResults();

  if (!bleScan->start(kBleScanDurationSeconds, handleBleScanComplete, false)) {
    setBleScanStatus(false, F("Failed to start Bluetooth scan."));
    return false;
  }

  return true;
}

void handleBluetoothScan() {
  if (!startBleScan()) {
    sendJsonError(500, bleScanError.isEmpty() ? String(F("Unable to start Bluetooth scan.")) : bleScanError);
    return;
  }

  sendJson(200, buildBluetoothJson());
}

void handleNotFound() {
  sendJsonError(404, F("Not found."));
}

void configureHttpServer() {
  webServer.on(F("/"), HTTP_GET, handleRoot);
  webServer.on(F("/api/state"), HTTP_GET, handleState);
  webServer.on(F("/api/pattern"), HTTP_POST, handlePatternChange);
  webServer.on(F("/api/morse"), HTTP_POST, handleMorseChange);
  webServer.on(F("/api/bluetooth"), HTTP_GET, handleBluetoothState);
  webServer.on(F("/api/bluetooth/scan"), HTTP_POST, handleBluetoothScan);
  webServer.on(F("/api/device"), HTTP_GET, handleDeviceState);
  webServer.on(F("/api/matrix"), HTTP_GET, handleMatrixState);
  webServer.on(F("/api/matrix"), HTTP_POST, handleMatrixUpdate);
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

void updateMatrixPattern() {
  if (!matrixReady || activeMatrixPattern == nullptr) {
    return;
  }

  const uint32_t now = millis();
  if (!matrixFrameDirty && !activeMatrixPattern->animated) {
    return;
  }

  if (!matrixFrameDirty && now - lastMatrixFrameAtMs < kMatrixFrameIntervalMs) {
    return;
  }

  renderMatrixFrame(now);
  writeMatrixFrame();
  matrixFrameDirty = false;
  lastMatrixFrameAtMs = now;
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

void initializeBluetoothScanner() {
  bleScanMutex = xSemaphoreCreateMutex();
  if (bleScanMutex == nullptr) {
    bleScanError = F("Failed to allocate Bluetooth scan mutex.");
    return;
  }

  if (!BLEDevice::init("XIAO Device Console")) {
    bleScanError = F("BLE init failed.");
    return;
  }

  bleScan = BLEDevice::getScan();
  if (bleScan == nullptr) {
    bleScanError = F("BLE scanner unavailable.");
    return;
  }

  bleScan->setActiveScan(true);
  bleScan->setInterval(120);
  bleScan->setWindow(80);
#if defined(CONFIG_NIMBLE_ENABLED)
  bleScan->setDuplicateFilter(true);
#endif
  bleReady = true;
  bleScanError = String("");
}

void initializeMatrix() {
  clearMatrixFrame();
  matrixStrip.begin();
  matrixStrip.clear();
  matrixReady = true;
  matrixError = String("");
  applyMatrixFrameNow();
}

}  // namespace

void setup() {
  Serial.begin(115200);
  delay(250);

  pinMode(kLedPin, OUTPUT);
  setLedBrightness(0);
  enableBoardWifiHardware();
  initializeBluetoothScanner();

  preferences.begin(kPreferenceNamespace, false);
  loadSavedPattern();
  loadSavedMatrixState();
  initializeMatrix();

  configureHttpServer();
  beginWifiConnection();
}

void loop() {
  updateLedPattern();
  updateMatrixPattern();
  updateWifiServices();

  if (webServerStarted) {
    webServer.handleClient();
  }
}
