#pragma once

size_t matrixPixelIndex(uint8_t row, uint8_t column) {
  if (activeMatrixMapping == nullptr) {
    return 0;
  }

  const uint8_t mappedRow = activeMatrixMapping->flipY ? static_cast<uint8_t>(kMatrixRows - 1 - row) : row;
  const uint8_t mappedColumn = activeMatrixMapping->flipX ? static_cast<uint8_t>(kMatrixColumns - 1 - column) : column;

  if (!activeMatrixMapping->columnMajor) {
    const size_t baseIndex = static_cast<size_t>(mappedRow) * kMatrixColumns;
    if ((mappedRow & 1U) == 0) {
      return baseIndex + mappedColumn;
    }

    return baseIndex + (kMatrixColumns - 1 - mappedColumn);
  }

  const size_t baseIndex = static_cast<size_t>(mappedColumn) * kMatrixRows;
  if ((mappedColumn & 1U) == 0) {
    return baseIndex + mappedRow;
  }

  return baseIndex + (kMatrixRows - 1 - mappedRow);
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

void addMatrixPixel(uint8_t row, uint8_t column, const RgbColor &color) {
  if (row >= kMatrixRows || column >= kMatrixColumns) {
    return;
  }

  const size_t index = matrixPixelIndex(row, column);
  matrixFrame[index] = addColors(matrixFrame[index], color);
}

void rowMajorStepToCoord(size_t step, uint8_t &row, uint8_t &column) {
  step %= kMatrixLedCount;
  row = static_cast<uint8_t>(step / kMatrixColumns);
  column = static_cast<uint8_t>(step % kMatrixColumns);
}

void columnSerpentineStepToCoord(size_t step, uint8_t &row, uint8_t &column) {
  step %= kMatrixLedCount;
  column = static_cast<uint8_t>(step / kMatrixRows);
  const uint8_t offset = static_cast<uint8_t>(step % kMatrixRows);
  if ((column & 1U) == 0) {
    row = offset;
    return;
  }

  row = static_cast<uint8_t>(kMatrixRows - 1 - offset);
}

void zigzagStepToCoord(size_t step, uint8_t &row, uint8_t &column) {
  step %= kMatrixLedCount;
  row = static_cast<uint8_t>(step / kMatrixColumns);
  const uint8_t offset = static_cast<uint8_t>(step % kMatrixColumns);
  if ((row & 1U) == 0) {
    column = offset;
    return;
  }

  column = static_cast<uint8_t>(kMatrixColumns - 1 - offset);
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
    const RgbColor scaled = matrixEnabled ? scaleColor(matrixFrame[pixelIndex], matrixBrightness) : makeColor(0, 0, 0);
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

const MatrixMappingDefinition *findMatrixMappingById(const String &mappingId) {
  for (const MatrixMappingDefinition &mapping : kMatrixMappings) {
    if (mappingId == mapping.id) {
      return &mapping;
    }
  }

  return nullptr;
}

const MoodDefinition *findMoodById(const String &moodId) {
  for (const MoodDefinition &mood : kMoods) {
    if (moodId == mood.id) {
      return &mood;
    }
  }

  return nullptr;
}

const MatrixGlyph *findMatrixGlyph(char character) {
  for (const MatrixGlyph &glyph : kMatrixGlyphs) {
    if (glyph.character == character) {
      return &glyph;
    }
  }

  return nullptr;
}

uint8_t matrixGlyphWidth(const MatrixGlyph &glyph) {
  if (glyph.character == '-') {
    return kMatrixWideGlyphWidth;
  }

  return kMatrixGlyphWidth;
}

void drawMood(const MoodDefinition &mood) {
  clearMatrixFrame();
  const RgbColor moodColor = matrixBaseColor;

  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    const char *pixels = mood.rows[row];
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      setMatrixPixel(row, column, pixels[column] == '.' ? makeColor(0, 0, 0) : moodColor);
    }
  }
}

void drawGlyph(const MatrixGlyph &glyph, int originColumn, const RgbColor &color) {
  const uint8_t glyphWidth = matrixGlyphWidth(glyph);
  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < glyphWidth; ++column) {
      if ((glyph.rows[row] & (1U << (glyphWidth - 1U - column))) == 0) {
        continue;
      }

      const int targetColumn = originColumn + column;
      if (targetColumn < 0 || targetColumn >= kMatrixColumns) {
        continue;
      }

      setMatrixPixel(row, static_cast<uint8_t>(targetColumn), color);
    }
  }
}

size_t matrixMessageWidth(const String &text) {
  if (text.isEmpty()) {
    return 0;
  }

  size_t totalWidth = 0;
  bool foundGlyph = false;
  for (size_t index = 0; index < text.length(); ++index) {
    const MatrixGlyph *glyph = findMatrixGlyph(text.charAt(index));
    if (glyph == nullptr) {
      continue;
    }

    if (foundGlyph) {
      totalWidth += kMatrixGlyphSpacing;
    }
    totalWidth += matrixGlyphWidth(*glyph);
    foundGlyph = true;
  }

  return totalWidth;
}

String normalizeMatrixMessage(const String &rawText) {
  String normalized;
  normalized.reserve(min(rawText.length(), kMaxMatrixMessageLength));
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

    if (findMatrixGlyph(current) == nullptr) {
      continue;
    }

    if (normalized.length() >= kMaxMatrixMessageLength) {
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

void persistMatrixState() {
  preferences.putString(kMatrixPatternPreferenceKey, activeMatrixPattern->id);
  preferences.putBool(kMatrixEnabledPreferenceKey, matrixEnabled);
  preferences.putUChar(kMatrixBrightnessPreferenceKey, matrixBrightness);
  preferences.putUChar(kMatrixAnimationSpeedPreferenceKey, matrixAnimationSpeed);
  preferences.putString(kMatrixColorPreferenceKey, matrixColorHex);
  preferences.putString(kMatrixMappingPreferenceKey, activeMatrixMapping->id);
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

bool updateMatrixEnabled(bool enabled, bool persistSelection) {
  matrixEnabled = enabled;
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

bool updateMatrixAnimationSpeed(int animationSpeed, bool persistSelection) {
  if (animationSpeed < kMatrixMinAnimationSpeed || animationSpeed > kMatrixMaxAnimationSpeed) {
    return false;
  }

  matrixAnimationSpeed = static_cast<uint8_t>(animationSpeed);
  matrixFrameDirty = true;
  lastMatrixFrameAtMs = 0;

  if (persistSelection) {
    persistMatrixState();
  }

  return true;
}

bool updateMatrixMapping(const String &mappingId, bool persistSelection) {
  const MatrixMappingDefinition *mapping = findMatrixMappingById(mappingId);
  if (mapping == nullptr) {
    return false;
  }

  activeMatrixMapping = mapping;
  matrixFrameDirty = true;
  lastMatrixFrameAtMs = 0;

  if (persistSelection) {
    persistMatrixState();
  }

  return true;
}

bool updateMatrixMood(const String &moodId, bool persistSelection) {
  const MoodDefinition *mood = findMoodById(moodId);
  if (mood == nullptr) {
    return false;
  }

  matrixMoodId = mood->id;
  matrixFrameDirty = true;
  lastMatrixFrameAtMs = 0;

  if (persistSelection) {
    preferences.putString(kMatrixMoodPreferenceKey, matrixMoodId);
  }

  return true;
}

bool activateMatrixMood(const String &moodId, bool persistSelection) {
  if (!updateMatrixMood(moodId, persistSelection)) {
    return false;
  }

  return selectMatrixPatternById(kMatrixMoodPatternId, persistSelection);
}

bool updateMatrixMessageText(const String &rawText, bool persistSelection) {
  const String normalizedText = normalizeMatrixMessage(rawText);
  if (normalizedText.isEmpty()) {
    return false;
  }

  matrixMessageText = normalizedText;
  matrixFrameDirty = true;
  lastMatrixFrameAtMs = 0;

  if (persistSelection) {
    preferences.putString(kMatrixMessagePreferenceKey, matrixMessageText);
  }

  return true;
}

bool activateMatrixMessage(const String &rawText, bool persistSelection) {
  if (!updateMatrixMessageText(rawText, persistSelection)) {
    return false;
  }

  return selectMatrixPatternById(kMatrixMessagePatternId, persistSelection);
}

void loadSavedMatrixState() {
  updateMatrixColor(preferences.getString(kMatrixColorPreferenceKey, kMatrixDefaultColorHex), false);

  bool storedEnabled = preferences.getBool(kMatrixEnabledPreferenceKey, true);
  const uint8_t storedBrightness = preferences.getUChar(kMatrixBrightnessPreferenceKey, kMatrixDefaultBrightness);
  if (!updateMatrixBrightness(storedBrightness, false)) {
    updateMatrixBrightness(kMatrixDefaultBrightness, false);
  }

  const uint8_t storedAnimationSpeed = preferences.getUChar(kMatrixAnimationSpeedPreferenceKey, kMatrixDefaultAnimationSpeed);
  if (!updateMatrixAnimationSpeed(storedAnimationSpeed, false)) {
    updateMatrixAnimationSpeed(kMatrixDefaultAnimationSpeed, false);
  }

  if (!updateMatrixMapping(preferences.getString(kMatrixMappingPreferenceKey, kDefaultMatrixMappingId), false)) {
    updateMatrixMapping(kDefaultMatrixMappingId, false);
  }

  if (!updateMatrixMood(preferences.getString(kMatrixMoodPreferenceKey, kDefaultMatrixMoodId), false)) {
    updateMatrixMood(kDefaultMatrixMoodId, false);
  }

  if (!updateMatrixMessageText(preferences.getString(kMatrixMessagePreferenceKey, kDefaultMatrixMessage), false)) {
    updateMatrixMessageText(kDefaultMatrixMessage, false);
  }

  String storedPattern = preferences.getString(kMatrixPatternPreferenceKey, kDefaultMatrixPatternId);
  if (storedPattern == "off") {
    storedEnabled = false;
    storedPattern = kDefaultMatrixPatternId;
  }

  updateMatrixEnabled(storedEnabled, false);

  if (!selectMatrixPatternById(storedPattern, false)) {
    selectMatrixPatternById(kDefaultMatrixPatternId, false);
  }
}

RgbColor matrixPreviewColorAt(uint8_t row, uint8_t column) {
  if (!matrixEnabled) {
    return makeColor(0, 0, 0);
  }

  return scaleColor(matrixFrame[matrixPixelIndex(row, column)], matrixBrightness);
}

uint32_t matrixAnimationNow(uint32_t now) {
  return static_cast<uint32_t>((static_cast<uint64_t>(now) * matrixAnimationSpeed) / kMatrixDefaultAnimationSpeed);
}

void initializeMatrix() {
  clearMatrixFrame();
  matrixStrip.begin();
  matrixStrip.clear();
  matrixReady = true;
  matrixError = String("");
  applyMatrixFrameNow();
}
