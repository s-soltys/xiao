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
  if (strcmp(activeMatrixPattern->id, "meteor") == 0) {
    return makeColor(0x60, 0xa5, 0xfa);
  }
  if (strcmp(activeMatrixPattern->id, "confetti") == 0) {
    return makeColor(0x34, 0xd3, 0x99);
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
      setMatrixPixel(row, column, blendColors(scaleColor(baseColor, 14), colorWheel(hue), wave));
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
  const RgbColor background = scaleColor(baseColor, 10);

  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const size_t logicalIndex = static_cast<size_t>(row) * kMatrixColumns + column;
      const uint32_t noise = hash32(tick * 313U + logicalIndex * 101U);
      const uint8_t selector = static_cast<uint8_t>(noise & 0xFFU);
      if (selector > 242) {
        setMatrixPixel(row, column, colorWheel(static_cast<uint8_t>((noise >> 8) & 0xFFU)));
      } else if (selector > 232) {
        setMatrixPixel(row, column, makeColor(255, 255, 255));
      } else {
        setMatrixPixel(row, column, background);
      }
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
  } else if (strcmp(activeMatrixPattern->id, "confetti") == 0) {
    renderMatrixConfetti(now);
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
