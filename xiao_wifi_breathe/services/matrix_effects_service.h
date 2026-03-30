#pragma once

void renderMatrixOff() {
  clearMatrixFrame();
}

void renderMatrixSolid() {
  fillMatrixFrame(matrixBaseColor);
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
  const RgbColor textColor = blendColors(matrixBaseColor, makeColor(255, 255, 255), 104);
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
      const size_t step = (head + kMatrixLedCount - tail) % kMatrixLedCount;
      const uint8_t intensity = static_cast<uint8_t>(255 - tail * 20U);
      uint8_t row = 0;
      uint8_t column = 0;
      columnSerpentineStepToCoord(step, row, column);
      setMatrixPixel(row, column, scaleColor(meteorColor, intensity));
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

void renderMatrixPixelSpectrum(uint32_t now) {
  clearMatrixFrame();

  const uint32_t tick = now / 18U;
  const size_t pixel = static_cast<size_t>((tick / 256U) % kMatrixLedCount);
  const uint8_t hue = static_cast<uint8_t>(tick & 0xFFU);
  uint8_t row = 0;
  uint8_t column = 0;
  rowMajorStepToCoord(pixel, row, column);
  setMatrixPixel(row, column, colorWheel(hue));
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
  fillMatrixFrame(scaleColor(matrixBaseColor, 6));

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

void renderMatrixRowSpectrum(uint32_t now) {
  clearMatrixFrame();

  const uint32_t tick = now / 16U;
  const uint8_t activeRow = static_cast<uint8_t>((tick / 256U) % kMatrixRows);
  const uint8_t baseHue = static_cast<uint8_t>(tick & 0xFFU);
  for (uint8_t column = 0; column < kMatrixColumns; ++column) {
    setMatrixPixel(activeRow, column, colorWheel(static_cast<uint8_t>(baseHue + column * 18U)));
  }
}

void renderMatrixColumnSpectrum(uint32_t now) {
  clearMatrixFrame();

  const uint32_t tick = now / 16U;
  const uint8_t activeColumn = static_cast<uint8_t>((tick / 256U) % kMatrixColumns);
  const uint8_t baseHue = static_cast<uint8_t>(tick & 0xFFU);
  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    setMatrixPixel(row, activeColumn, colorWheel(static_cast<uint8_t>(baseHue + row * 28U)));
  }
}

void renderMatrixRowFill(uint32_t now) {
  clearMatrixFrame();

  const uint8_t phase = static_cast<uint8_t>((now / 220U) % (kMatrixRows * 2U));
  const uint8_t filledRows = phase < kMatrixRows ? static_cast<uint8_t>(phase + 1U) : static_cast<uint8_t>((kMatrixRows * 2U) - phase - 1U);
  for (uint8_t row = 0; row < filledRows; ++row) {
    const RgbColor rowColor = blendColors(matrixBaseColor, colorWheel(static_cast<uint8_t>(row * 36U + now / 18U)), 110);
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      setMatrixPixel(static_cast<uint8_t>(kMatrixRows - 1 - row), column, rowColor);
    }
  }
}

void renderMatrixColumnFill(uint32_t now) {
  clearMatrixFrame();

  const uint8_t phase = static_cast<uint8_t>((now / 180U) % (kMatrixColumns * 2U));
  const uint8_t filledColumns = phase < kMatrixColumns ? static_cast<uint8_t>(phase + 1U) : static_cast<uint8_t>((kMatrixColumns * 2U) - phase - 1U);
  for (uint8_t column = 0; column < filledColumns; ++column) {
    const RgbColor columnColor = blendColors(matrixBaseColor, colorWheel(static_cast<uint8_t>(column * 20U + now / 14U)), 128);
    for (uint8_t row = 0; row < kMatrixRows; ++row) {
      setMatrixPixel(row, column, columnColor);
    }
  }
}

void renderMatrixDiagonalWipe(uint32_t now) {
  fillMatrixFrame(scaleColor(matrixBaseColor, 5));

  const int stripe = static_cast<int>((now / 75U) % (kMatrixColumns + kMatrixRows - 1U)) - static_cast<int>(kMatrixRows - 1U);
  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const int distance = abs(static_cast<int>(column) - static_cast<int>(row) - stripe);
      if (distance == 0) {
        setMatrixPixel(row, column, makeColor(255, 255, 255));
      } else if (distance == 1) {
        setMatrixPixel(row, column, colorWheel(static_cast<uint8_t>(now / 10U + column * 18U)));
      }
    }
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

void renderMatrixBorderFill(uint32_t now) {
  fillMatrixFrame(scaleColor(matrixBaseColor, 4));

  const size_t perimeter = borderPixelCount();
  const size_t phase = static_cast<size_t>(now / 90U) % (perimeter * 2U);
  const size_t filled = phase < perimeter ? (phase + 1U) : ((perimeter * 2U) - phase - 1U);
  for (size_t step = 0; step < filled; ++step) {
    uint8_t row = 0;
    uint8_t column = 0;
    borderStepToCoord(step, row, column);
    setMatrixPixel(row, column, colorWheel(static_cast<uint8_t>(now / 11U + step * 7U)));
  }
}

void renderMatrixRainDrops(uint32_t now) {
  const uint8_t surfaceRow = 3;
  const RgbColor horizon = scaleColor(blendColors(matrixBaseColor, makeColor(24, 96, 255), 172), 44);
  const RgbColor water = scaleColor(blendColors(matrixBaseColor, makeColor(0, 30, 90), 200), 26);
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
          addMatrixPixel(row, column, scaleColor(blendColors(matrixBaseColor, makeColor(96, 180, 255), 180), intensity));
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
  } else if (strcmp(activeMatrixPattern->id, "pixel-spectrum") == 0) {
    renderMatrixPixelSpectrum(now);
  } else if (strcmp(activeMatrixPattern->id, "raster-trace") == 0) {
    renderMatrixRasterTrace(now);
  } else if (strcmp(activeMatrixPattern->id, "zigzag-trace") == 0) {
    renderMatrixZigzagTrace(now);
  } else if (strcmp(activeMatrixPattern->id, "row-spectrum") == 0) {
    renderMatrixRowSpectrum(now);
  } else if (strcmp(activeMatrixPattern->id, "column-spectrum") == 0) {
    renderMatrixColumnSpectrum(now);
  } else if (strcmp(activeMatrixPattern->id, "row-fill") == 0) {
    renderMatrixRowFill(now);
  } else if (strcmp(activeMatrixPattern->id, "column-fill") == 0) {
    renderMatrixColumnFill(now);
  } else if (strcmp(activeMatrixPattern->id, "diagonal-wipe") == 0) {
    renderMatrixDiagonalWipe(now);
  } else if (strcmp(activeMatrixPattern->id, "spiral-trace") == 0) {
    renderMatrixSpiralTrace(now);
  } else if (strcmp(activeMatrixPattern->id, "border-fill") == 0) {
    renderMatrixBorderFill(now);
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
