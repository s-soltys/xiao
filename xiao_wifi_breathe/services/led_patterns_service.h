#pragma once

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
  if (activePattern == nullptr) {
    return;
  }

  if (activePattern->mode == PatternMode::kSequence) {
    updateSequencePattern(now);
    return;
  }

  updateWavePattern(now);
}
