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
constexpr uint8_t kMatrixDefaultAnimationSpeed = 100;
constexpr uint8_t kMatrixMinAnimationSpeed = 25;
constexpr uint8_t kMatrixMaxAnimationSpeed = 200;
constexpr char kMatrixDefaultColorHex[] = "#22c55e";
constexpr char kPreferenceNamespace[] = "xiao-app";
constexpr char kPatternPreferenceKey[] = "pattern";
constexpr char kMorseTextPreferenceKey[] = "morseText";
constexpr char kMatrixPatternPreferenceKey[] = "matrixPattern";
constexpr char kMatrixEnabledPreferenceKey[] = "matrixEnabled";
constexpr char kMatrixBrightnessPreferenceKey[] = "matrixBright";
constexpr char kMatrixAnimationSpeedPreferenceKey[] = "matrixSpeed";
constexpr char kMatrixColorPreferenceKey[] = "matrixColor";
constexpr char kMatrixMappingPreferenceKey[] = "matrixMap";
constexpr char kMatrixMoodPreferenceKey[] = "matrixMood";
constexpr char kMatrixMessagePreferenceKey[] = "matrixMessage";
constexpr char kMorsePatternId[] = "morse-text";
constexpr char kMorsePatternLabel[] = "Custom Morse";
constexpr char kMatrixSolidPatternId[] = "solid";
constexpr char kMatrixMoodPatternId[] = "mood";
constexpr char kMatrixMessagePatternId[] = "message";
constexpr char kDefaultMatrixPatternId[] = "rainbow";
constexpr char kDefaultMatrixMappingId[] = "cols-bl";
constexpr char kDefaultMatrixMoodId[] = "happy";
constexpr char kDefaultMatrixMessage[] = "HELLO";
constexpr int kJsonFieldMissing = -1000;
constexpr int kJsonBoolFieldMissing = -1;
constexpr size_t kMaxMatrixMessageLength = 64;
constexpr uint8_t kMatrixGlyphWidth = 3;
constexpr uint8_t kMatrixWideGlyphWidth = 5;
constexpr uint8_t kMatrixGlyphSpacing = 1;
constexpr uint16_t kMatrixMessageScrollMs = 110;

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
  bool showInMatrixApp;
};

struct MatrixMappingDefinition {
  const char *id;
  const char *label;
  bool columnMajor;
  bool flipX;
  bool flipY;
};

struct MoodDefinition {
  const char *id;
  const char *label;
  const char *rows[kMatrixRows];
};

struct MatrixGlyph {
  char character;
  uint8_t rows[kMatrixRows];
};

#include "catalogs/led_patterns_catalog.h"
#include "catalogs/matrix_catalog.h"

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
const MatrixPatternDefinition *activeMatrixPattern = &kMatrixPatterns[1];
const MatrixMappingDefinition *activeMatrixMapping = &kMatrixMappings[2];
String morseText;
String matrixColorHex = String(kMatrixDefaultColorHex);
String matrixMoodId = String(kDefaultMatrixMoodId);
String matrixMessageText = String(kDefaultMatrixMessage);
String matrixError;
bool matrixEnabled = true;
uint8_t matrixBrightness = kMatrixDefaultBrightness;
uint8_t matrixAnimationSpeed = kMatrixDefaultAnimationSpeed;
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

void applyMatrixFrameNow();

#include "services/platform_utils.h"
#include "services/led_patterns_service.h"
#include "services/bluetooth_service.h"
#include "services/matrix_service.h"
#include "services/matrix_effects_service.h"
#include "apps/led_app.h"
#include "apps/glow_app.h"
#include "apps/matrix_app.h"
#include "apps/mood_app.h"
#include "apps/message_app.h"
#include "apps/bluetooth_app.h"
#include "apps/device_app.h"
#include "apps/app_registry.h"
#include "services/wifi_runtime_service.h"

void setupFirmware() {
  Serial.begin(115200);
  delay(250);

  pinMode(kLedPin, OUTPUT);
  setLedBrightness(kPwmMax);
  enableBoardWifiHardware();
  initializeBluetoothScanner();

  preferences.begin(kPreferenceNamespace, false);
  loadSavedPattern();
  loadSavedMatrixState();
  initializeMatrix();

  configureHttpServer();
  beginWifiConnection();
}

void loopFirmware() {
  updateLedPattern();
  updateMatrixPattern();
  updateWifiServices();

  if (webServerStarted) {
    webServer.handleClient();
  }
}

}  // namespace

void setup() {
  setupFirmware();
}

void loop() {
  loopFirmware();
}
