#pragma once

void renderMatrixOff() {
  clearMatrixFrame();
}

RgbColor matrixEffectBaseColor() {
  if (activeMatrixPattern == nullptr) {
    return makeColor(0x22, 0xc5, 0x5e);
  }

  if (strcmp(activeMatrixPattern->id, "scanner") == 0) {
    return makeColor(0x2d, 0xd4, 0xbf);
  }
  if (strcmp(activeMatrixPattern->id, "chase") == 0) {
    return makeColor(0xf5, 0x9e, 0x0b);
  }
  if (strcmp(activeMatrixPattern->id, "pulse") == 0) {
    return makeColor(0xf4, 0x72, 0xb6);
  }
  if (strcmp(activeMatrixPattern->id, "checker") == 0) {
    return makeColor(0x38, 0xbd, 0xf8);
  }
  if (strcmp(activeMatrixPattern->id, "sparkle") == 0) {
    return makeColor(0xa7, 0xf3, 0xd0);
  }
  if (strcmp(activeMatrixPattern->id, "plasma") == 0) {
    return makeColor(0x81, 0x8c, 0xff);
  }
  if (strcmp(activeMatrixPattern->id, "prism") == 0) {
    return makeColor(0xf9, 0xa8, 0xd4);
  }
  if (strcmp(activeMatrixPattern->id, "ripple") == 0) {
    return makeColor(0x22, 0xd3, 0xee);
  }
  if (strcmp(activeMatrixPattern->id, "ripple-dual") == 0) {
    return makeColor(0xc0, 0x84, 0xfc);
  }
  if (strcmp(activeMatrixPattern->id, "ripple-core") == 0) {
    return makeColor(0xfb, 0x71, 0x71);
  }
  if (strcmp(activeMatrixPattern->id, "ripple-grid") == 0) {
    return makeColor(0x38, 0xbd, 0xf8);
  }
  if (strcmp(activeMatrixPattern->id, "ripple-surge") == 0) {
    return makeColor(0x2d, 0xd4, 0xbf);
  }
  if (strcmp(activeMatrixPattern->id, "meteor") == 0) {
    return makeColor(0x60, 0xa5, 0xfa);
  }
  if (strcmp(activeMatrixPattern->id, "confetti") == 0) {
    return makeColor(0x34, 0xd3, 0x99);
  }
  if (strcmp(activeMatrixPattern->id, "confetti-pop") == 0) {
    return makeColor(0xfb, 0x71, 0x71);
  }
  if (strcmp(activeMatrixPattern->id, "confetti-fizz") == 0) {
    return makeColor(0x67, 0xe8, 0xf9);
  }
  if (strcmp(activeMatrixPattern->id, "confetti-drift") == 0) {
    return makeColor(0xf4, 0x72, 0xb6);
  }
  if (strcmp(activeMatrixPattern->id, "confetti-shower") == 0) {
    return makeColor(0xf5, 0x9e, 0x0b);
  }
  if (strcmp(activeMatrixPattern->id, "spiral") == 0) {
    return makeColor(0xa3, 0xe6, 0x35);
  }
  if (strcmp(activeMatrixPattern->id, "radar") == 0) {
    return makeColor(0x22, 0xc5, 0x5e);
  }
  if (strcmp(activeMatrixPattern->id, "cross") == 0) {
    return makeColor(0xfb, 0x71, 0x71);
  }
  if (strcmp(activeMatrixPattern->id, "helix") == 0) {
    return makeColor(0x67, 0xe8, 0xf9);
  }
  if (strcmp(activeMatrixPattern->id, "zigzag-trace") == 0) {
    return makeColor(0xc0, 0x84, 0xfc);
  }
  if (strcmp(activeMatrixPattern->id, "rain-drops") == 0) {
    return makeColor(0x60, 0xa5, 0xfa);
  }

  return makeColor(0x22, 0xc5, 0x5e);
}

RgbColor confettiPaletteColor(const RgbColor &baseColor, uint32_t noise, uint8_t blendAmount) {
  return blendColors(baseColor, colorWheel(static_cast<uint8_t>((noise >> 8) & 0xFFU)), blendAmount);
}

RgbColor neonWaveColor(const RgbColor &baseColor, uint8_t hue, uint8_t wave, uint8_t floorAmount) {
  RgbColor color = blendColors(scaleColor(baseColor, floorAmount), colorWheel(hue), wave);
  if (wave > 228U) {
    color = blendColors(color, makeColor(255, 255, 255), static_cast<uint8_t>((wave - 228U) * 9U));
  }

  return color;
}

float sweepProjection(float x, float y, float directionX, float directionY) {
  return x * directionX + y * directionY;
}

void randomSweepDirection(uint8_t directionIndex, float &directionX, float &directionY) {
  constexpr float kInvSqrt2 = 0.70710678f;

  switch (directionIndex % 8U) {
    case 0:
      directionX = 1.0f;
      directionY = 0.0f;
      break;
    case 1:
      directionX = -1.0f;
      directionY = 0.0f;
      break;
    case 2:
      directionX = 0.0f;
      directionY = 1.0f;
      break;
    case 3:
      directionX = 0.0f;
      directionY = -1.0f;
      break;
    case 4:
      directionX = kInvSqrt2;
      directionY = kInvSqrt2;
      break;
    case 5:
      directionX = -kInvSqrt2;
      directionY = -kInvSqrt2;
      break;
    case 6:
      directionX = -kInvSqrt2;
      directionY = kInvSqrt2;
      break;
    default:
      directionX = kInvSqrt2;
      directionY = -kInvSqrt2;
      break;
  }
}

RgbColor sweepEventColor(uint32_t seed) {
  const uint8_t hue = static_cast<uint8_t>(((seed >> 8) & 0xFFU) + ((seed >> 22) & 0x3FU));
  return blendColors(colorWheel(hue), colorWheel(static_cast<uint8_t>(hue + 96U)), 56);
}

void renderMatrixSolid() {
  fillMatrixFrame(matrixSolidGlowColor());
}

void renderMatrixMood() {
  const MoodDefinition *mood = findMoodById(matrixMoodId);
  if (mood == nullptr) {
    mood = findMoodById(String(kDefaultMatrixMoodId));
  }

  if (mood == nullptr) {
    clearMatrixFrame();
    return;
  }

  drawMood(*mood);
}

void renderMatrixMessage(uint32_t now) {
  clearMatrixFrame();

  if (matrixMessageText.isEmpty()) {
    return;
  }

  const int textWidth = static_cast<int>(matrixMessageWidth(matrixMessageText));
  if (textWidth <= 0) {
    return;
  }

  const int travel = textWidth + kMatrixColumns + 1;
  const int offset = static_cast<int>((now / kMatrixMessageScrollMs) % static_cast<uint32_t>(travel));
  const int originColumn = kMatrixColumns - offset;
  const RgbColor textColor = matrixMessageRenderColor();
  int penColumn = originColumn;

  for (size_t index = 0; index < matrixMessageText.length(); ++index) {
    const MatrixGlyph *glyph = findMatrixGlyph(matrixMessageText.charAt(index));
    if (glyph == nullptr) {
      continue;
    }

    drawGlyph(*glyph, penColumn, textColor);
    penColumn += static_cast<int>(matrixGlyphWidth(*glyph) + kMatrixGlyphSpacing);
  }
}

void renderMatrixScanner(uint32_t now) {
  clearMatrixFrame();
  const RgbColor baseColor = matrixEffectBaseColor();

  const uint8_t travel = (kMatrixColumns - 1) * 2;
  uint8_t column = static_cast<uint8_t>((now / 80U) % travel);
  if (column >= kMatrixColumns) {
    column = static_cast<uint8_t>(travel - column);
  }

  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t currentColumn = 0; currentColumn < kMatrixColumns; ++currentColumn) {
      const int distance = abs(static_cast<int>(currentColumn) - static_cast<int>(column));
      if (distance == 0) {
        setMatrixPixel(row, currentColumn, baseColor);
      } else if (distance == 1) {
        setMatrixPixel(row, currentColumn, scaleColor(baseColor, 104));
      } else if (distance == 2) {
        setMatrixPixel(row, currentColumn, scaleColor(baseColor, 36));
      }
    }
  }
}

void renderMatrixSpectrumScan(uint32_t now) {
  clearMatrixFrame();

  const uint32_t cycleMs = 960U;
  const uint32_t activeMs = 780U;
  const uint32_t eventIndex = now / cycleMs;
  const uint32_t local = now % cycleMs;

  if (local >= activeMs) {
    return;
  }

  const uint32_t seed = hash32(eventIndex * 1181U + 97U);
  float directionX = 1.0f;
  float directionY = 0.0f;
  randomSweepDirection(static_cast<uint8_t>(seed & 0x7U), directionX, directionY);

  const float maxColumn = static_cast<float>(kMatrixColumns - 1U);
  const float maxRow = static_cast<float>(kMatrixRows - 1U);
  const float projections[] = {
    sweepProjection(0.0f, 0.0f, directionX, directionY),
    sweepProjection(maxColumn, 0.0f, directionX, directionY),
    sweepProjection(0.0f, maxRow, directionX, directionY),
    sweepProjection(maxColumn, maxRow, directionX, directionY),
  };

  float minProjection = projections[0];
  float maxProjection = projections[0];
  for (uint8_t index = 1; index < 4; ++index) {
    if (projections[index] < minProjection) {
      minProjection = projections[index];
    }
    if (projections[index] > maxProjection) {
      maxProjection = projections[index];
    }
  }

  const float overscan = 2.8f;
  const float progress = static_cast<float>(local) / static_cast<float>(activeMs);
  const float headProjection = (minProjection - overscan) + progress * ((maxProjection - minProjection) + overscan * 2.0f);
  const RgbColor sweepColor = sweepEventColor(seed);
  const RgbColor coreColor = blendColors(sweepColor, makeColor(255, 255, 255), 180);

  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const float pixelProjection = sweepProjection(static_cast<float>(column), static_cast<float>(row), directionX, directionY);
      const float delta = headProjection - pixelProjection;

      if (delta >= -0.24f && delta < 0.34f) {
        const float highlight = 1.0f - min(fabsf(delta - 0.04f) / 0.30f, 1.0f);
        setMatrixPixel(row, column, scaleColor(coreColor, static_cast<uint8_t>(132.0f + highlight * 123.0f)));
      } else if (delta >= 0.34f && delta < 1.22f) {
        const float fade = 1.0f - ((delta - 0.34f) / 0.88f);
        setMatrixPixel(row, column, scaleColor(sweepColor, static_cast<uint8_t>(94.0f + fade * 150.0f)));
      } else if (delta >= 1.22f && delta < 2.20f) {
        const float fade = 1.0f - ((delta - 1.22f) / 0.98f);
        setMatrixPixel(row, column, scaleColor(sweepColor, static_cast<uint8_t>(22.0f + fade * 70.0f)));
      }
    }
  }
}

void renderMatrixChase(uint32_t now) {
  const RgbColor baseColor = matrixEffectBaseColor();
  const uint8_t phase = static_cast<uint8_t>((now / 140U) % 3U);
  const RgbColor dim = scaleColor(baseColor, 18);

  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const bool lit = ((row + column + phase) % 3U) == 0;
      setMatrixPixel(row, column, lit ? baseColor : dim);
    }
  }
}

void renderMatrixPulse(uint32_t now) {
  const RgbColor baseColor = matrixEffectBaseColor();
  const float progress = static_cast<float>(now % 2200U) / 2200.0f;
  const float normalized = 0.12f + 0.88f * (0.5f - (0.5f * cosf(2.0f * PI * progress)));
  fillMatrixFrame(scaleColor(baseColor, static_cast<uint8_t>(normalized * 255.0f + 0.5f)));
}

void renderMatrixChecker(uint32_t now) {
  const RgbColor baseColor = matrixEffectBaseColor();
  const uint8_t phase = static_cast<uint8_t>((now / 420U) & 1U);
  const RgbColor accent = scaleColor(baseColor, 36);

  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const bool lit = ((row + column + phase) & 1U) == 0;
      setMatrixPixel(row, column, lit ? baseColor : accent);
    }
  }
}

void renderMatrixSparkle(uint32_t now) {
  const RgbColor baseColor = matrixEffectBaseColor();
  const uint32_t tick = now / 110U;
  const RgbColor background = scaleColor(baseColor, 22);
  const RgbColor medium = scaleColor(baseColor, 120);

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
  const RgbColor baseColor = matrixEffectBaseColor();
  const float phase = static_cast<float>(now) / 340.0f;
  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const float wave = sinf(column * 0.85f + phase) + sinf(row * 1.10f - phase * 1.25f) + sinf((column + row) * 0.55f + phase * 0.70f);
      const float normalized = (wave + 3.0f) / 6.0f;
      const uint8_t hue = static_cast<uint8_t>(normalized * 255.0f + row * 13U + column * 7U);
      setMatrixPixel(row, column, blendColors(scaleColor(baseColor, 48), colorWheel(hue), 196));
    }
  }
}

void renderMatrixPrism(uint32_t now) {
  const RgbColor baseColor = matrixEffectBaseColor();
  const float phase = static_cast<float>(now) / 260.0f;
  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const uint8_t stripe = sine8(column * 1.05f - row * 0.70f + phase);
      const uint8_t hue = static_cast<uint8_t>((now / 20U) + column * 26U + row * 18U);
      setMatrixPixel(row, column, blendColors(scaleColor(baseColor, 26), colorWheel(hue), stripe));
    }
  }
}

void renderMatrixRipple(uint32_t now) {
  const RgbColor baseColor = matrixEffectBaseColor();
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
      setMatrixPixel(row, column, neonWaveColor(baseColor, hue, wave, 14));
    }
  }
}

void renderMatrixDualRipple(uint32_t now) {
  clearMatrixFrame();
  const RgbColor baseColor = matrixEffectBaseColor();
  const float phase = static_cast<float>(now) / 140.0f;
  const float centerAX = 2.0f + 1.5f * sinf(static_cast<float>(now) / 620.0f);
  const float centerAY = 1.4f + 1.2f * cosf(static_cast<float>(now) / 760.0f);
  const float centerBX = 7.0f + 1.5f * cosf(static_cast<float>(now) / 680.0f);
  const float centerBY = 3.6f + 1.2f * sinf(static_cast<float>(now) / 540.0f);

  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const float deltaAX = column - centerAX;
      const float deltaAY = row - centerAY;
      const float deltaBX = column - centerBX;
      const float deltaBY = row - centerBY;
      const float distanceA = sqrtf(deltaAX * deltaAX + deltaAY * deltaAY);
      const float distanceB = sqrtf(deltaBX * deltaBX + deltaBY * deltaBY);
      const uint8_t waveA = sine8(distanceA * 3.0f - phase);
      const uint8_t waveB = sine8(distanceB * 3.0f - phase * 1.08f);
      const uint8_t highlight = max<uint8_t>(waveA, waveB);
      const uint8_t hue = static_cast<uint8_t>((now / 11U) + distanceA * 26.0f + distanceB * 21.0f);
      RgbColor color = neonWaveColor(baseColor, hue, highlight, 8);

      if (waveA > 208U && waveB > 208U) {
        color = blendColors(color, makeColor(255, 255, 255), 160);
      }

      setMatrixPixel(row, column, color);
    }
  }
}

void renderMatrixRippleCore(uint32_t now) {
  clearMatrixFrame();
  const RgbColor baseColor = matrixEffectBaseColor();
  const float centerX = (kMatrixColumns - 1) * 0.5f;
  const float centerY = (kMatrixRows - 1) * 0.5f;
  const float phase = static_cast<float>(now) / 105.0f;
  const uint8_t corePulse = sine8(phase * 0.55f);

  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const float deltaX = column - centerX;
      const float deltaY = row - centerY;
      const float distance = sqrtf(deltaX * deltaX + deltaY * deltaY);
      const uint8_t wave = sine8(distance * 3.6f - phase);
      const uint8_t hue = static_cast<uint8_t>((now / 10U) + distance * 30.0f + corePulse / 4U);
      RgbColor color = neonWaveColor(baseColor, hue, wave, 6);

      if (distance < 1.1f) {
        const float pulseStrength = static_cast<float>(corePulse) / 255.0f;
        const float coreStrength = max(0.0f, 1.0f - (distance / 1.1f));
        const uint8_t whiteMix = static_cast<uint8_t>(255.0f * coreStrength * (0.30f + 0.70f * pulseStrength) + 0.5f);
        color = blendColors(color, makeColor(255, 255, 255), whiteMix);
      }

      setMatrixPixel(row, column, color);
    }
  }
}

void renderMatrixRippleGrid(uint32_t now) {
  clearMatrixFrame();
  const RgbColor baseColor = matrixEffectBaseColor();
  const float phaseX = static_cast<float>(now) / 150.0f;
  const float phaseY = static_cast<float>(now) / 170.0f;

  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const uint8_t waveX = sine8(column * 1.8f - phaseX);
      const uint8_t waveY = sine8(row * 2.4f + phaseY);
      const uint8_t wave = max<uint8_t>(waveX, waveY);
      const uint8_t hue = static_cast<uint8_t>((now / 13U) + column * 22U + row * 31U);
      RgbColor color = neonWaveColor(baseColor, hue, wave, 10);

      if (waveX > 224U && waveY > 224U) {
        color = blendColors(color, makeColor(255, 255, 255), 180);
      }

      setMatrixPixel(row, column, color);
    }
  }
}

void renderMatrixRippleSurge(uint32_t now) {
  clearMatrixFrame();
  const RgbColor baseColor = matrixEffectBaseColor();
  const float phase = static_cast<float>(now) / 115.0f;

  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const float diagonal = column * 0.95f + row * 1.40f;
      const uint8_t leadWave = sine8(diagonal * 2.0f - phase);
      const uint8_t echoWave = sine8(diagonal * 3.3f - phase * 1.6f);
      const uint8_t echo = echoWave > 196U ? static_cast<uint8_t>(echoWave - 64U) : 0U;
      const uint8_t wave = max<uint8_t>(leadWave, echo);
      const uint8_t hue = static_cast<uint8_t>((now / 9U) + diagonal * 28.0f);
      RgbColor color = neonWaveColor(baseColor, hue, wave, 6);

      if (leadWave > 236U) {
        color = blendColors(color, makeColor(255, 255, 255), 148);
      }

      setMatrixPixel(row, column, color);
    }
  }
}

void renderMatrixMeteor(uint32_t now) {
  const RgbColor baseColor = matrixEffectBaseColor();
  fillMatrixFrame(scaleColor(baseColor, 8));

  const size_t headBase = static_cast<size_t>(now / 55U);
  for (uint8_t meteorIndex = 0; meteorIndex < 3; ++meteorIndex) {
    const size_t head = (headBase + meteorIndex * (kMatrixLedCount / 3)) % kMatrixLedCount;
    const RgbColor meteorColor = blendColors(baseColor, colorWheel(static_cast<uint8_t>(head * 5U + meteorIndex * 60U)), 132);

    for (uint8_t tail = 0; tail < 12; ++tail) {
      const size_t step = (head + kMatrixLedCount - tail) % kMatrixLedCount;
      const uint8_t intensity = static_cast<uint8_t>(255 - tail * 20U);
      uint8_t row = 0;
      uint8_t column = 0;
      columnSerpentineStepToCoord(step, row, column);
      setMatrixPixel(row, column, scaleColor(meteorColor, intensity));
    }
  }
}

void renderMatrixConfetti(uint32_t now) {
  const RgbColor baseColor = matrixEffectBaseColor();
  const uint32_t tick = now / 85U;
  clearMatrixFrame();

  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const size_t logicalIndex = static_cast<size_t>(row) * kMatrixColumns + column;
      const uint32_t noise = hash32(tick * 313U + logicalIndex * 101U);
      const uint8_t selector = static_cast<uint8_t>(noise & 0xFFU);
      if (selector > 249U) {
        setMatrixPixel(row, column, makeColor(255, 255, 255));
      } else if (selector > 241U) {
        setMatrixPixel(row, column, confettiPaletteColor(baseColor, noise, 196));
      } else if (selector > 236U) {
        setMatrixPixel(row, column, scaleColor(confettiPaletteColor(baseColor, noise, 212), 120));
      }
    }
  }
}

void renderMatrixConfettiPop(uint32_t now) {
  clearMatrixFrame();
  const RgbColor baseColor = matrixEffectBaseColor();
  const uint32_t cycleMs = 980U;

  for (uint8_t burstIndex = 0; burstIndex < 3; ++burstIndex) {
    const uint32_t shiftedNow = now + burstIndex * (cycleMs / 3U);
    const uint32_t eventIndex = shiftedNow / cycleMs;
    const uint32_t local = shiftedNow % cycleMs;
    const uint32_t seed = hash32(eventIndex * 577U + burstIndex * 131U);
    const float centerX = 1.0f + static_cast<float>(seed % (kMatrixColumns - 2U));
    const float centerY = 1.0f + static_cast<float>((seed >> 8) % (kMatrixRows - 2U));
    const RgbColor burstColor = confettiPaletteColor(baseColor, seed, 188);

    if (local < 140U) {
      const uint8_t coreMix = static_cast<uint8_t>(255U - ((local * 180U) / 140U));
      addMatrixPixel(static_cast<uint8_t>(centerY), static_cast<uint8_t>(centerX), blendColors(burstColor, makeColor(255, 255, 255), coreMix));
      continue;
    }

    const float radius = static_cast<float>(local - 140U) / static_cast<float>(cycleMs - 140U) * 4.6f;

    for (uint8_t row = 0; row < kMatrixRows; ++row) {
      for (uint8_t column = 0; column < kMatrixColumns; ++column) {
        const size_t logicalIndex = static_cast<size_t>(row) * kMatrixColumns + column;
        const float deltaX = static_cast<float>(column) - centerX;
        const float deltaY = static_cast<float>(row) - centerY;
        const float distance = sqrtf(deltaX * deltaX + deltaY * deltaY);
        const float delta = fabsf(distance - radius);

        if (delta <= 0.42f) {
          const uint8_t intensity = static_cast<uint8_t>(220.0f * (1.0f - (delta / 0.42f)));
          addMatrixPixel(row, column, scaleColor(confettiPaletteColor(baseColor, seed + logicalIndex * 53U, 208), intensity));
        } else if (delta <= 0.92f) {
          const uint32_t sparkSeed = hash32(seed + logicalIndex * 41U + local * 17U);
          if ((sparkSeed & 0x3U) == 0U) {
            const uint8_t intensity = static_cast<uint8_t>(120.0f * (1.0f - ((delta - 0.42f) / 0.50f)));
            addMatrixPixel(row, column, scaleColor(confettiPaletteColor(baseColor, sparkSeed, 212), intensity));
          }
        }
      }
    }
  }
}

void renderMatrixConfettiFizz(uint32_t now) {
  clearMatrixFrame();
  const RgbColor baseColor = matrixEffectBaseColor();
  const uint32_t tick = now / 58U;

  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const size_t logicalIndex = static_cast<size_t>(row) * kMatrixColumns + column;
      const uint32_t noise = hash32(tick * 457U + logicalIndex * 131U);
      const uint8_t selector = static_cast<uint8_t>(noise & 0xFFU);

      if (selector > 252U) {
        setMatrixPixel(row, column, makeColor(255, 255, 255));
      } else if (selector > 244U) {
        const RgbColor color = confettiPaletteColor(baseColor, noise, 212);
        setMatrixPixel(row, column, color);
        if (column + 1U < kMatrixColumns && (((noise >> 16) & 0x1U) == 0U)) {
          addMatrixPixel(row, column + 1U, scaleColor(color, 72));
        }
      } else if (selector > 238U && (((noise >> 11) & 0x1U) == 0U)) {
        setMatrixPixel(row, column, scaleColor(confettiPaletteColor(baseColor, noise, 188), 96));
      }
    }
  }
}

void renderMatrixConfettiDrift(uint32_t now) {
  clearMatrixFrame();
  const RgbColor baseColor = matrixEffectBaseColor();
  const float travelWidth = static_cast<float>(kMatrixColumns) + 4.0f;

  for (uint8_t particleIndex = 0; particleIndex < 7; ++particleIndex) {
    const uint32_t seed = hash32(particleIndex * 811U + 17U);
    const float phase = static_cast<float>(now) / (210.0f + static_cast<float>(seed & 0x7FU));
    const float x = fmodf(phase + particleIndex * 2.3f, travelWidth) - 2.0f;
    const float y = ((kMatrixRows - 1) * 0.5f) + 1.9f * sinf(static_cast<float>(now) / (260.0f + static_cast<float>((seed >> 8) & 0x3FU)) + particleIndex * 1.2f);
    const RgbColor particleColor = confettiPaletteColor(baseColor, seed + now * 13U, 204);

    for (uint8_t row = 0; row < kMatrixRows; ++row) {
      for (uint8_t column = 0; column < kMatrixColumns; ++column) {
        const float deltaX = static_cast<float>(column) - x;
        const float deltaY = static_cast<float>(row) - y;
        const float distance = sqrtf(deltaX * deltaX + deltaY * deltaY);

        if (distance < 0.60f) {
          addMatrixPixel(row, column, particleColor);
        } else if (distance < 1.20f) {
          addMatrixPixel(row, column, scaleColor(particleColor, static_cast<uint8_t>(130.0f * (1.0f - ((distance - 0.60f) / 0.60f)))));
        }
      }
    }
  }
}

void renderMatrixConfettiShower(uint32_t now) {
  clearMatrixFrame();
  const RgbColor baseColor = matrixEffectBaseColor();
  const uint32_t cycleMs = 1040U;

  for (uint8_t slot = 0; slot < 6; ++slot) {
    const uint32_t shiftedNow = now + slot * (cycleMs / 6U);
    const uint32_t eventIndex = shiftedNow / cycleMs;
    const uint32_t local = shiftedNow % cycleMs;
    const uint32_t seed = hash32(eventIndex * 661U + slot * 97U);
    const uint8_t column = static_cast<uint8_t>(seed % kMatrixColumns);
    const RgbColor particleColor = confettiPaletteColor(baseColor, seed, 196);

    if (local < 880U) {
      const int headRow = static_cast<int>(((kMatrixRows + 2U) * local) / 880U) - 1;
      for (uint8_t tail = 0; tail < 3; ++tail) {
        const int row = headRow - static_cast<int>(tail);
        if (row < 0 || row >= kMatrixRows) {
          continue;
        }

        const uint8_t intensity = static_cast<uint8_t>(255U - tail * 90U);
        addMatrixPixel(static_cast<uint8_t>(row), column, scaleColor(particleColor, intensity));
      }
      continue;
    }

    const uint8_t splashRow = static_cast<uint8_t>(kMatrixRows - 1U);
    addMatrixPixel(splashRow, column, blendColors(particleColor, makeColor(255, 255, 255), 160));
    if (column > 0U) {
      addMatrixPixel(splashRow, column - 1U, scaleColor(particleColor, 100));
    }
    if (column + 1U < kMatrixColumns) {
      addMatrixPixel(splashRow, column + 1U, scaleColor(particleColor, 100));
    }
    if (splashRow > 0U) {
      addMatrixPixel(splashRow - 1U, column, scaleColor(particleColor, 64));
    }
  }
}

void renderMatrixStatic(uint32_t now) {
  const uint32_t tick = now / 45U;

  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const size_t logicalIndex = static_cast<size_t>(row) * kMatrixColumns + column;
      const uint32_t noise = hash32(tick * 911U + logicalIndex * 137U);
      const uint8_t hue = static_cast<uint8_t>(noise & 0xFFU);
      const uint8_t intensity = static_cast<uint8_t>(40 + ((noise >> 8) & 0xD7U));
      setMatrixPixel(row, column, scaleColor(colorWheel(hue), intensity));
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
  const RgbColor baseColor = matrixEffectBaseColor();
  fillMatrixFrame(scaleColor(baseColor, 10));

  const size_t head = static_cast<size_t>(now / 75U) % kMatrixLedCount;
  for (uint8_t tail = 0; tail < 18; ++tail) {
    const size_t step = (head + kMatrixLedCount - tail) % kMatrixLedCount;
    uint8_t row = 0;
    uint8_t column = 0;
    spiralStepToCoord(step, row, column);
    const uint8_t hue = static_cast<uint8_t>((step * 17U) + (now / 18U));
    const RgbColor color = blendColors(baseColor, colorWheel(hue), 116);
    setMatrixPixel(row, column, scaleColor(color, static_cast<uint8_t>(255 - tail * 12U)));
  }
}

void renderMatrixRadar(uint32_t now) {
  clearMatrixFrame();
  const RgbColor baseColor = matrixEffectBaseColor();

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
      setMatrixPixel(row, column, scaleColor(baseColor, intensity));
    }
  }
}

void renderMatrixCross(uint32_t now) {
  clearMatrixFrame();
  const RgbColor baseColor = matrixEffectBaseColor();

  const float radius = triangleUnit(static_cast<float>(now % 1800U) / 1800.0f) * 4.5f;
  const float centerX = (kMatrixColumns - 1) * 0.5f;
  const float centerY = (kMatrixRows - 1) * 0.5f;
  const RgbColor accent = blendColors(baseColor, colorWheel(static_cast<uint8_t>(now / 15U)), 110);

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
        setMatrixPixel(row, column, scaleColor(baseColor, 8));
      }
    }
  }
}

void renderMatrixHelix(uint32_t now) {
  clearMatrixFrame();
  const RgbColor baseColor = matrixEffectBaseColor();

  for (uint8_t column = 0; column < kMatrixColumns; ++column) {
    const float phase = (now / 210.0f) + column * 0.62f;
    const float topWave = (0.5f + 0.5f * sinf(phase)) * (kMatrixRows - 1);
      const float bottomWave = (0.5f + 0.5f * sinf(phase + PI)) * (kMatrixRows - 1);

    for (uint8_t row = 0; row < kMatrixRows; ++row) {
      const float topDistance = fabsf(row - topWave);
      const float bottomDistance = fabsf(row - bottomWave);
      if (topDistance < 0.55f) {
        setMatrixPixel(row, column, baseColor);
      } else if (bottomDistance < 0.55f) {
        setMatrixPixel(row, column, blendColors(baseColor, colorWheel(170), 120));
      } else if (topDistance < 1.3f || bottomDistance < 1.3f) {
        setMatrixPixel(row, column, scaleColor(baseColor, 44));
      } else {
        setMatrixPixel(row, column, scaleColor(baseColor, 8));
      }
    }
  }
}

void renderMatrixRasterTrace(uint32_t now) {
  clearMatrixFrame();

  const size_t head = static_cast<size_t>(now / 55U) % kMatrixLedCount;
  for (uint8_t tail = 0; tail < 10; ++tail) {
    const size_t step = (head + kMatrixLedCount - tail) % kMatrixLedCount;
    uint8_t row = 0;
    uint8_t column = 0;
    rowMajorStepToCoord(step, row, column);
    const uint8_t hue = static_cast<uint8_t>((now / 12U) + tail * 22U);
    setMatrixPixel(row, column, scaleColor(colorWheel(hue), static_cast<uint8_t>(255 - tail * 20U)));
  }
}

void renderMatrixZigzagTrace(uint32_t now) {
  const RgbColor baseColor = matrixEffectBaseColor();
  fillMatrixFrame(scaleColor(baseColor, 6));

  const size_t head = static_cast<size_t>(now / 60U) % kMatrixLedCount;
  for (uint8_t tail = 0; tail < 12; ++tail) {
    const size_t step = (head + kMatrixLedCount - tail) % kMatrixLedCount;
    uint8_t row = 0;
    uint8_t column = 0;
    zigzagStepToCoord(step, row, column);
    const uint8_t hue = static_cast<uint8_t>((now / 14U) + step * 5U);
    setMatrixPixel(row, column, scaleColor(colorWheel(hue), static_cast<uint8_t>(255 - tail * 16U)));
  }
}

void renderMatrixSpiralTrace(uint32_t now) {
  clearMatrixFrame();

  const size_t head = static_cast<size_t>(now / 70U) % kMatrixLedCount;
  for (uint8_t tail = 0; tail < 14; ++tail) {
    const size_t step = (head + kMatrixLedCount - tail) % kMatrixLedCount;
    uint8_t row = 0;
    uint8_t column = 0;
    spiralStepToCoord(step, row, column);
    const uint8_t hue = static_cast<uint8_t>((step * 13U) + (now / 15U));
    setMatrixPixel(row, column, scaleColor(colorWheel(hue), static_cast<uint8_t>(255 - tail * 16U)));
  }
}

void renderMatrixRainDrops(uint32_t now) {
  const RgbColor baseColor = matrixEffectBaseColor();
  const uint8_t surfaceRow = 3;
  const RgbColor horizon = scaleColor(blendColors(baseColor, makeColor(24, 96, 255), 172), 44);
  const RgbColor water = scaleColor(blendColors(baseColor, makeColor(0, 30, 90), 200), 26);
  const uint32_t cycleMs = 1080U;
  const uint32_t fallMs = 260U;

  clearMatrixFrame();
  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      if (row < surfaceRow) {
        setMatrixPixel(row, column, makeColor(0, 0, 0));
      } else if (row == surfaceRow) {
        setMatrixPixel(row, column, horizon);
      } else {
        const uint8_t depth = static_cast<uint8_t>(34U + (row - surfaceRow) * 20U);
        setMatrixPixel(row, column, scaleColor(water, depth));
      }
    }
  }

  for (uint8_t slot = 0; slot < 3; ++slot) {
    const uint32_t shiftedNow = now + slot * (cycleMs / 3U);
    const uint32_t eventIndex = shiftedNow / cycleMs;
    const uint32_t local = shiftedNow % cycleMs;
    const uint32_t seed = hash32(eventIndex * 277U + slot * 911U);
    const uint8_t impactColumn = static_cast<uint8_t>(1U + (seed % (kMatrixColumns - 2U)));

    if (local < fallMs) {
      const float progress = static_cast<float>(local) / static_cast<float>(fallMs);
      const int headRow = static_cast<int>(progress * surfaceRow + 0.5f);
      for (uint8_t trail = 0; trail < 3; ++trail) {
        const int row = headRow - trail;
        if (row < 0 || row > surfaceRow) {
          continue;
        }

        const uint8_t intensity = static_cast<uint8_t>(255U - trail * 80U);
        addMatrixPixel(static_cast<uint8_t>(row), impactColumn, scaleColor(makeColor(210, 240, 255), intensity));
      }
      continue;
    }

    const uint32_t rippleAge = local - fallMs;
    const float progress = static_cast<float>(rippleAge) / static_cast<float>(cycleMs - fallMs);
    const float radius = progress * 4.0f;
    const float coreFade = 1.0f - min(progress * 1.8f, 1.0f);

    for (uint8_t row = surfaceRow; row < kMatrixRows; ++row) {
      for (uint8_t column = 0; column < kMatrixColumns; ++column) {
        const float dx = static_cast<float>(column) - static_cast<float>(impactColumn);
        const float dy = (static_cast<float>(row) - static_cast<float>(surfaceRow)) * 1.5f;
        const float distance = sqrtf(dx * dx + dy * dy);
        const float delta = fabsf(distance - radius);

        if (delta <= 0.42f) {
          const uint8_t intensity = static_cast<uint8_t>(210.0f * (1.0f - (delta / 0.42f)));
          addMatrixPixel(row, column, scaleColor(blendColors(baseColor, makeColor(96, 180, 255), 180), intensity));
        } else if (delta <= 0.9f) {
          const uint8_t intensity = static_cast<uint8_t>(88.0f * (1.0f - ((delta - 0.42f) / 0.48f)));
          addMatrixPixel(row, column, scaleColor(makeColor(40, 120, 255), intensity));
        }
      }
    }

    if (coreFade > 0.0f) {
      addMatrixPixel(surfaceRow, impactColumn, scaleColor(makeColor(255, 255, 255), static_cast<uint8_t>(coreFade * 255.0f)));
      if (impactColumn > 0) {
        addMatrixPixel(surfaceRow, impactColumn - 1, scaleColor(makeColor(140, 220, 255), static_cast<uint8_t>(coreFade * 120.0f)));
      }
      if (impactColumn + 1 < kMatrixColumns) {
        addMatrixPixel(surfaceRow, impactColumn + 1, scaleColor(makeColor(140, 220, 255), static_cast<uint8_t>(coreFade * 120.0f)));
      }
    }
  }
}

void renderMatrixFrame(uint32_t now) {
  if (activeMatrixPattern == nullptr) {
    clearMatrixFrame();
    return;
  }

  if (strcmp(activeMatrixPattern->id, kMatrixSolidPatternId) == 0) {
    renderMatrixSolid();
  } else if (strcmp(activeMatrixPattern->id, "scanner") == 0) {
    renderMatrixScanner(now);
  } else if (strcmp(activeMatrixPattern->id, "spectrum-scan") == 0) {
    renderMatrixSpectrumScan(now);
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
  } else if (strcmp(activeMatrixPattern->id, "ripple-dual") == 0) {
    renderMatrixDualRipple(now);
  } else if (strcmp(activeMatrixPattern->id, "ripple-core") == 0) {
    renderMatrixRippleCore(now);
  } else if (strcmp(activeMatrixPattern->id, "ripple-grid") == 0) {
    renderMatrixRippleGrid(now);
  } else if (strcmp(activeMatrixPattern->id, "ripple-surge") == 0) {
    renderMatrixRippleSurge(now);
  } else if (strcmp(activeMatrixPattern->id, "meteor") == 0) {
    renderMatrixMeteor(now);
  } else if (strcmp(activeMatrixPattern->id, "confetti") == 0) {
    renderMatrixConfetti(now);
  } else if (strcmp(activeMatrixPattern->id, "confetti-pop") == 0) {
    renderMatrixConfettiPop(now);
  } else if (strcmp(activeMatrixPattern->id, "confetti-fizz") == 0) {
    renderMatrixConfettiFizz(now);
  } else if (strcmp(activeMatrixPattern->id, "confetti-drift") == 0) {
    renderMatrixConfettiDrift(now);
  } else if (strcmp(activeMatrixPattern->id, "confetti-shower") == 0) {
    renderMatrixConfettiShower(now);
  } else if (strcmp(activeMatrixPattern->id, "static") == 0) {
    renderMatrixStatic(now);
  } else if (strcmp(activeMatrixPattern->id, "lava") == 0) {
    renderMatrixLava(now);
  } else if (strcmp(activeMatrixPattern->id, "spiral") == 0) {
    renderMatrixSpiral(now);
  } else if (strcmp(activeMatrixPattern->id, "radar") == 0) {
    renderMatrixRadar(now);
  } else if (strcmp(activeMatrixPattern->id, "cross") == 0) {
    renderMatrixCross(now);
  } else if (strcmp(activeMatrixPattern->id, "helix") == 0) {
    renderMatrixHelix(now);
  } else if (strcmp(activeMatrixPattern->id, "raster-trace") == 0) {
    renderMatrixRasterTrace(now);
  } else if (strcmp(activeMatrixPattern->id, "zigzag-trace") == 0) {
    renderMatrixZigzagTrace(now);
  } else if (strcmp(activeMatrixPattern->id, "spiral-trace") == 0) {
    renderMatrixSpiralTrace(now);
  } else if (strcmp(activeMatrixPattern->id, "rain-drops") == 0) {
    renderMatrixRainDrops(now);
  } else if (strcmp(activeMatrixPattern->id, kMatrixMoodPatternId) == 0) {
    renderMatrixMood();
  } else if (strcmp(activeMatrixPattern->id, kMatrixMessagePatternId) == 0) {
    renderMatrixMessage(now);
  } else {
    clearMatrixFrame();
  }
}

void applyMatrixFrameNow() {
  renderMatrixFrame(matrixAnimationNow(millis()));
  writeMatrixFrame();
  matrixFrameDirty = false;
  lastMatrixFrameAtMs = millis();
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

  renderMatrixFrame(matrixAnimationNow(now));
  writeMatrixFrame();
  matrixFrameDirty = false;
  lastMatrixFrameAtMs = now;
}
