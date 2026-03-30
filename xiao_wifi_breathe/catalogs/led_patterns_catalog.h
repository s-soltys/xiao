#pragma once

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
