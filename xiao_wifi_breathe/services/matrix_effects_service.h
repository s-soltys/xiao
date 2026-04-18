#pragma once

void renderMatrixOff() {
  clearMatrixFrame();
}

int matrixEffectVariant3(const char *patternId, const char *firstId, const char *secondId, const char *thirdId);

RgbColor matrixEffectBaseColor() {
  if (activeMatrixPattern == nullptr) {
    return makeColor(0x22, 0xc5, 0x5e);
  }

  if (strcmp(activeMatrixPattern->id, "scanner") == 0) {
    return makeColor(0x2d, 0xd4, 0xbf);
  }
  if (strcmp(activeMatrixPattern->id, "spectrum-scan") == 0) {
    return makeColor(0xf9, 0xa8, 0xd4);
  }
  if (strcmp(activeMatrixPattern->id, "spectrum-storm") == 0) {
    return makeColor(0xc4, 0xb5, 0xfd);
  }
  if (strcmp(activeMatrixPattern->id, "lightning") == 0) {
    return makeColor(0x67, 0xe8, 0xf9);
  }
  if (strcmp(activeMatrixPattern->id, "storm-ripple") == 0) {
    return makeColor(0x7d, 0xf9, 0xff);
  }
  if (strcmp(activeMatrixPattern->id, "storm-raster") == 0) {
    return makeColor(0x93, 0xc5, 0xfd);
  }
  if (strcmp(activeMatrixPattern->id, "pride-wave") == 0) {
    return makeColor(0xff, 0x4d, 0x8d);
  }
  if (strcmp(activeMatrixPattern->id, "heart-throb") == 0) {
    return makeColor(0xe1, 0x1d, 0x48);
  }
  if (strcmp(activeMatrixPattern->id, "beat-wave") == 0) {
    return makeColor(0xf4, 0x72, 0xb6);
  }
  if (strcmp(activeMatrixPattern->id, "chase") == 0) {
    return makeColor(0xf5, 0x9e, 0x0b);
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
  if (strcmp(activeMatrixPattern->id, "ripple") == 0) {
    return makeColor(0x22, 0xd3, 0xee);
  }
  if (strcmp(activeMatrixPattern->id, "ripple-dual") == 0) {
    return makeColor(0xc0, 0x84, 0xfc);
  }
  if (strcmp(activeMatrixPattern->id, "ripple-core") == 0) {
    return makeColor(0xfb, 0x71, 0x71);
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
  if (strcmp(activeMatrixPattern->id, "helix") == 0) {
    return makeColor(0x67, 0xe8, 0xf9);
  }
  if (strcmp(activeMatrixPattern->id, "zigzag-trace") == 0) {
    return makeColor(0xc0, 0x84, 0xfc);
  }
  if (matrixEffectVariant3(activeMatrixPattern->id, "aurora-drift", "aurora-fold", "aurora-borealis") >= 0) {
    return makeColor(0x5e, 0xff, 0xd3);
  }
  if (matrixEffectVariant3(activeMatrixPattern->id, "tidal-pulse", "undertow", "crosscurrent") >= 0) {
    return makeColor(0x38, 0xbd, 0xf8);
  }
  if (matrixEffectVariant3(activeMatrixPattern->id, "gravity-well", "graviton-lens", "singularity-burst") >= 0) {
    return makeColor(0xa7, 0x8b, 0xfa);
  }
  if (matrixEffectVariant3(activeMatrixPattern->id, "ember-updraft", "forge-bloom", "ashfall") >= 0) {
    return makeColor(0xfb, 0x92, 0x3c);
  }
  if (matrixEffectVariant3(activeMatrixPattern->id, "prism-fan", "prism-slice", "prism-cascade") >= 0) {
    return makeColor(0xfa, 0xe8, 0x68);
  }
  if (matrixEffectVariant3(activeMatrixPattern->id, "swarm", "swarm-weave", "swarm-burst") >= 0) {
    return makeColor(0x7d, 0xfc, 0x8a);
  }
  if (matrixEffectVariant3(activeMatrixPattern->id, "solar-flare", "corona-spin", "eclipse-ring") >= 0) {
    return makeColor(0xfb, 0x71, 0x15);
  }
  if (matrixEffectVariant3(activeMatrixPattern->id, "data-rain", "data-stream", "data-burst") >= 0) {
    return makeColor(0x34, 0xd3, 0x99);
  }
  if (matrixEffectVariant3(activeMatrixPattern->id, "dune-shift", "dune-rush", "dune-vault") >= 0) {
    return makeColor(0xfb, 0xbf, 0x24);
  }
  if (matrixEffectVariant3(activeMatrixPattern->id, "quake-lines", "fault-pulse", "fracture-field") >= 0) {
    return makeColor(0xf8, 0x71, 0x71);
  }
  if (matrixEffectVariant3(activeMatrixPattern->id, "sonar-ping", "sonar-fan", "sonar-net") >= 0) {
    return makeColor(0x2d, 0xd4, 0xbf);
  }
  if (matrixEffectVariant3(activeMatrixPattern->id, "starfield-warp", "hyperspace", "orbital-slip") >= 0) {
    return makeColor(0x60, 0xa5, 0xfa);
  }
  if (matrixEffectVariant3(activeMatrixPattern->id, "strobe-grid", "strobe-slices", "strobe-tunnel") >= 0) {
    return makeColor(0xf8, 0xfa, 0xfc);
  }
  if (matrixEffectVariant3(activeMatrixPattern->id, "wave-lattice", "phase-knot", "caustic-flow") >= 0) {
    return makeColor(0x67, 0xe8, 0xf9);
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

RgbColor sweepEventColor(uint32_t seed) {
  const uint8_t hue = static_cast<uint8_t>(((seed >> 8) & 0xFFU) + ((seed >> 22) & 0x3FU));
  return blendColors(colorWheel(hue), colorWheel(static_cast<uint8_t>(hue + 96U)), 56);
}

struct LightningPoint {
  int row;
  int column;
};

struct LightningPatternSegment {
  int startRow;
  int startColumn;
  int endRow;
  int endColumn;
};

RgbColor lightningStrikeColor(uint32_t seed) {
  const uint8_t hue = static_cast<uint8_t>(((seed >> 7) & 0xFFU) + ((seed >> 19) & 0x3FU));
  return blendColors(colorWheel(hue), colorWheel(static_cast<uint8_t>(hue + 40U)), 84);
}

RgbColor stormEventColor(uint32_t seed) {
  const RgbColor primary = blendColors(sweepEventColor(seed), lightningStrikeColor(seed ^ 0x4f1bbcdcU), 96);
  const uint8_t accentHue = static_cast<uint8_t>(((seed >> 11) & 0xFFU) + ((seed >> 23) & 0x1FU));
  const uint8_t accentMix = static_cast<uint8_t>(48U + ((seed >> 5) & 0x5FU));
  return blendColors(primary, colorWheel(accentHue), accentMix);
}

size_t wrappedTraversalStep(size_t startOffset, size_t offset, size_t count, bool reverse) {
  if (count == 0U) {
    return 0U;
  }

  startOffset %= count;
  offset %= count;
  if (!reverse) {
    return (startOffset + offset) % count;
  }

  return (startOffset + count - offset) % count;
}

RgbColor prideStripeColor(uint8_t stripeIndex) {
  switch (stripeIndex) {
    case 0:
      return makeColor(0xE4, 0x03, 0x03);
    case 1:
      return makeColor(0xFF, 0x8C, 0x00);
    case 2:
      return makeColor(0xFF, 0xED, 0x00);
    case 3:
      return makeColor(0x00, 0x80, 0x26);
    case 4:
      return makeColor(0x00, 0x4D, 0xFF);
    default:
      return makeColor(0x75, 0x07, 0x87);
  }
}

float pulseWindow(float progress, float center, float halfWidth) {
  const float distance = fabsf(progress - center);
  if (distance >= halfWidth || halfWidth <= 0.0f) {
    return 0.0f;
  }

  return 0.5f + 0.5f * cosf((distance / halfWidth) * PI);
}

float heartbeatEnvelope(float progress) {
  return max(
    pulseWindow(progress, 0.18f, 0.10f),
    0.82f * pulseWindow(progress, 0.34f, 0.08f)
  );
}

float bandStrength(float value, float center, float halfWidth) {
  if (halfWidth <= 0.0f) {
    return 0.0f;
  }

  return max(0.0f, 1.0f - (fabsf(value - center) / halfWidth));
}

float radialStrength(float distance, float radius) {
  if (radius <= 0.0f) {
    return 0.0f;
  }

  return max(0.0f, 1.0f - (distance / radius));
}

void addSoftOrb(float centerRow, float centerColumn, float radius, const RgbColor &color, uint8_t intensityScale) {
  if (radius <= 0.0f || intensityScale == 0U) {
    return;
  }

  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const float deltaX = static_cast<float>(column) - centerColumn;
      const float deltaY = static_cast<float>(row) - centerRow;
      const float distance = sqrtf(deltaX * deltaX + deltaY * deltaY);
      const float mask = radialStrength(distance, radius);
      if (mask <= 0.0f) {
        continue;
      }

      addMatrixPixel(row, column, scaleColor(color, static_cast<uint8_t>(mask * intensityScale + 0.5f)));
    }
  }
}

uint8_t unitToScale(float value) {
  return static_cast<uint8_t>(constrain(value, 0.0f, 1.0f) * 255.0f + 0.5f);
}

int matrixEffectVariant3(const char *patternId, const char *firstId, const char *secondId, const char *thirdId) {
  if (strcmp(patternId, firstId) == 0) {
    return 0;
  }
  if (strcmp(patternId, secondId) == 0) {
    return 1;
  }
  if (strcmp(patternId, thirdId) == 0) {
    return 2;
  }

  return -1;
}

float wrappedAngleDelta(float firstAngle, float secondAngle) {
  float delta = fabsf(firstAngle - secondAngle);
  if (delta > PI) {
    delta = (2.0f * PI) - delta;
  }

  return delta;
}

void selectLightningEndpoints(uint32_t seed, int &startRow, int &startColumn, int &targetRow, int &targetColumn) {
  const uint8_t direction = static_cast<uint8_t>(seed & 0x7U);

  switch (direction) {
    case 0:
      startRow = static_cast<int>((seed >> 8) % kMatrixRows);
      startColumn = 0;
      targetRow = static_cast<int>((seed >> 16) % kMatrixRows);
      targetColumn = static_cast<int>(kMatrixColumns - 2U - ((seed >> 20) % 2U));
      break;
    case 1:
      startRow = static_cast<int>((seed >> 8) % kMatrixRows);
      startColumn = kMatrixColumns - 1;
      targetRow = static_cast<int>((seed >> 16) % kMatrixRows);
      targetColumn = static_cast<int>(1U + ((seed >> 20) % 2U));
      break;
    case 2:
      startRow = 0;
      startColumn = static_cast<int>((seed >> 8) % kMatrixColumns);
      targetRow = static_cast<int>(kMatrixRows - 2U - ((seed >> 18) % 2U));
      targetColumn = static_cast<int>((seed >> 22) % kMatrixColumns);
      break;
    case 3:
      startRow = kMatrixRows - 1;
      startColumn = static_cast<int>((seed >> 8) % kMatrixColumns);
      targetRow = static_cast<int>(1U + ((seed >> 18) % 2U));
      targetColumn = static_cast<int>((seed >> 22) % kMatrixColumns);
      break;
    case 4:
      startRow = 0;
      startColumn = 0;
      targetRow = static_cast<int>(kMatrixRows - 2U - ((seed >> 18) % 2U));
      targetColumn = static_cast<int>(kMatrixColumns - 2U - ((seed >> 22) % 2U));
      break;
    case 5:
      startRow = 0;
      startColumn = kMatrixColumns - 1;
      targetRow = static_cast<int>(kMatrixRows - 2U - ((seed >> 18) % 2U));
      targetColumn = static_cast<int>(1U + ((seed >> 22) % 2U));
      break;
    case 6:
      startRow = kMatrixRows - 1;
      startColumn = 0;
      targetRow = static_cast<int>(1U + ((seed >> 18) % 2U));
      targetColumn = static_cast<int>(kMatrixColumns - 2U - ((seed >> 22) % 2U));
      break;
    default:
      startRow = kMatrixRows - 1;
      startColumn = kMatrixColumns - 1;
      targetRow = static_cast<int>(1U + ((seed >> 18) % 2U));
      targetColumn = static_cast<int>(1U + ((seed >> 22) % 2U));
      break;
  }
}

void selectCardinalLightningEndpoints(uint32_t seed, int &startRow, int &startColumn, int &targetRow, int &targetColumn) {
  const uint8_t direction = static_cast<uint8_t>(seed & 0x3U);

  switch (direction) {
    case 0:
      startRow = static_cast<int>((seed >> 8) % kMatrixRows);
      startColumn = 0;
      targetRow = static_cast<int>((seed >> 16) % kMatrixRows);
      targetColumn = static_cast<int>(kMatrixColumns - 2U - ((seed >> 20) % 2U));
      break;
    case 1:
      startRow = static_cast<int>((seed >> 8) % kMatrixRows);
      startColumn = kMatrixColumns - 1;
      targetRow = static_cast<int>((seed >> 16) % kMatrixRows);
      targetColumn = static_cast<int>(1U + ((seed >> 20) % 2U));
      break;
    case 2:
      startRow = 0;
      startColumn = static_cast<int>((seed >> 8) % kMatrixColumns);
      targetRow = static_cast<int>(kMatrixRows - 2U - ((seed >> 18) % 2U));
      targetColumn = static_cast<int>((seed >> 22) % kMatrixColumns);
      break;
    default:
      startRow = kMatrixRows - 1;
      startColumn = static_cast<int>((seed >> 8) % kMatrixColumns);
      targetRow = static_cast<int>(1U + ((seed >> 18) % 2U));
      targetColumn = static_cast<int>((seed >> 22) % kMatrixColumns);
      break;
  }
}

uint8_t traceLightningPath(int startRow, int startColumn, int targetRow, int targetColumn, uint32_t seed, uint8_t maxPoints, LightningPoint *points) {
  if (maxPoints == 0U) {
    return 0U;
  }

  int row = constrain(startRow, 0, kMatrixRows - 1);
  int column = constrain(startColumn, 0, kMatrixColumns - 1);
  points[0] = {row, column};
  uint8_t count = 1U;

  for (uint8_t step = 1U; step < maxPoints; ++step) {
    if (row == targetRow && column == targetColumn && count > 3U) {
      break;
    }

    const uint32_t noise = hash32(seed + static_cast<uint32_t>(step) * 131U + static_cast<uint32_t>(row * 17 + column * 29));
    const int deltaRow = targetRow - row;
    const int deltaColumn = targetColumn - column;
    int rowStep = 0;
    int columnStep = 0;
    const bool preferColumn = abs(deltaColumn) > abs(deltaRow) || (abs(deltaColumn) == abs(deltaRow) && ((noise & 0x1U) == 0U));

    if (preferColumn && deltaColumn != 0) {
      columnStep = deltaColumn > 0 ? 1 : -1;
      if (deltaRow != 0 && ((noise >> 3) & 0x3U) != 0U) {
        rowStep = deltaRow > 0 ? 1 : -1;
      }
    } else if (deltaRow != 0) {
      rowStep = deltaRow > 0 ? 1 : -1;
      if (deltaColumn != 0 && ((noise >> 5) & 0x3U) != 0U) {
        columnStep = deltaColumn > 0 ? 1 : -1;
      }
    }

    if (((noise >> 8) & 0x7U) == 0U) {
      if (rowStep == 0 && row > 0 && row + 1 < kMatrixRows) {
        rowStep = ((noise >> 12) & 0x1U) == 0U ? -1 : 1;
      } else if (columnStep == 0 && column > 0 && column + 1 < kMatrixColumns) {
        columnStep = ((noise >> 13) & 0x1U) == 0U ? -1 : 1;
      }
    }

    if (rowStep == 0 && columnStep == 0) {
      if (deltaColumn != 0) {
        columnStep = deltaColumn > 0 ? 1 : -1;
      } else if (deltaRow != 0) {
        rowStep = deltaRow > 0 ? 1 : -1;
      } else {
        break;
      }
    }

    const int nextRow = constrain(row + rowStep, 0, kMatrixRows - 1);
    const int nextColumn = constrain(column + columnStep, 0, kMatrixColumns - 1);
    if (nextRow == row && nextColumn == column) {
      break;
    }

    row = nextRow;
    column = nextColumn;
    points[count++] = {row, column};
  }

  return count;
}

void addLightningGlow(int row, int column, const RgbColor &coreColor, const RgbColor &glowColor, uint8_t glowScale) {
  addMatrixPixel(static_cast<uint8_t>(row), static_cast<uint8_t>(column), coreColor);

  if (glowScale == 0U) {
    return;
  }

  const uint32_t noise = hash32(static_cast<uint32_t>((row + 1) * 97 + (column + 1) * 193 + glowScale * 17));
  if ((noise & 0x3U) != 0U) {
    return;
  }

  const RgbColor sideSpark = scaleColor(glowColor, glowScale);
  if ((noise & 0x4U) == 0U) {
    const int offsetRow = ((noise >> 3) & 0x1U) == 0U ? -1 : 1;
    addMatrixPixel(static_cast<uint8_t>(row + offsetRow), static_cast<uint8_t>(column), sideSpark);
    return;
  }

  const int offsetColumn = ((noise >> 3) & 0x1U) == 0U ? -1 : 1;
  addMatrixPixel(static_cast<uint8_t>(row), static_cast<uint8_t>(column + offsetColumn), sideSpark);
}

void renderLightningSegment(const LightningPoint &start, const LightningPoint &end, const RgbColor &coreColor, const RgbColor &glowColor, uint8_t glowScale) {
  int row = start.row;
  int column = start.column;
  const int deltaRow = abs(end.row - start.row);
  const int stepRow = start.row < end.row ? 1 : -1;
  const int deltaColumn = -abs(end.column - start.column);
  const int stepColumn = start.column < end.column ? 1 : -1;
  int error = deltaRow + deltaColumn;

  while (true) {
    addLightningGlow(row, column, coreColor, glowColor, glowScale);
    if (row == end.row && column == end.column) {
      break;
    }

    const int doubledError = error * 2;
    if (doubledError >= deltaColumn) {
      error += deltaColumn;
      row += stepRow;
    }
    if (doubledError <= deltaRow) {
      error += deltaRow;
      column += stepColumn;
    }
  }
}

void renderLightningPatternFlash(
  uint32_t now,
  uint32_t cycleMs,
  uint32_t buildMs,
  uint32_t holdMs,
  uint32_t fadeMs,
  uint32_t seedSalt,
  const LightningPatternSegment *segments,
  uint8_t segmentCount,
  uint8_t glowScale
) {
  clearMatrixFrame();
  if (segmentCount == 0U) {
    return;
  }

  const uint32_t activeMs = buildMs + holdMs + fadeMs;
  const uint32_t local = now % cycleMs;
  if (local >= activeMs) {
    return;
  }

  const uint32_t eventIndex = now / cycleMs;
  const uint32_t seed = hash32(eventIndex * seedSalt + 887U);
  const RgbColor strikeColor = blendColors(lightningStrikeColor(seed), colorWheel(static_cast<uint8_t>((seed >> 3) & 0xFFU)), 92);
  const RgbColor coreColor = blendColors(strikeColor, makeColor(255, 255, 255), 196);

  uint8_t visibleSegmentCount = segmentCount;
  if (buildMs > 0U && local < buildMs) {
    const float buildProgress = static_cast<float>(local) / static_cast<float>(buildMs);
    visibleSegmentCount = max<uint8_t>(1U, static_cast<uint8_t>(1U + buildProgress * static_cast<float>(segmentCount - 1U)));
  }

  uint8_t intensityScale = 255U;
  if (local >= buildMs + holdMs) {
    const float fadeProgress = static_cast<float>(local - buildMs - holdMs) / static_cast<float>(fadeMs);
    const float fadeCurve = powf(max(0.0f, 1.0f - fadeProgress), 1.28f);
    intensityScale = static_cast<uint8_t>(255.0f * fadeCurve + 0.5f);
  }

  for (uint8_t segmentIndex = 0U; segmentIndex < visibleSegmentCount; ++segmentIndex) {
    const LightningPatternSegment &segment = segments[segmentIndex];
    const LightningPoint start = {segment.startRow, segment.startColumn};
    const LightningPoint end = {segment.endRow, segment.endColumn};
    const uint32_t segmentSeed = hash32(seed + segmentIndex * 263U);
    const RgbColor segmentStrike = blendColors(strikeColor, colorWheel(static_cast<uint8_t>(segmentSeed & 0xFFU)), 54);
    const RgbColor scaledStrike = scaleColor(segmentStrike, intensityScale);
    const RgbColor scaledCore = blendColors(scaleColor(coreColor, intensityScale), scaledStrike, 96);
    renderLightningSegment(start, end, scaledCore, scaledStrike, glowScale);
  }

  const LightningPatternSegment &firstSegment = segments[0];
  const LightningPatternSegment &lastSegment = segments[visibleSegmentCount - 1U];
  const uint8_t flashScale = local < buildMs ? 255U : static_cast<uint8_t>(min<uint16_t>(255U, intensityScale + 36U));
  addLightningGlow(firstSegment.startRow, firstSegment.startColumn, scaleColor(coreColor, flashScale), scaleColor(strikeColor, flashScale), static_cast<uint8_t>(glowScale + 8U));
  addLightningGlow(lastSegment.endRow, lastSegment.endColumn, scaleColor(coreColor, flashScale), scaleColor(strikeColor, flashScale), static_cast<uint8_t>(glowScale + 8U));
}

void renderLightningPath(
  const LightningPoint *points, uint8_t pointCount, uint8_t visiblePointCount, const RgbColor &strikeColor, const RgbColor &coreColor, uint8_t intensityScale, uint8_t glowScale
) {
  if (pointCount == 0U || visiblePointCount == 0U) {
    return;
  }

  const uint8_t limitedPoints = min<uint8_t>(visiblePointCount, pointCount);
  for (uint8_t pointIndex = 0U; pointIndex < limitedPoints; ++pointIndex) {
    const uint8_t segmentScale =
      static_cast<uint8_t>((static_cast<uint16_t>(intensityScale) * static_cast<uint16_t>(255U - ((limitedPoints - 1U - pointIndex) * 28U))) / 255U);
    const RgbColor scaledStrike = scaleColor(strikeColor, segmentScale);
    const RgbColor scaledCore = blendColors(scaledStrike, scaleColor(coreColor, segmentScale), 160);
    addLightningGlow(points[pointIndex].row, points[pointIndex].column, scaledCore, scaledStrike, glowScale);

    if (pointIndex + 1U < limitedPoints) {
      renderLightningSegment(points[pointIndex], points[pointIndex + 1U], scaledCore, scaledStrike, glowScale);
    }
  }
}

void renderMatrixLightning(uint32_t now) {
  clearMatrixFrame();

  const uint32_t cycleMs = 1460U;
  const uint32_t buildMs = 210U;
  const uint32_t activeMs = 1260U;
  const uint32_t local = now % cycleMs;
  if (local >= activeMs) {
    return;
  }

  const uint32_t eventIndex = now / cycleMs;
  const uint32_t seed = hash32(eventIndex * 1877U + 311U);
  const RgbColor strikeColor = lightningStrikeColor(seed);
  const RgbColor coreColor = blendColors(strikeColor, makeColor(255, 255, 255), 188);

  int startRow = 0;
  int startColumn = 0;
  int targetRow = 0;
  int targetColumn = 0;
  selectLightningEndpoints(seed, startRow, startColumn, targetRow, targetColumn);

  LightningPoint trunk[12];
  const uint8_t trunkCount = traceLightningPath(startRow, startColumn, targetRow, targetColumn, seed, 12U, trunk);
  if (trunkCount == 0U) {
    return;
  }

  const bool building = local < buildMs;
  const float buildProgress = building ? (static_cast<float>(local) / static_cast<float>(buildMs)) : 1.0f;
  float fadeProgress = 0.0f;
  if (!building) {
    fadeProgress = static_cast<float>(local - buildMs) / static_cast<float>(activeMs - buildMs);
  }

  const float fadeCurve = building ? 1.0f : powf(max(0.0f, 1.0f - fadeProgress), 1.35f);
  const uint8_t strikeScale = static_cast<uint8_t>(255.0f * fadeCurve + 0.5f);
  const uint8_t trunkVisible =
    building ? max<uint8_t>(2U, static_cast<uint8_t>(1U + buildProgress * static_cast<float>(trunkCount - 1U))) : trunkCount;

  renderLightningPath(trunk, trunkCount, trunkVisible, strikeColor, coreColor, strikeScale, 18U);

  const uint8_t branchCount = 2U + static_cast<uint8_t>((seed >> 14) & 0x1U);
  for (uint8_t branchIndex = 0U; branchIndex < branchCount; ++branchIndex) {
    if (trunkCount < 4U) {
      break;
    }

    const uint32_t branchSeed = hash32(seed + branchIndex * 719U + 97U);
    const uint8_t anchorIndex = 1U + static_cast<uint8_t>(branchSeed % (trunkCount - 2U));
    if (anchorIndex >= trunkVisible) {
      continue;
    }

    const LightningPoint anchor = trunk[anchorIndex];
    const LightningPoint next = trunk[min<uint8_t>(anchorIndex + 1U, trunkCount - 1U)];
    const int trunkDeltaRow = next.row - anchor.row;
    const int trunkDeltaColumn = next.column - anchor.column;
    int branchTargetRow = anchor.row;
    int branchTargetColumn = anchor.column;

    if (abs(trunkDeltaColumn) >= abs(trunkDeltaRow)) {
      branchTargetRow += ((branchSeed >> 5) & 0x1U) == 0U ? -2 - static_cast<int>((branchSeed >> 9) % 2U) : 2 + static_cast<int>((branchSeed >> 9) % 2U);
      branchTargetColumn += trunkDeltaColumn == 0 ? (((branchSeed >> 7) & 0x1U) == 0U ? -1 : 1) : (trunkDeltaColumn > 0 ? 1 : -1) * (1 + static_cast<int>((branchSeed >> 11) % 2U));
    } else {
      branchTargetColumn += ((branchSeed >> 5) & 0x1U) == 0U ? -2 - static_cast<int>((branchSeed >> 9) % 2U) : 2 + static_cast<int>((branchSeed >> 9) % 2U);
      branchTargetRow += trunkDeltaRow == 0 ? (((branchSeed >> 7) & 0x1U) == 0U ? -1 : 1) : (trunkDeltaRow > 0 ? 1 : -1) * (1 + static_cast<int>((branchSeed >> 11) % 2U));
    }

    branchTargetRow = constrain(branchTargetRow, 0, kMatrixRows - 1);
    branchTargetColumn = constrain(branchTargetColumn, 0, kMatrixColumns - 1);
    if (branchTargetRow == anchor.row && branchTargetColumn == anchor.column) {
      continue;
    }

    LightningPoint branch[6];
    const uint8_t branchPathCount = traceLightningPath(anchor.row, anchor.column, branchTargetRow, branchTargetColumn, branchSeed, 6U, branch);
    if (branchPathCount < 2U) {
      continue;
    }

    uint8_t branchVisible = branchPathCount;
    if (building) {
      const float anchorProgress = static_cast<float>(anchorIndex) / static_cast<float>(max<uint8_t>(1U, trunkCount - 1U));
      if (buildProgress <= anchorProgress) {
        continue;
      }

      const float localBranchProgress = min(1.0f, (buildProgress - anchorProgress) / max(0.12f, 1.0f - anchorProgress));
      branchVisible = max<uint8_t>(2U, static_cast<uint8_t>(1U + localBranchProgress * static_cast<float>(branchPathCount - 1U)));
    }

    const uint8_t branchScale = static_cast<uint8_t>((static_cast<uint16_t>(strikeScale) * static_cast<uint16_t>(180U - branchIndex * 26U)) / 255U);
    const RgbColor branchStrike = blendColors(strikeColor, colorWheel(static_cast<uint8_t>(branchSeed & 0xFFU)), 88);
    const RgbColor branchCore = blendColors(branchStrike, makeColor(255, 255, 255), 176);
    renderLightningPath(branch, branchPathCount, branchVisible, branchStrike, branchCore, branchScale, 10U);
  }

  const uint8_t flashScale = building ? static_cast<uint8_t>(160U + buildProgress * 95.0f) : static_cast<uint8_t>(120.0f * fadeCurve + 0.5f);
  addLightningGlow(trunk[0].row, trunk[0].column, scaleColor(makeColor(255, 255, 255), flashScale), scaleColor(strikeColor, flashScale), 36U);
  addLightningGlow(trunk[trunkVisible - 1U].row, trunk[trunkVisible - 1U].column, scaleColor(coreColor, flashScale), scaleColor(strikeColor, flashScale), 28U);
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

void renderDiagonalSpectrumSweep(uint8_t directionIndex, float progress, const RgbColor &sweepColor, const RgbColor &coreColor) {
  const float maxColumn = static_cast<float>(kMatrixColumns - 1U);
  const float maxRow = static_cast<float>(kMatrixRows - 1U);
  const float diagonalOverscan = 2.0f;
  float entryX = 0.0f;
  float entryY = 0.0f;
  float exitX = maxColumn;
  float exitY = maxRow;

  if (directionIndex == 5U) {
    entryX = maxColumn;
    entryY = maxRow;
    exitX = 0.0f;
    exitY = 0.0f;
  } else if (directionIndex == 6U) {
    entryX = maxColumn;
    entryY = 0.0f;
    exitX = 0.0f;
    exitY = maxRow;
  } else if (directionIndex == 7U) {
    entryX = 0.0f;
    entryY = maxRow;
    exitX = maxColumn;
    exitY = 0.0f;
  }

  const float deltaX = exitX - entryX;
  const float deltaY = exitY - entryY;
  const float motionLength = sqrtf(deltaX * deltaX + deltaY * deltaY);
  const float motionX = deltaX / motionLength;
  const float motionY = deltaY / motionLength;
  const float startX = entryX - motionX * diagonalOverscan;
  const float startY = entryY - motionY * diagonalOverscan;
  const float endX = exitX + motionX * diagonalOverscan;
  const float endY = exitY + motionY * diagonalOverscan;
  const float headX = startX + (endX - startX) * progress;
  const float headY = startY + (endY - startY) * progress;
  const float perpendicularX = -motionY;
  const float perpendicularY = motionX;
  const uint8_t trailLevels[] = {255U, 208U, 164U, 124U, 88U, 58U, 34U};

  for (uint8_t trailIndex = 0; trailIndex < sizeof(trailLevels); ++trailIndex) {
    const float distance = static_cast<float>(trailIndex) * 1.18f;
    const float x = headX - motionX * distance;
    const float y = headY - motionY * distance;
    const RgbColor strokeColor = trailIndex == 0U ? coreColor : scaleColor(sweepColor, trailLevels[trailIndex]);
    addMatrixPixel(static_cast<uint8_t>(lroundf(y)), static_cast<uint8_t>(lroundf(x)), strokeColor);

    if (trailIndex > 4U) {
      continue;
    }

    addMatrixPixel(
      static_cast<uint8_t>(lroundf(y + perpendicularY * 0.72f)), static_cast<uint8_t>(lroundf(x + perpendicularX * 0.72f)), scaleColor(strokeColor, 84)
    );
    addMatrixPixel(
      static_cast<uint8_t>(lroundf(y - perpendicularY * 0.72f)), static_cast<uint8_t>(lroundf(x - perpendicularX * 0.72f)), scaleColor(strokeColor, 84)
    );
  }
}

void renderCardinalSpectrumSweep(uint8_t directionIndex, float progress, const RgbColor &sweepColor, const RgbColor &coreColor) {
  const uint8_t trailLevels[] = {255U, 176U, 104U, 48U};

  if (directionIndex < 2U) {
    const float startColumn = directionIndex == 0U ? -1.5f : static_cast<float>(kMatrixColumns) + 0.5f;
    const float endColumn = directionIndex == 0U ? static_cast<float>(kMatrixColumns) + 0.5f : -1.5f;
    const int motion = directionIndex == 0U ? 1 : -1;
    const float headColumn = startColumn + (endColumn - startColumn) * progress;

    for (uint8_t trailIndex = 0; trailIndex < sizeof(trailLevels); ++trailIndex) {
      const int column = lroundf(headColumn - static_cast<float>(motion * static_cast<int>(trailIndex)));
      const RgbColor strokeColor = trailIndex == 0U ? coreColor : scaleColor(sweepColor, trailLevels[trailIndex]);
      for (uint8_t row = 0; row < kMatrixRows; ++row) {
        addMatrixPixel(row, static_cast<uint8_t>(column), strokeColor);
      }
    }
    return;
  }

  const float startRow = directionIndex == 2U ? -1.5f : static_cast<float>(kMatrixRows) + 0.5f;
  const float endRow = directionIndex == 2U ? static_cast<float>(kMatrixRows) + 0.5f : -1.5f;
  const int motion = directionIndex == 2U ? 1 : -1;
  const float headRow = startRow + (endRow - startRow) * progress;

  for (uint8_t trailIndex = 0; trailIndex < sizeof(trailLevels); ++trailIndex) {
    const int row = lroundf(headRow - static_cast<float>(motion * static_cast<int>(trailIndex)));
    const RgbColor strokeColor = trailIndex == 0U ? coreColor : scaleColor(sweepColor, trailLevels[trailIndex]);
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      addMatrixPixel(static_cast<uint8_t>(row), column, strokeColor);
    }
  }
}

void renderMatrixSpectrumScan(uint32_t now) {
  clearMatrixFrame();

  const uint32_t cycleMs = 980U;
  const uint32_t activeMs = 820U;
  const uint32_t eventIndex = now / cycleMs;
  const uint32_t local = now % cycleMs;

  if (local >= activeMs) {
    return;
  }

  const uint32_t seed = hash32(eventIndex * 1181U + 97U);
  const uint8_t directionIndex = static_cast<uint8_t>(seed & 0x3U);
  const float progress = static_cast<float>(local) / static_cast<float>(activeMs);
  const RgbColor sweepColor = sweepEventColor(seed);
  const RgbColor coreColor = blendColors(sweepColor, makeColor(255, 255, 255), 180);
  renderCardinalSpectrumSweep(directionIndex, progress, sweepColor, coreColor);
}

void renderMatrixSpectrumStorm(uint32_t now) {
  clearMatrixFrame();

  const uint32_t cycleMs = 760U;
  const uint32_t lineStepMs = 14U;
  const uint32_t fullHoldMs = 72U;
  const uint32_t fadeMs = 280U;
  const uint32_t local = now % cycleMs;
  const uint32_t eventIndex = now / cycleMs;
  const uint32_t seed = hash32(eventIndex * 1637U + 557U);
  const uint8_t directionIndex = static_cast<uint8_t>(seed & 0x3U);
  const bool columnSweep = directionIndex < 2U;
  const uint8_t lineCount = columnSweep ? kMatrixColumns : kMatrixRows;
  const size_t startOffset = static_cast<size_t>((seed >> 9) % lineCount);
  const bool reverse = directionIndex == 1U || directionIndex == 3U;
  const uint32_t sweepMs = static_cast<uint32_t>(lineCount) * lineStepMs;
  const uint32_t activeMs = sweepMs + fullHoldMs + fadeMs;
  if (local >= activeMs) {
    return;
  }

  const bool sweeping = local < sweepMs;
  const uint8_t visibleLineCount = sweeping
    ? min<uint8_t>(lineCount, static_cast<uint8_t>(local / lineStepMs) + 1U)
    : lineCount;
  const int highlightOrderedIndex = sweeping ? min<int>(lineCount - 1, static_cast<int>(local / lineStepMs)) : -1;
  uint8_t globalScale = 255U;
  if (local >= sweepMs + fullHoldMs) {
    const float fadeProgress = static_cast<float>(local - sweepMs - fullHoldMs) / static_cast<float>(fadeMs);
    const float fadeCurve = powf(max(0.0f, 1.0f - fadeProgress), 1.35f);
    globalScale = static_cast<uint8_t>(255.0f * fadeCurve + 0.5f);
  }

  for (uint8_t orderedIndex = 0; orderedIndex < visibleLineCount; ++orderedIndex) {
    const bool corePulse = static_cast<int>(orderedIndex) == highlightOrderedIndex;
    const size_t lineIndex = wrappedTraversalStep(startOffset, orderedIndex, lineCount, reverse);
    const uint32_t lineSeed = hash32(seed + static_cast<uint32_t>(lineIndex) * 313U + orderedIndex * 47U);
    const RgbColor variedStrikeColor = blendColors(stormEventColor(lineSeed), colorWheel(static_cast<uint8_t>((lineSeed >> 7) & 0xFFU)), 52);
    const RgbColor variedCoreColor = blendColors(variedStrikeColor, makeColor(255, 255, 255), 196);
    const RgbColor lineColor = scaleColor(variedStrikeColor, globalScale);
    const RgbColor coreLineColor = scaleColor(variedCoreColor, globalScale);

    if (columnSweep) {
      const uint8_t column = static_cast<uint8_t>(lineIndex);
      for (uint8_t row = 0; row < kMatrixRows; ++row) {
        setMatrixPixel(row, column, corePulse ? coreLineColor : lineColor);
      }
    } else {
      const uint8_t row = static_cast<uint8_t>(lineIndex);
      for (uint8_t column = 0; column < kMatrixColumns; ++column) {
        setMatrixPixel(row, column, corePulse ? coreLineColor : lineColor);
      }
    }
  }
}

void renderMatrixStormRipple(uint32_t now) {
  clearMatrixFrame();

  const uint32_t cycleMs = 980U;
  const uint32_t buildMs = 170U;
  const uint32_t holdMs = 70U;
  const uint32_t fadeMs = 340U;
  const uint32_t activeMs = buildMs + holdMs + fadeMs;
  const uint32_t local = now % cycleMs;
  if (local >= activeMs) {
    return;
  }

  const uint32_t eventIndex = now / cycleMs;
  const uint32_t seed = hash32(eventIndex * 1999U + 887U);
  const RgbColor strikeColor = stormEventColor(seed);
  const RgbColor coreColor = blendColors(strikeColor, makeColor(255, 255, 255), 208);
  const float buildProgress = min(1.0f, static_cast<float>(local) / static_cast<float>(buildMs));
  const float startX = (static_cast<float>((seed >> 7) % 20U) * 0.5f) - 0.25f;
  const float startY = (static_cast<float>((seed >> 13) % 12U) * 0.5f) - 0.25f;
  const float directionAngle = (static_cast<float>((seed >> 18) & 0xFFU) / 255.0f) * (2.0f * PI);
  const float driftDistance = 0.9f + (static_cast<float>((seed >> 26) & 0x7U) * 0.22f);
  const float centerX = startX + cosf(directionAngle) * driftDistance * buildProgress;
  const float centerY = startY + sinf(directionAngle) * driftDistance * buildProgress;
  const float maxRadius = 4.8f + static_cast<float>((seed >> 22) & 0x7U) * 0.22f;
  const float currentRadius = maxRadius * buildProgress;
  uint8_t intensityScale = 255U;
  if (local >= buildMs + holdMs) {
    const float fadeProgress = static_cast<float>(local - buildMs - holdMs) / static_cast<float>(fadeMs);
    intensityScale = static_cast<uint8_t>(255.0f * powf(max(0.0f, 1.0f - fadeProgress), 1.28f) + 0.5f);
  }

  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const float deltaX = static_cast<float>(column) - centerX;
      const float deltaY = static_cast<float>(row) - centerY;
      const float distance = sqrtf(deltaX * deltaX + deltaY * deltaY);
      const float trailRadius = currentRadius + 0.42f;
      if (distance > trailRadius) {
        continue;
      }

      const float edgeDistance = fabsf(distance - currentRadius);
      const bool frontEdge = edgeDistance < 0.58f && local < buildMs + holdMs;
      const uint8_t pixelScale = frontEdge
        ? intensityScale
        : static_cast<uint8_t>((static_cast<uint16_t>(intensityScale) * static_cast<uint16_t>(distance < currentRadius ? 176U : 92U)) / 255U);
      const uint32_t pixelSeed = hash32(seed + row * 53U + column * 97U);
      const RgbColor pixelColor = blendColors(strikeColor, colorWheel(static_cast<uint8_t>(pixelSeed & 0xFFU)), static_cast<uint8_t>(36U + ((pixelSeed >> 9) & 0x3FU)));
      addLightningGlow(
        row,
        column,
        scaleColor(frontEdge ? coreColor : pixelColor, pixelScale),
        scaleColor(pixelColor, pixelScale),
        frontEdge ? 30U : 12U
      );
    }
  }
}

void renderMatrixStormRaster(uint32_t now) {
  clearMatrixFrame();

  const uint32_t cycleMs = 920U;
  const uint32_t buildMs = 170U;
  const uint32_t holdMs = 60U;
  const uint32_t fadeMs = 320U;
  const uint32_t activeMs = buildMs + holdMs + fadeMs;
  const uint32_t local = now % cycleMs;
  if (local >= activeMs) {
    return;
  }

  const uint32_t eventIndex = now / cycleMs;
  const uint32_t seed = hash32(eventIndex * 2291U + 887U);
  const RgbColor strikeColor = stormEventColor(seed);
  const RgbColor coreColor = blendColors(strikeColor, makeColor(255, 255, 255), 188);
  const size_t startOffset = static_cast<size_t>((seed >> 10) % kMatrixLedCount);
  const bool reverse = ((seed >> 20) & 0x1U) != 0U;
  const uint8_t visibleSteps = min<uint8_t>(
    static_cast<uint8_t>(kMatrixLedCount),
    static_cast<uint8_t>(1U + (min<uint32_t>(local, buildMs) * kMatrixLedCount) / buildMs)
  );
  uint8_t intensityScale = 255U;
  if (local >= buildMs + holdMs) {
    const float fadeProgress = static_cast<float>(local - buildMs - holdMs) / static_cast<float>(fadeMs);
    intensityScale = static_cast<uint8_t>(255.0f * powf(max(0.0f, 1.0f - fadeProgress), 1.24f) + 0.5f);
  }

  for (uint8_t step = 0; step < visibleSteps; ++step) {
    uint8_t row = 0;
    uint8_t column = 0;
    const size_t rasterStep = wrappedTraversalStep(startOffset, step, kMatrixLedCount, reverse);
    rowMajorStepToCoord(rasterStep, row, column);
    const uint32_t pixelSeed = hash32(seed + static_cast<uint32_t>(rasterStep) * 211U);
    const uint8_t stripePulse = static_cast<uint8_t>((pixelSeed >> 7) & 0x7FU);
    const uint8_t tailScale = static_cast<uint8_t>((static_cast<uint16_t>(intensityScale) * static_cast<uint16_t>(132U + stripePulse)) / 255U);
    const bool head = step + kMatrixColumns >= visibleSteps && local < buildMs + holdMs;
    const RgbColor pixelColor = blendColors(strikeColor, colorWheel(static_cast<uint8_t>(pixelSeed & 0xFFU)), static_cast<uint8_t>(38U + ((pixelSeed >> 16) & 0x3FU)));
    addLightningGlow(
      row,
      column,
      scaleColor(head ? coreColor : pixelColor, tailScale),
      scaleColor(pixelColor, tailScale),
      head ? 22U : 8U
    );
  }
}

void renderMatrixPrideWave(uint32_t now) {
  clearMatrixFrame();

  const float phase = static_cast<float>(now) / 230.0f;
  for (uint8_t column = 0; column < kMatrixColumns; ++column) {
    const float columnProgress = static_cast<float>(column) / static_cast<float>(max<uint8_t>(1U, kMatrixColumns - 1U));
    const float primaryFold = sinf(phase + column * 0.72f);
    const float secondaryFold = sinf((phase * 1.33f) - column * 1.18f);
    const float waveOffset = (0.08f + (0.38f * columnProgress)) * ((primaryFold * 0.66f) + (secondaryFold * 0.34f));
    const float clothShade = 0.58f + 0.18f * (0.5f + 0.5f * primaryFold) + 0.18f * (0.5f + 0.5f * secondaryFold);
    const float highlightStrength = max(0.0f, primaryFold) * (0.08f + 0.18f * columnProgress);

    for (uint8_t row = 0; row < kMatrixRows; ++row) {
      const float shiftedRow = static_cast<float>(row) + waveOffset;
      const int stripeIndex = constrain(static_cast<int>(shiftedRow + 0.5f), 0, kMatrixRows - 1);
      const float rowTexture = 0.88f + 0.12f * sinf((phase * 0.74f) + column * 0.31f + row * 0.62f);
      const float brightness = min(1.0f, max(0.42f, clothShade * rowTexture));
      const RgbColor stripeColor = prideStripeColor(static_cast<uint8_t>(stripeIndex));
      const RgbColor shadedColor = scaleColor(stripeColor, static_cast<uint8_t>(brightness * 255.0f + 0.5f));
      const uint8_t whiteMix = static_cast<uint8_t>(min(255.0f, highlightStrength * 255.0f));
      setMatrixPixel(row, column, blendColors(shadedColor, makeColor(255, 255, 255), whiteMix));
    }
  }
}

void renderMatrixHeartThrob(uint32_t now) {
  clearMatrixFrame();

  const uint32_t cycleMs = 1220U;
  const float progress = static_cast<float>(now % cycleMs) / static_cast<float>(cycleMs);
  const float beatA = pulseWindow(progress, 0.18f, 0.10f);
  const float beatB = 0.82f * pulseWindow(progress, 0.34f, 0.08f);
  const float beat = max(beatA, beatB);
  const float afterGlow = 0.36f * pulseWindow(progress, 0.24f, 0.18f) + 0.28f * pulseWindow(progress, 0.40f, 0.16f);
  const float centerX = (kMatrixColumns - 1) * 0.5f;
  const float centerY = (kMatrixRows - 1) * 0.5f;
  const RgbColor baseColor = makeColor(0xb9, 0x1c, 0x1c);
  const RgbColor accentColor = makeColor(0xfb, 0x71, 0x71);
  const RgbColor coreColor = makeColor(255, 255, 255);

  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const float deltaX = static_cast<float>(column) - centerX;
      const float deltaY = static_cast<float>(row) - centerY;
      const float distance = sqrtf(deltaX * deltaX + deltaY * deltaY);
      const float coreMask = max(0.0f, 1.0f - (distance / (1.20f + beat * 0.95f)));
      const float ringRadius = 0.7f + beatA * 2.2f + beatB * 3.0f;
      const float ringMask = max(0.0f, 1.0f - (fabsf(distance - ringRadius) / (0.80f + beat * 0.34f)));
      const float ambient = 0.08f + afterGlow;
      const float intensity = min(1.0f, ambient + coreMask * (0.52f + beat * 0.42f) + ringMask * (0.20f + beat * 0.34f));
      RgbColor pixelColor = scaleColor(baseColor, static_cast<uint8_t>(intensity * 255.0f + 0.5f));
      pixelColor = blendColors(pixelColor, accentColor, static_cast<uint8_t>(min(255.0f, ringMask * 148.0f)));
      pixelColor = blendColors(pixelColor, coreColor, static_cast<uint8_t>(min(255.0f, coreMask * (88.0f + beat * 120.0f))));
      setMatrixPixel(row, column, pixelColor);
    }
  }
}

void renderMatrixBeatWave(uint32_t now) {
  clearMatrixFrame();

  const uint32_t cycleMs = 1260U;
  const float progress = static_cast<float>(now % cycleMs) / static_cast<float>(cycleMs);
  const float beatA = pulseWindow(progress, 0.18f, 0.10f);
  const float beatB = 0.86f * pulseWindow(progress, 0.34f, 0.08f);
  const float beat = max(beatA, beatB);
  const float centerColumn = (kMatrixColumns - 1) * 0.5f;
  const float centerRow = (kMatrixRows - 1) * 0.5f;
  const float maxDistance = centerColumn + 0.5f;
  const float frontA = beatA * maxDistance;
  const float frontB = beatB * maxDistance;
  const RgbColor baseColor = makeColor(0xbe, 0x12, 0x3c);
  const RgbColor accentColor = makeColor(0xf9, 0xa8, 0xd4);
  const RgbColor coreColor = makeColor(255, 255, 255);

  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const float columnDistance = fabsf(static_cast<float>(column) - centerColumn);
      const float rowWeight = max(0.40f, 1.0f - (fabsf(static_cast<float>(row) - centerRow) / (centerRow + 0.8f)));
      const float frontMaskA = max(0.0f, 1.0f - (fabsf(columnDistance - frontA) / 0.95f));
      const float frontMaskB = max(0.0f, 1.0f - (fabsf(columnDistance - frontB) / 0.80f));
      const float fillMask = max(0.0f, 1.0f - (columnDistance / (1.45f + beat * 3.1f)));
      const float intensity = min(1.0f, rowWeight * (0.12f + fillMask * (0.28f + beat * 0.24f) + frontMaskA * 0.34f + frontMaskB * 0.42f));
      RgbColor pixelColor = scaleColor(baseColor, static_cast<uint8_t>(intensity * 255.0f + 0.5f));
      pixelColor = blendColors(pixelColor, accentColor, static_cast<uint8_t>(min(255.0f, (frontMaskA * 72.0f) + (frontMaskB * 110.0f))));
      pixelColor = blendColors(pixelColor, coreColor, static_cast<uint8_t>(min(255.0f, fillMask * beat * 120.0f)));
      setMatrixPixel(row, column, pixelColor);
    }
  }
}

void renderMatrixTunnelBloom(uint32_t now) {
  clearMatrixFrame();

  const float centerX = (kMatrixColumns - 1) * 0.5f;
  const float centerY = (kMatrixRows - 1) * 0.5f;
  const float maxRadius = 6.2f;
  const float front = fmodf(static_cast<float>(now) / 170.0f, maxRadius + 2.3f);
  const float echo = fmodf(front + 2.25f, maxRadius + 2.3f);

  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const float deltaX = static_cast<float>(column) - centerX;
      const float deltaY = static_cast<float>(row) - centerY;
      const float distance = sqrtf(deltaX * deltaX + deltaY * deltaY);
      const float ringA = bandStrength(distance, front, 0.78f);
      const float ringB = bandStrength(distance, echo, 1.02f);
      const float core = radialStrength(distance, 1.35f + (0.25f * sinf(static_cast<float>(now) / 420.0f)));
      const float intensity = min(1.0f, (ringA * 0.78f) + (ringB * 0.42f) + (core * 0.24f));
      const uint8_t hue = static_cast<uint8_t>((now / 16U) + static_cast<uint8_t>(distance * 36.0f) + column * 5U);
      RgbColor color = blendColors(colorWheel(hue), makeColor(255, 90, 200), 104);
      color = scaleColor(color, static_cast<uint8_t>(intensity * 255.0f + 0.5f));
      color = blendColors(color, makeColor(255, 255, 255), static_cast<uint8_t>((core * 120.0f) + (ringA * 80.0f)));
      setMatrixPixel(row, column, color);
    }
  }
}

void renderMatrixShardRain(uint32_t now) {
  clearMatrixFrame();

  const uint32_t cycleMs = 940U;
  for (uint8_t slot = 0; slot < 6U; ++slot) {
    const uint32_t shiftedNow = now + slot * 140U;
    const uint32_t local = shiftedNow % cycleMs;
    const uint32_t eventIndex = shiftedNow / cycleMs;
    const uint32_t seed = hash32(eventIndex * 2711U + slot * 137U);
    const int drift = static_cast<int>((seed >> 6) % 3U) - 1;
    const int startColumn = static_cast<int>((seed >> 12) % kMatrixColumns);
    const int headRow = static_cast<int>(((kMatrixRows + 4U) * min<uint32_t>(local, 720U)) / 720U) - 3;
    const RgbColor shardColor = blendColors(colorWheel(static_cast<uint8_t>(seed & 0xFFU)), makeColor(255, 255, 255), 92);

    for (uint8_t tail = 0; tail < 4U; ++tail) {
      const int row = headRow - static_cast<int>(tail);
      const int column = startColumn + (drift * static_cast<int>(tail));
      if (row < 0 || row >= kMatrixRows || column < 0 || column >= kMatrixColumns) {
        continue;
      }

      const uint8_t scale = static_cast<uint8_t>(255U - tail * 58U);
      addLightningGlow(row, column, scaleColor(blendColors(shardColor, makeColor(255, 255, 255), tail == 0U ? 132 : 36), scale), scaleColor(shardColor, scale), tail == 0U ? 22U : 8U);
    }

    if (local > 720U && local < 860U) {
      const uint8_t splashColumn = static_cast<uint8_t>(constrain(startColumn + drift * 3, 0, kMatrixColumns - 1));
      const uint8_t splashScale = static_cast<uint8_t>(255U - ((local - 720U) * 180U / 140U));
      addMatrixPixel(kMatrixRows - 1U, splashColumn, scaleColor(shardColor, splashScale));
      if (splashColumn > 0U) {
        addMatrixPixel(kMatrixRows - 1U, splashColumn - 1U, scaleColor(shardColor, splashScale / 3U));
      }
      if (splashColumn + 1U < kMatrixColumns) {
        addMatrixPixel(kMatrixRows - 1U, splashColumn + 1U, scaleColor(shardColor, splashScale / 3U));
      }
    }
  }
}

void renderMatrixOrbitDots(uint32_t now) {
  clearMatrixFrame();

  const float centerX = (kMatrixColumns - 1) * 0.5f;
  const float centerY = (kMatrixRows - 1) * 0.5f;
  const float phase = static_cast<float>(now) / 240.0f;
  for (uint8_t orbit = 0; orbit < 3U; ++orbit) {
    const float orbitSpeed = 0.92f + orbit * 0.17f;
    const RgbColor orbitColor = blendColors(colorWheel(static_cast<uint8_t>((now / 14U) + orbit * 72U)), makeColor(255, 255, 255), 84);
    for (uint8_t tail = 0; tail < 5U; ++tail) {
      const float angle = (phase * orbitSpeed) - (tail * (0.30f + orbit * 0.04f)) + orbit * 2.09f;
      const float orbColumn = centerX + cosf(angle) * (1.6f + orbit * 1.15f);
      const float orbRow = centerY + sinf(angle * 1.22f) * (0.9f + orbit * 0.62f);
      const uint8_t scale = static_cast<uint8_t>(220U - tail * 38U);
      addSoftOrb(orbRow, orbColumn, tail == 0U ? 1.25f : 0.82f, orbitColor, scale);
    }
  }
}

void renderMatrixPinwheel(uint32_t now) {
  clearMatrixFrame();

  const float centerX = (kMatrixColumns - 1) * 0.5f;
  const float centerY = (kMatrixRows - 1) * 0.5f;
  const float phase = static_cast<float>(now) / 310.0f;
  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const float deltaX = static_cast<float>(column) - centerX;
      const float deltaY = static_cast<float>(row) - centerY;
      const float distance = sqrtf(deltaX * deltaX + deltaY * deltaY);
      const float angle = atan2f(deltaY, deltaX);
      const float blade = max(0.0f, cosf((angle * 4.0f) - (phase * 2.2f)));
      const float echo = max(0.0f, cosf((angle * 4.0f) - (phase * 2.2f) + 0.78f));
      const float intensity = ((blade * 0.72f) + (echo * 0.28f)) * radialStrength(distance, 5.8f) + (radialStrength(distance, 1.2f) * 0.18f);
      const uint8_t hue = static_cast<uint8_t>(static_cast<int>(phase * 42.0f + angle * 48.0f + distance * 24.0f));
      RgbColor color = blendColors(colorWheel(hue), colorWheel(static_cast<uint8_t>(hue + 72U)), 104);
      setMatrixPixel(row, column, scaleColor(color, static_cast<uint8_t>(min(1.0f, intensity) * 255.0f + 0.5f)));
    }
  }
}

void renderMatrixLatticeBloom(uint32_t now) {
  clearMatrixFrame();

  const float phase = static_cast<float>(now) / 340.0f;
  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const float vertical = max(0.0f, 1.0f - (fabsf(sinf(column * 0.96f + phase)) / 0.58f));
      const float horizontal = max(0.0f, 1.0f - (fabsf(sinf(row * 1.34f - phase * 1.08f)) / 0.58f));
      const float intersection = min(1.0f, vertical * horizontal * 1.35f);
      const float lattice = max(vertical * 0.42f, horizontal * 0.42f);
      const float intensity = min(1.0f, 0.06f + lattice + intersection * 0.42f);
      const uint8_t hue = static_cast<uint8_t>(160U + column * 18U + row * 11U + (now / 21U));
      RgbColor color = blendColors(colorWheel(hue), makeColor(255, 255, 255), static_cast<uint8_t>(intersection * 150.0f));
      setMatrixPixel(row, column, scaleColor(color, static_cast<uint8_t>(intensity * 255.0f + 0.5f)));
    }
  }
}

void renderMatrixSunburst(uint32_t now) {
  clearMatrixFrame();

  const float centerX = ((kMatrixColumns - 1) * 0.5f) + (0.6f * sinf(static_cast<float>(now) / 540.0f));
  const float centerY = (kMatrixRows - 1) * 0.5f;
  const float phase = static_cast<float>(now) / 260.0f;
  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const float deltaX = static_cast<float>(column) - centerX;
      const float deltaY = static_cast<float>(row) - centerY;
      const float distance = sqrtf(deltaX * deltaX + deltaY * deltaY);
      const float angle = atan2f(deltaY, deltaX);
      const float rays = max(0.0f, cosf((angle - phase * 0.55f) * 6.0f));
      const float echo = max(0.0f, cosf((angle + phase * 0.35f) * 3.0f));
      const float intensity = ((rays * 0.72f) + (echo * 0.24f)) * radialStrength(distance, 5.6f) + (radialStrength(distance, 1.15f) * 0.28f);
      RgbColor color = blendColors(makeColor(255, 170, 32), makeColor(255, 70, 0), static_cast<uint8_t>(distance * 28.0f));
      color = blendColors(color, makeColor(255, 255, 255), static_cast<uint8_t>((rays * 80.0f) + (radialStrength(distance, 1.2f) * 120.0f)));
      setMatrixPixel(row, column, scaleColor(color, static_cast<uint8_t>(min(1.0f, intensity) * 255.0f + 0.5f)));
    }
  }
}

void renderMatrixCircuitTrace(uint32_t now) {
  clearMatrixFrame();

  static const LightningPoint kCircuitPath[] = {
    {5, 0}, {4, 0}, {4, 2}, {1, 2}, {1, 4}, {3, 4}, {3, 6}, {0, 6},
    {0, 9}, {2, 9}, {2, 7}, {5, 7}, {5, 5}, {4, 5}, {4, 3}, {5, 3},
  };
  constexpr size_t kCircuitCount = sizeof(kCircuitPath) / sizeof(kCircuitPath[0]);

  for (size_t index = 0; index < kCircuitCount; ++index) {
    addMatrixPixel(
      static_cast<uint8_t>(kCircuitPath[index].row),
      static_cast<uint8_t>(kCircuitPath[index].column),
      scaleColor(makeColor(18, 70, 34), static_cast<uint8_t>(28U + ((index & 1U) ? 16U : 0U)))
    );
  }

  const size_t head = static_cast<size_t>(now / 70U) % kCircuitCount;
  for (uint8_t tail = 0U; tail < 9U; ++tail) {
    const size_t step = (head + kCircuitCount - tail) % kCircuitCount;
    const size_t previous = (step + kCircuitCount - 1U) % kCircuitCount;
    const uint8_t scale = static_cast<uint8_t>(255U - tail * 24U);
    const RgbColor traceColor = blendColors(makeColor(34, 197, 94), colorWheel(static_cast<uint8_t>((now / 16U) + step * 17U)), 86);
    const RgbColor scaledTrace = scaleColor(traceColor, scale);
    const RgbColor scaledCore = blendColors(scaledTrace, makeColor(255, 255, 255), tail == 0U ? 156 : 84);
    renderLightningSegment(kCircuitPath[previous], kCircuitPath[step], scaledCore, scaledTrace, tail == 0U ? 18U : 6U);
    addLightningGlow(kCircuitPath[step].row, kCircuitPath[step].column, scaledCore, scaledTrace, tail == 0U ? 22U : 8U);
  }
}

void renderMatrixVortex(uint32_t now) {
  clearMatrixFrame();

  const float centerX = (kMatrixColumns - 1) * 0.5f;
  const float centerY = (kMatrixRows - 1) * 0.5f;
  const float phase = static_cast<float>(now) / 240.0f;
  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const float deltaX = static_cast<float>(column) - centerX;
      const float deltaY = static_cast<float>(row) - centerY;
      const float distance = sqrtf(deltaX * deltaX + deltaY * deltaY);
      const float angle = atan2f(deltaY, deltaX);
      const float swirl = 0.5f + (0.5f * sinf(distance * 2.6f - phase * 1.9f - angle * 3.4f));
      const float arm = max(0.0f, cosf(angle * 2.0f + phase * 1.15f - distance * 0.95f));
      const float intensity = 0.06f + ((swirl * 0.56f) + (arm * 0.34f)) * radialStrength(distance, 5.8f) + (radialStrength(distance, 0.9f) * 0.22f);
      const uint8_t hue = static_cast<uint8_t>(static_cast<int>(170.0f + angle * 46.0f + (now / 17.0f) - distance * 24.0f));
      RgbColor color = blendColors(colorWheel(hue), colorWheel(static_cast<uint8_t>(hue + 80U)), 108);
      setMatrixPixel(row, column, scaleColor(color, static_cast<uint8_t>(min(1.0f, intensity) * 255.0f + 0.5f)));
    }
  }
}

void renderMatrixSkylinePulse(uint32_t now) {
  clearMatrixFrame();

  const uint32_t tick = now / 150U;
  const float pulse = heartbeatEnvelope(fmodf(static_cast<float>(now) / 1400.0f, 1.0f));
  for (uint8_t column = 0; column < kMatrixColumns; ++column) {
    const uint32_t noise = hash32(tick * 191U + column * 71U);
    const uint8_t height = static_cast<uint8_t>(1U + ((noise >> 5) % kMatrixRows));
    const RgbColor buildingColor = blendColors(colorWheel(static_cast<uint8_t>(180U + column * 11U + (now / 20U))), makeColor(255, 255, 255), 46);

    for (uint8_t level = 0; level < height; ++level) {
      const uint8_t row = static_cast<uint8_t>(kMatrixRows - 1U - level);
      uint8_t scale = static_cast<uint8_t>(88U + level * 22U);
      if (level + 1U == height) {
        scale = static_cast<uint8_t>(150.0f + pulse * 90.0f);
      }
      setMatrixPixel(row, column, scaleColor(buildingColor, scale));
    }
  }
}

void renderMatrixCrystalScan(uint32_t now) {
  clearMatrixFrame();

  const float centerX = (kMatrixColumns - 1) * 0.5f;
  const float centerY = (kMatrixRows - 1) * 0.5f;
  const float front = fmodf(static_cast<float>(now) / 160.0f, 9.0f);
  const float echo = fmodf(front + 2.2f, 9.0f);
  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const float dx = fabsf(static_cast<float>(column) - centerX);
      const float dy = fabsf(static_cast<float>(row) - centerY);
      const float diamond = dx + (dy * 1.35f);
      const float scan = bandStrength(diamond, front, 0.85f);
      const float echoScan = bandStrength(diamond, echo, 1.10f);
      const float facet = bandStrength(fabsf(dx - (dy * 1.3f)), 0.0f, 0.75f);
      const float intensity = min(1.0f, 0.05f + (scan * 0.72f) + (echoScan * 0.38f) + (facet * 0.18f));
      const uint8_t hue = static_cast<uint8_t>(150U + (now / 18U) + column * 12U + row * 20U);
      RgbColor color = blendColors(colorWheel(hue), makeColor(255, 255, 255), static_cast<uint8_t>((scan * 96.0f) + (facet * 54.0f)));
      setMatrixPixel(row, column, scaleColor(color, static_cast<uint8_t>(intensity * 255.0f + 0.5f)));
    }
  }
}

void renderMatrixPulseDiamond(uint32_t now) {
  clearMatrixFrame();

  const float centerX = (kMatrixColumns - 1) * 0.5f;
  const float centerY = (kMatrixRows - 1) * 0.5f;
  const float radius = triangleUnit(static_cast<float>(now % 1700U) / 1700.0f) * 7.4f;
  const float echoRadius = fmodf(radius + 2.4f, 7.4f);
  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const float diamond = fabsf(static_cast<float>(column) - centerX) + fabsf(static_cast<float>(row) - centerY);
      const float edge = bandStrength(diamond, radius, 0.9f);
      const float echo = bandStrength(diamond, echoRadius, 1.15f);
      const float core = radialStrength(diamond, 2.1f);
      const float intensity = min(1.0f, 0.05f + (edge * 0.75f) + (echo * 0.36f) + (core * 0.16f));
      RgbColor color = blendColors(makeColor(34, 211, 238), makeColor(244, 114, 182), static_cast<uint8_t>((diamond / 7.4f) * 180.0f));
      color = blendColors(color, makeColor(255, 255, 255), static_cast<uint8_t>((edge * 86.0f) + (core * 96.0f)));
      setMatrixPixel(row, column, scaleColor(color, static_cast<uint8_t>(intensity * 255.0f + 0.5f)));
    }
  }
}

void renderMatrixCometOrbit(uint32_t now) {
  clearMatrixFrame();

  const float centerX = (kMatrixColumns - 1) * 0.5f;
  const float centerY = (kMatrixRows - 1) * 0.5f;
  for (uint8_t comet = 0U; comet < 2U; ++comet) {
    const float baseAngle = (static_cast<float>(now) / 210.0f) * (1.0f + comet * 0.14f) + comet * PI;
    const RgbColor cometColor = blendColors(colorWheel(static_cast<uint8_t>((now / 12U) + comet * 94U)), makeColor(255, 255, 255), 64);
    for (uint8_t tail = 0U; tail < 8U; ++tail) {
      const float tailAngle = baseAngle - tail * (0.33f + comet * 0.04f);
      const float orbColumn = centerX + cosf(tailAngle) * (2.0f + comet * 1.7f);
      const float orbRow = centerY + sinf(tailAngle * 1.28f) * (1.0f + comet * 0.8f);
      addSoftOrb(orbRow, orbColumn, tail == 0U ? 1.05f : 0.72f, cometColor, static_cast<uint8_t>(220U - tail * 24U));
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

RgbColor twoHueColor(uint8_t firstHue, uint8_t secondHue, float mix, float intensity) {
  return scaleColor(blendColors(colorWheel(firstHue), colorWheel(secondHue), unitToScale(mix)), unitToScale(intensity));
}

void addDirectionalTrail(float centerRow, float centerColumn, float rowStep, float columnStep, uint8_t length, const RgbColor &color, uint8_t headScale, uint8_t tailFloor) {
  if (length == 0U || headScale == 0U) {
    return;
  }

  for (uint8_t tail = 0U; tail < length; ++tail) {
    const float row = centerRow - rowStep * tail;
    const float column = centerColumn - columnStep * tail;
    const uint16_t decay = static_cast<uint16_t>(tail) * 32U;
    const uint8_t scale = tail == 0U ? headScale : max<uint8_t>(tailFloor, static_cast<uint8_t>(headScale > decay ? headScale - decay : tailFloor));
    addMatrixPixel(static_cast<uint8_t>(lroundf(row)), static_cast<uint8_t>(lroundf(column)), scaleColor(color, scale));
  }
}

void renderMatrixAurora(uint32_t now, uint8_t variant) {
  clearMatrixFrame();

  const float phase = static_cast<float>(now) / (variant == 0U ? 430.0f : (variant == 1U ? 320.0f : 260.0f));
  const float foldSpeed = variant == 0U ? 0.66f : (variant == 1U ? 0.98f : 1.25f);
  const uint8_t hueA = variant == 0U ? 88U : (variant == 1U ? 150U : 76U);
  const uint8_t hueB = variant == 0U ? 154U : (variant == 1U ? 228U : 184U);

  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    const float altitude = 1.0f - (static_cast<float>(row) / static_cast<float>(max<uint8_t>(1U, kMatrixRows - 1U)));
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const float curtainA = 0.5f + 0.5f * sinf(column * 0.54f + phase + row * 0.34f);
      const float curtainB = 0.5f + 0.5f * sinf(column * 0.18f - phase * 1.24f + row * 1.08f + variant * 0.9f);
      const float fold = 0.5f + 0.5f * sinf(row * 1.25f - phase * foldSpeed + column * (variant == 1U ? 0.42f : 0.24f));
      const float shimmer = 0.5f + 0.5f * sinf((row + 1.0f) * (column + 1.0f) * 0.17f + phase * 1.8f);
      float intensity = (0.22f + 0.78f * altitude) * (0.28f + 0.72f * curtainA) * (0.30f + 0.70f * fold);
      intensity *= variant == 1U ? (0.66f + 0.34f * curtainB) : (0.54f + 0.46f * shimmer);

      RgbColor color = twoHueColor(
        static_cast<uint8_t>(hueA + row * 7U + (now / 42U)),
        static_cast<uint8_t>(hueB + column * 13U + (now / 58U)),
        variant == 2U ? shimmer : curtainB,
        intensity
      );
      const uint8_t whiteMix = unitToScale((variant == 2U ? 0.24f : 0.12f) * fold * shimmer);
      setMatrixPixel(row, column, blendColors(color, makeColor(255, 255, 255), whiteMix));
    }
  }
}

void renderMatrixTidalFlow(uint32_t now, uint8_t variant) {
  clearMatrixFrame();

  const float phase = static_cast<float>(now) / (variant == 0U ? 260.0f : (variant == 1U ? 180.0f : 210.0f));
  const RgbColor deep = variant == 1U ? makeColor(2, 18, 38) : makeColor(4, 24, 58);
  const RgbColor mid = variant == 2U ? makeColor(18, 122, 160) : makeColor(10, 88, 146);
  const RgbColor surface = variant == 0U ? makeColor(148, 255, 240) : (variant == 1U ? makeColor(77, 180, 255) : makeColor(255, 255, 255));

  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const float primary = 0.5f + 0.5f * sinf(row * 0.72f + column * 0.38f - phase);
      const float secondary = 0.5f + 0.5f * sinf(column * 0.96f + phase * 1.27f - row * (variant == 1U ? 0.58f : 0.24f));
      const float undertow = 0.5f + 0.5f * sinf(row * 1.18f - phase * 1.42f + column * 0.12f);
      const float crest = variant == 0U ? primary : (variant == 1U ? (0.70f * primary + 0.30f * (1.0f - undertow)) : (0.55f * primary + 0.45f * secondary));
      const float foam = bandStrength(crest, 0.92f, variant == 2U ? 0.18f : 0.12f);
      const float intensity = min(1.0f, 0.18f + 0.70f * crest + 0.32f * foam);

      RgbColor color = blendColors(deep, mid, unitToScale(crest));
      color = blendColors(color, surface, unitToScale(foam * (variant == 2U ? 0.86f : 0.64f)));
      setMatrixPixel(row, column, scaleColor(color, unitToScale(intensity)));
    }
  }
}

void renderMatrixGravityField(uint32_t now, uint8_t variant) {
  clearMatrixFrame();

  const float centerX = (kMatrixColumns - 1) * 0.5f;
  const float centerY = (kMatrixRows - 1) * 0.5f;
  const float phase = static_cast<float>(now) / (variant == 2U ? 180.0f : 320.0f);
  const float wellAX = centerX + cosf(phase * 0.84f) * (variant == 0U ? 1.8f : 2.2f);
  const float wellAY = centerY + sinf(phase * 1.12f) * (variant == 0U ? 1.4f : 1.0f);
  const float wellBX = centerX + cosf(phase * 1.37f + PI) * 1.7f;
  const float wellBY = centerY + sinf(phase * 0.96f + PI) * 1.3f;
  const float burstRadius = 1.2f + fmodf(phase * 1.7f, 4.6f);

  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const float deltaAX = static_cast<float>(column) - wellAX;
      const float deltaAY = static_cast<float>(row) - wellAY;
      const float distanceA = sqrtf(deltaAX * deltaAX + deltaAY * deltaAY);
      const float swirlA = 0.5f + 0.5f * sinf(distanceA * 4.2f - phase * 2.3f + atan2f(deltaAY, deltaAX) * (variant == 1U ? 2.0f : 1.0f));
      RgbColor pixelColor = twoHueColor(
        static_cast<uint8_t>(166U + row * 4U + (now / 46U)),
        static_cast<uint8_t>(230U + column * 7U + (now / 62U)),
        swirlA,
        0.22f + 0.72f * radialStrength(distanceA, 4.2f)
      );

      if (variant != 0U) {
        const float deltaBX = static_cast<float>(column) - wellBX;
        const float deltaBY = static_cast<float>(row) - wellBY;
        const float distanceB = sqrtf(deltaBX * deltaBX + deltaBY * deltaBY);
        const float swirlB = 0.5f + 0.5f * sinf(distanceB * 4.6f - phase * 2.6f + atan2f(deltaBY, deltaBX) * 1.8f);
        pixelColor = addColors(
          pixelColor,
          twoHueColor(
            static_cast<uint8_t>(205U + column * 5U + (now / 38U)),
            static_cast<uint8_t>(120U + row * 9U + (now / 54U)),
            swirlB,
            0.18f + 0.60f * radialStrength(distanceB, 3.6f)
          )
        );
      }

      const float centerDeltaX = static_cast<float>(column) - centerX;
      const float centerDeltaY = static_cast<float>(row) - centerY;
      const float centerDistance = sqrtf(centerDeltaX * centerDeltaX + centerDeltaY * centerDeltaY);
      if (variant == 2U) {
        const float burst = bandStrength(centerDistance, burstRadius, 0.55f);
        pixelColor = addColors(pixelColor, scaleColor(blendColors(makeColor(64, 224, 255), makeColor(255, 255, 255), 96), unitToScale(burst)));
      }

      const float shadow = variant == 2U ? radialStrength(centerDistance, 0.92f) : max(radialStrength(distanceA, 0.72f), variant == 0U ? 0.0f : radialStrength(sqrtf(powf(static_cast<float>(column) - wellBX, 2.0f) + powf(static_cast<float>(row) - wellBY, 2.0f)), 0.72f));
      setMatrixPixel(row, column, scaleColor(pixelColor, static_cast<uint8_t>(255U - unitToScale(shadow * 0.84f))));
    }
  }
}

void renderMatrixEmberField(uint32_t now, uint8_t variant) {
  clearMatrixFrame();

  const bool falling = variant == 2U;
  const RgbColor topColor = falling ? makeColor(6, 10, 20) : makeColor(18, 4, 0);
  const RgbColor bottomColor = variant == 1U ? makeColor(76, 18, 0) : (falling ? makeColor(20, 22, 34) : makeColor(52, 8, 0));
  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    const uint8_t gradient = static_cast<uint8_t>((static_cast<uint16_t>(row) * 255U) / max<uint8_t>(1U, kMatrixRows - 1U));
    const RgbColor background = blendColors(topColor, bottomColor, gradient);
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      setMatrixPixel(row, column, background);
    }
  }

  if (variant == 1U) {
    addSoftOrb(kMatrixRows - 0.7f, (kMatrixColumns - 1) * 0.5f + sinf(now / 250.0f) * 1.6f, 3.2f, makeColor(255, 80, 0), 84);
  }

  for (uint8_t particleIndex = 0U; particleIndex < 10U; ++particleIndex) {
    const uint32_t seed = hash32(particleIndex * 811U + variant * 271U);
    const uint32_t cycleMs = falling ? 980U : (variant == 1U ? 740U : 820U);
    const float progress = static_cast<float>((now + (seed & 0x3FFU)) % cycleMs) / static_cast<float>(cycleMs);
    float row = falling ? (-1.0f + progress * (kMatrixRows + 2.0f)) : (static_cast<float>(kMatrixRows) - progress * (kMatrixRows + 2.0f));
    if (variant == 1U) {
      row += 0.55f * sinf(progress * 2.0f * PI * 2.0f + particleIndex * 0.8f);
    }

    const float drift = sinf((now / 180.0f) + particleIndex * 0.9f + ((seed >> 6) & 0x7U)) * (variant == 1U ? 1.6f : 1.0f);
    const float column = static_cast<float>(seed % kMatrixColumns) + drift * (falling ? 0.55f : 1.0f);
    RgbColor color = falling
      ? blendColors(makeColor(255, 170, 84), makeColor(128, 160, 255), unitToScale(progress))
      : blendColors(makeColor(255, 96, 0), makeColor(255, 255, 255), variant == 1U ? 108U : 72U);
    if (variant == 1U) {
      color = blendColors(color, makeColor(255, 210, 120), 96);
    }

    addDirectionalTrail(row, column, falling ? 1.0f : -1.0f, drift * 0.18f, variant == 1U ? 4U : 3U, color, falling ? 150U : 220U, 28U);
    addSoftOrb(row, column, falling ? 0.45f : 0.70f, blendColors(color, makeColor(255, 255, 255), falling ? 24U : 92U), falling ? 108U : 160U);
  }
}

void renderMatrixPrismBurst(uint32_t now, uint8_t variant) {
  clearMatrixFrame();

  const float centerX = (kMatrixColumns - 1) * 0.5f;
  const float centerY = (kMatrixRows - 1) * 0.5f;
  const float phase = static_cast<float>(now) / (variant == 0U ? 210.0f : (variant == 1U ? 170.0f : 140.0f));

  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      float intensity = 0.0f;
      float mix = 0.0f;
      if (variant == 0U) {
        const float deltaX = static_cast<float>(column) + 1.0f;
        const float deltaY = static_cast<float>(row) - (kMatrixRows - 1);
        const float angle = atan2f(deltaY, deltaX);
        const float radius = sqrtf(deltaX * deltaX + deltaY * deltaY);
        const float spokeA = max(0.0f, 1.0f - (wrappedAngleDelta(angle, phase * 0.52f) / 0.34f));
        const float spokeB = max(0.0f, 1.0f - (wrappedAngleDelta(angle, phase * 0.52f + 0.78f) / 0.28f));
        intensity = min(1.0f, (spokeA + spokeB) * (0.36f + 0.64f * radialStrength(radius, 8.8f)));
        mix = 0.5f + 0.5f * sinf(radius * 1.05f - phase * 1.2f);
      } else if (variant == 1U) {
        const float projection = column * 0.82f + row * 0.55f - phase * 2.6f;
        const float slice = 0.5f + 0.5f * sinf(projection);
        const float secondary = 0.5f + 0.5f * sinf(column * 1.1f - row * 0.6f + phase * 1.7f);
        intensity = powf(slice, 1.9f) * (0.45f + 0.55f * secondary);
        mix = secondary;
      } else {
        const float diagonal = column * 0.76f - row * 0.62f + phase * 2.4f;
        const float cascade = 0.5f + 0.5f * sinf(diagonal);
        const float sparkle = 0.5f + 0.5f * sinf(row * 1.2f + phase * 3.2f + column * 0.3f);
        intensity = powf(cascade, 2.2f) * (0.34f + 0.66f * sparkle);
        mix = 0.5f + 0.5f * sinf((row - centerY) * 0.9f + (column - centerX) * 0.3f + phase);
      }

      RgbColor color = twoHueColor(
        static_cast<uint8_t>(12U + column * 14U + (now / 16U)),
        static_cast<uint8_t>(160U + row * 17U + (now / 26U)),
        mix,
        intensity
      );
      const uint8_t whiteMix = unitToScale(intensity * (variant == 2U ? 0.36f : 0.20f));
      setMatrixPixel(row, column, blendColors(color, makeColor(255, 255, 255), whiteMix));
    }
  }
}

void renderMatrixSwarmEffect(uint32_t now, uint8_t variant) {
  fillMatrixFrame(scaleColor(matrixEffectBaseColor(), 5));

  const float centerX = (kMatrixColumns - 1) * 0.5f;
  const float centerY = (kMatrixRows - 1) * 0.5f;
  const float phase = static_cast<float>(now) / (variant == 2U ? 130.0f : 220.0f);
  const uint8_t agentCount = variant == 2U ? 7U : 6U;

  for (uint8_t agent = 0U; agent < agentCount; ++agent) {
    const uint32_t seed = hash32(agent * 911U + variant * 313U);
    float column = centerX;
    float row = centerY;
    if (variant == 0U) {
      column = centerX + sinf(phase * 1.2f + agent * 1.1f) * (2.0f + ((seed >> 6) & 0x3U) * 0.15f) + cosf(phase * 0.7f + agent) * 0.7f;
      row = centerY + cosf(phase * 1.5f + agent * 0.9f) * (1.7f + ((seed >> 10) & 0x3U) * 0.12f);
    } else if (variant == 1U) {
      const float progress = fmodf(phase * 0.22f + agent * 0.17f, 1.0f);
      column = -1.0f + progress * (kMatrixColumns + 2.0f);
      row = centerY + sinf(progress * 2.0f * PI * 2.0f + agent) * 2.0f + 0.6f * cosf(phase + agent);
    } else {
      const float progress = fmodf(static_cast<float>(now + agent * 80U) / (620.0f + agent * 24.0f), 1.0f);
      const float radius = triangleUnit(progress) * 4.5f;
      const float angle = agent * 1.1f + phase * 0.9f;
      column = centerX + cosf(angle) * radius;
      row = centerY + sinf(angle * 1.18f) * radius * 0.68f;
    }

    const RgbColor color = blendColors(colorWheel(static_cast<uint8_t>((now / 18U) + agent * 43U)), colorWheel(static_cast<uint8_t>(80U + agent * 29U)), variant == 1U ? 144U : 92U);
    addSoftOrb(row, column, variant == 2U ? 0.95f : 1.25f, color, variant == 2U ? 124U : 104U);
    addSoftOrb(row, column, 0.42f, blendColors(color, makeColor(255, 255, 255), 132), 170U);
  }
}

void renderMatrixSolarField(uint32_t now, uint8_t variant) {
  clearMatrixFrame();

  const float centerX = (kMatrixColumns - 1) * 0.5f;
  const float centerY = (kMatrixRows - 1) * 0.5f;
  const float phase = static_cast<float>(now) / 250.0f;
  const float moonX = centerX + sinf(phase * 0.7f) * 1.2f;
  const float moonY = centerY + cosf(phase * 0.51f) * 0.6f;

  for (uint8_t row = 0; row < kMatrixRows; ++row) {
    for (uint8_t column = 0; column < kMatrixColumns; ++column) {
      const float deltaX = static_cast<float>(column) - centerX;
      const float deltaY = static_cast<float>(row) - centerY;
      const float angle = atan2f(deltaY, deltaX);
      const float distance = sqrtf(deltaX * deltaX + deltaY * deltaY);
      float intensity = 0.0f;
      RgbColor color = makeColor(0, 0, 0);

      if (variant == 0U) {
        const float core = radialStrength(distance, 2.9f);
        const float rays = max(0.0f, 1.0f - (wrappedAngleDelta(angle, phase * 0.94f + sinf(distance * 0.8f) * 0.24f) / 0.20f));
        const float flare = 0.5f + 0.5f * sinf(angle * 4.0f - phase * 1.8f + distance * 1.1f);
        intensity = min(1.0f, core * 0.82f + rays * (1.0f - distance / 6.0f) * 0.90f + flare * 0.18f);
        color = blendColors(makeColor(255, 96, 0), makeColor(255, 228, 96), unitToScale(core));
      } else if (variant == 1U) {
        const float ring = bandStrength(distance, 1.8f + 0.22f * sinf(phase * 1.8f), 0.55f);
        const float spoke = max(0.0f, 1.0f - (wrappedAngleDelta(angle, phase * 1.3f + distance * 0.12f) / 0.18f));
        const float coreShadow = radialStrength(distance, 1.0f);
        intensity = min(1.0f, ring * 0.94f + spoke * (1.0f - coreShadow) * 0.52f);
        color = blendColors(makeColor(255, 108, 0), makeColor(255, 255, 255), unitToScale(ring * 0.60f));
      } else {
        const float moonDeltaX = static_cast<float>(column) - moonX;
        const float moonDeltaY = static_cast<float>(row) - moonY;
        const float moonDistance = sqrtf(moonDeltaX * moonDeltaX + moonDeltaY * moonDeltaY);
        const float corona = bandStrength(distance, 2.6f, 0.65f);
        const float rim = bandStrength(moonDistance, 1.62f, 0.42f);
        const float shadow = radialStrength(moonDistance, 1.46f);
        intensity = min(1.0f, corona * 0.72f + rim * 0.98f);
        color = blendColors(makeColor(255, 120, 0), makeColor(96, 160, 255), unitToScale(rim));
        color = scaleColor(color, static_cast<uint8_t>(255U - unitToScale(shadow * 0.92f)));
      }

      const uint8_t whiteMix = unitToScale(intensity * (variant == 0U ? 0.22f : 0.14f));
      setMatrixPixel(row, column, blendColors(scaleColor(color, unitToScale(intensity)), makeColor(255, 255, 255), whiteMix));
    }
  }
}

void renderMatrixDataFlow(uint32_t now, uint8_t variant) {
  fillMatrixFrame(makeColor(0, variant == 2U ? 8 : 14, variant == 1U ? 10 : 4));

  if (variant == 0U) {
    for (uint8_t column = 0U; column < kMatrixColumns; ++column) {
      const uint32_t seed = hash32(column * 277U + 17U);
      const uint32_t cycleMs = 420U + (seed & 0x1FFU);
      const float head = (static_cast<float>((now + ((seed >> 9) & 0x3FFU)) % cycleMs) / static_cast<float>(cycleMs)) * (kMatrixRows + 4.0f) - 2.0f;
      const RgbColor streamColor = blendColors(makeColor(30, 255, 144), makeColor(120, 255, 255), static_cast<uint8_t>(seed & 0x7FU));
      for (uint8_t row = 0U; row < kMatrixRows; ++row) {
        const float distance = head - row;
        if (distance >= 0.0f && distance < 4.2f) {
          const float intensity = 1.0f - (distance / 4.2f);
          setMatrixPixel(row, column, scaleColor(streamColor, unitToScale(intensity)));
        } else if ((hash32((now / 120U) * 131U + column * 37U + row * 17U) & 0x1FU) == 0U) {
          addMatrixPixel(row, column, scaleColor(streamColor, 38));
        }
      }
    }
    return;
  }

  if (variant == 1U) {
    for (uint8_t row = 0U; row < kMatrixRows; ++row) {
      const uint32_t seed = hash32(row * 401U + 97U);
      const uint32_t cycleMs = 420U + (seed & 0xFFU);
      const float head = (static_cast<float>((now + ((seed >> 11) & 0x3FFU)) % cycleMs) / static_cast<float>(cycleMs)) * (kMatrixColumns + 5.0f) - 2.0f;
      const RgbColor streamColor = blendColors(makeColor(0, 255, 220), makeColor(120, 255, 150), static_cast<uint8_t>(seed & 0x7FU));
      for (uint8_t column = 0U; column < kMatrixColumns; ++column) {
        const float distance = head - column;
        if (distance >= 0.0f && distance < 4.5f) {
          const float intensity = 1.0f - (distance / 4.5f);
          setMatrixPixel(row, column, scaleColor(streamColor, unitToScale(intensity)));
        }
      }
    }
    return;
  }

  const float centerX = (kMatrixColumns - 1) * 0.5f;
  const float centerY = (kMatrixRows - 1) * 0.5f;
  const float packetFront = fmodf(static_cast<float>(now) / 110.0f, static_cast<float>(kMatrixRows + kMatrixColumns));
  for (uint8_t row = 0U; row < kMatrixRows; ++row) {
    for (uint8_t column = 0U; column < kMatrixColumns; ++column) {
      const float manhattan = fabsf(static_cast<float>(row) - centerY) + fabsf(static_cast<float>(column) - centerX);
      const float packet = bandStrength(manhattan, packetFront, 0.9f);
      const float grid = max(0.0f, 1.0f - fabsf(sinf(column * 1.6f + now / 90.0f) * cosf(row * 1.4f - now / 120.0f)));
      const uint32_t noise = hash32((now / 80U) * 193U + row * 47U + column * 89U);
      RgbColor color = blendColors(makeColor(0, 255, 190), makeColor(96, 180, 255), unitToScale(grid));
      color = scaleColor(color, unitToScale(min(1.0f, packet * 0.92f + ((noise & 0x7U) == 0U ? 0.14f : 0.0f))));
      setMatrixPixel(row, column, color);
    }
  }
}

void renderMatrixDuneFlow(uint32_t now, uint8_t variant) {
  clearMatrixFrame();

  const float phase = static_cast<float>(now) / (variant == 0U ? 360.0f : (variant == 1U ? 160.0f : 280.0f));
  for (uint8_t row = 0U; row < kMatrixRows; ++row) {
    for (uint8_t column = 0U; column < kMatrixColumns; ++column) {
      const float ridge = 0.5f + 0.5f * sinf(column * 0.72f - phase + row * 0.38f);
      const float secondary = 0.5f + 0.5f * sinf(column * 0.24f + phase * 0.58f - row * 0.82f);
      const float gust = variant == 1U ? (0.5f + 0.5f * sinf(column * 1.42f + row * 0.94f - phase * 2.4f)) : 0.0f;
      const float vault = variant == 2U ? max(0.0f, 1.0f - (fabsf(static_cast<float>(column) - (kMatrixColumns - 1) * 0.5f) / 3.6f)) : 0.0f;
      const float intensity = min(1.0f, (0.20f + 0.80f * ridge) * (0.50f + 0.50f * secondary) + gust * 0.28f + vault * 0.22f);
      RgbColor color = blendColors(makeColor(72, 28, 0), makeColor(214, 134, 34), unitToScale(ridge));
      color = blendColors(color, makeColor(255, 220, 120), unitToScale(secondary * (variant == 1U ? 0.38f : 0.22f)));
      setMatrixPixel(row, column, scaleColor(color, unitToScale(intensity)));
    }
  }
}

void renderMatrixQuake(uint32_t now, uint8_t variant) {
  clearMatrixFrame();

  const uint32_t cycleMs = variant == 2U ? 680U : 900U;
  const uint32_t eventIndex = now / cycleMs;
  const uint32_t local = now % cycleMs;
  const uint8_t faultCount = variant == 2U ? 3U : (variant == 1U ? 2U : 1U);
  const float shockRadius = 0.6f + (static_cast<float>(local) / static_cast<float>(cycleMs)) * 7.0f;

  for (uint8_t row = 0U; row < kMatrixRows; ++row) {
    for (uint8_t column = 0U; column < kMatrixColumns; ++column) {
      RgbColor pixelColor = makeColor(0, 0, 0);
      for (uint8_t faultIndex = 0U; faultIndex < faultCount; ++faultIndex) {
        const uint32_t seed = hash32(eventIndex * 991U + faultIndex * 131U + variant * 29U);
        const int baseColumn = 1 + static_cast<int>(seed % max<uint8_t>(1U, kMatrixColumns - 2U));
        const int jitter = lroundf(sinf(row * 0.95f + (seed & 0xFFU) * 0.03f + local / 140.0f) * (variant == 2U ? 2.6f : 1.8f));
        const int noiseOffset = static_cast<int>((hash32(seed + row * 53U) & 0x3U)) - 1;
        const int crackColumn = constrain(baseColumn + jitter + noiseOffset, 0, kMatrixColumns - 1);
        const int epicenterRow = static_cast<int>((seed >> 8) % kMatrixRows);
        const float crackMask = max(0.0f, 1.0f - (fabsf(static_cast<float>(column) - crackColumn) / (variant == 1U ? 1.2f : 0.8f)));
        const float shockDistance = sqrtf(powf(static_cast<float>(column) - baseColumn, 2.0f) + powf(static_cast<float>(row) - epicenterRow, 2.0f));
        const float shock = bandStrength(shockDistance, shockRadius + faultIndex * 0.6f, 0.55f);
        const float flicker = 0.5f + 0.5f * sinf(local / 70.0f + row * 0.8f + faultIndex);
        const float intensity = min(1.0f, crackMask * (0.56f + 0.44f * flicker) + shock * 0.82f);
        const RgbColor color = twoHueColor(
          static_cast<uint8_t>(faultIndex * 24U + (now / 26U)),
          static_cast<uint8_t>(40U + faultIndex * 18U + row * 6U),
          shock,
          intensity
        );
        pixelColor = addColors(pixelColor, color);
      }

      setMatrixPixel(row, column, pixelColor);
    }
  }
}

void renderMatrixSonar(uint32_t now, uint8_t variant) {
  clearMatrixFrame();

  const float centerX = variant == 1U ? -1.0f : ((kMatrixColumns - 1) * 0.5f + sinf(now / 520.0f) * 0.8f);
  const float centerY = variant == 1U ? (kMatrixRows - 1) * 0.5f : ((kMatrixRows - 1) * 0.5f + cosf(now / 610.0f) * 0.7f);
  const float phase = static_cast<float>(now) / 280.0f;

  for (uint8_t row = 0U; row < kMatrixRows; ++row) {
    for (uint8_t column = 0U; column < kMatrixColumns; ++column) {
      const float deltaX = static_cast<float>(column) - centerX;
      const float deltaY = static_cast<float>(row) - centerY;
      const float angle = atan2f(deltaY, deltaX);
      const float distance = sqrtf(deltaX * deltaX + deltaY * deltaY);
      float intensity = 0.0f;

      if (variant == 0U) {
        const float ping = bandStrength(distance, fmodf(phase * 2.0f, 6.4f), 0.42f);
        const float sweep = max(0.0f, 1.0f - (wrappedAngleDelta(angle, phase * 0.9f) / 0.22f));
        intensity = min(1.0f, ping * 0.92f + sweep * 0.56f * radialStrength(distance, 6.4f));
      } else if (variant == 1U) {
        const float fanCenter = (phase * 0.72f) - 0.8f;
        const float fan = max(0.0f, 1.0f - (wrappedAngleDelta(angle, fanCenter) / 0.52f));
        const float band = bandStrength(distance, 2.2f + fmodf(phase * 1.6f, 4.6f), 0.55f);
        intensity = min(1.0f, fan * (0.34f + 0.66f * radialStrength(distance, 7.0f)) + band * 0.48f);
      } else {
        const float horizontal = bandStrength(static_cast<float>(column), fmodf(phase * 2.4f, kMatrixColumns + 1.0f), 0.48f);
        const float vertical = bandStrength(static_cast<float>(row), fmodf(phase * 1.9f, kMatrixRows + 1.0f), 0.48f);
        intensity = min(1.0f, horizontal * 0.72f + vertical * 0.72f);
      }

      RgbColor color = blendColors(makeColor(4, 36, 48), makeColor(64, 255, 220), unitToScale(intensity));
      color = blendColors(color, makeColor(255, 255, 255), unitToScale(intensity * 0.18f));
      setMatrixPixel(row, column, color);
    }
  }
}

void renderMatrixStarfield(uint32_t now, uint8_t variant) {
  fillMatrixFrame(makeColor(0, 0, 6));

  const float centerX = (kMatrixColumns - 1) * 0.5f;
  const float centerY = (kMatrixRows - 1) * 0.5f;
  const float basePhase = static_cast<float>(now) / 680.0f;

  for (uint8_t star = 0U; star < 12U; ++star) {
    const uint32_t seed = hash32(star * 547U + variant * 97U);
    const RgbColor starColor = blendColors(makeColor(96, 160, 255), makeColor(255, 255, 255), static_cast<uint8_t>((seed >> 8) & 0x7FU));

    if (variant < 2U) {
      const float progress = fmodf((variant == 1U ? static_cast<float>(now) / 420.0f : basePhase) + (seed & 0xFFU) / 255.0f, 1.0f);
      const float angle = ((seed >> 8) & 0xFFU) / 255.0f * (2.0f * PI);
      const float radius = progress * progress * (variant == 1U ? 7.4f : 6.0f);
      const float column = centerX + cosf(angle) * radius;
      const float row = centerY + sinf(angle) * radius * 0.82f;
      addDirectionalTrail(
        row,
        column,
        sinf(angle) * (variant == 1U ? 0.95f : 0.62f),
        cosf(angle) * (variant == 1U ? 1.35f : 0.90f),
        variant == 1U ? 5U : 3U,
        starColor,
        220U,
        16U
      );
      continue;
    }

    const float progress = fmodf(static_cast<float>(now) / (640.0f + star * 17.0f) + (seed & 0x3FU) / 64.0f, 1.0f);
    const float column = fmodf(progress * (kMatrixColumns + 4.0f) - 2.0f + sinf(now / 300.0f + star) * 0.8f + (seed & 0x3U), kMatrixColumns + 4.0f) - 2.0f;
    const float row = fmodf(progress * (kMatrixRows + 3.0f) - 1.0f + cosf(now / 360.0f + star * 0.7f) * 1.5f, kMatrixRows + 3.0f) - 1.0f;
    addDirectionalTrail(row, column, -0.35f, 0.70f, 4U, starColor, 210U, 20U);
  }
}

void renderMatrixStrobe(uint32_t now, uint8_t variant) {
  clearMatrixFrame();

  const float beat = fmodf(static_cast<float>(now) / (variant == 2U ? 120.0f : 160.0f), 1.0f);
  const float gateA = heartbeatEnvelope(beat);
  const float gateB = pulseWindow(beat, 0.66f, 0.12f);
  const float centerX = (kMatrixColumns - 1) * 0.5f;
  const float centerY = (kMatrixRows - 1) * 0.5f;

  for (uint8_t row = 0U; row < kMatrixRows; ++row) {
    for (uint8_t column = 0U; column < kMatrixColumns; ++column) {
      float flash = 0.0f;
      if (variant == 0U) {
        flash = ((row + column) & 1U) == 0U ? gateA : gateB;
        flash += ((row % 2U) == 0U || (column % 2U) == 0U) ? gateB * 0.35f : 0.0f;
      } else if (variant == 1U) {
        const float slice = 0.5f + 0.5f * sinf((column * 0.9f + row * 1.2f) - now / 55.0f);
        flash = max(gateA * slice, gateB * (1.0f - slice));
      } else {
        const float diamond = fabsf(static_cast<float>(column) - centerX) + fabsf(static_cast<float>(row) - centerY);
        const float tunnel = bandStrength(diamond, 1.0f + triangleUnit(beat) * 4.0f, 0.7f);
        flash = max(gateA * 0.72f, tunnel);
      }

      RgbColor color = blendColors(colorWheel(static_cast<uint8_t>((now / 10U) + row * 18U + column * 12U)), makeColor(255, 255, 255), unitToScale(flash * 0.62f));
      setMatrixPixel(row, column, scaleColor(color, unitToScale(min(1.0f, flash))));
    }
  }
}

void renderMatrixPhaseField(uint32_t now, uint8_t variant) {
  clearMatrixFrame();

  const float phase = static_cast<float>(now) / (variant == 2U ? 210.0f : 280.0f);
  const float centerX = (kMatrixColumns - 1) * 0.5f;
  const float centerY = (kMatrixRows - 1) * 0.5f;

  for (uint8_t row = 0U; row < kMatrixRows; ++row) {
    for (uint8_t column = 0U; column < kMatrixColumns; ++column) {
      float intensity = 0.0f;
      float mix = 0.0f;
      if (variant == 0U) {
        const float waveX = 0.5f + 0.5f * sinf(column * 1.12f + phase);
        const float waveY = 0.5f + 0.5f * sinf(row * 1.34f - phase * 1.18f);
        intensity = waveX * waveY;
        mix = waveX;
      } else if (variant == 1U) {
        const float deltaX = static_cast<float>(column) - centerX;
        const float deltaY = static_cast<float>(row) - centerY;
        const float angle = atan2f(deltaY, deltaX);
        const float distance = sqrtf(deltaX * deltaX + deltaY * deltaY);
        intensity = 0.5f + 0.5f * sinf(angle * 3.0f + distance * 1.7f - phase * 1.9f);
        mix = 0.5f + 0.5f * sinf(distance * 2.6f + phase * 1.4f);
      } else {
        const float causticA = max(0.0f, sinf(column * 0.92f + phase * 1.3f));
        const float causticB = max(0.0f, sinf(row * 1.16f - phase * 1.1f));
        const float causticC = max(0.0f, sinf((column + row) * 0.58f + phase * 1.9f));
        intensity = min(1.0f, (causticA + causticB + causticC) / 1.8f);
        mix = 0.5f + 0.5f * sinf((column - row) * 0.64f + phase * 0.8f);
      }

      RgbColor color = twoHueColor(
        static_cast<uint8_t>(120U + row * 8U + (now / 26U)),
        static_cast<uint8_t>(190U + column * 10U + (now / 34U)),
        mix,
        intensity
      );
      setMatrixPixel(row, column, color);
    }
  }
}

void renderMatrixFrame(uint32_t now) {
  if (activeMatrixPattern == nullptr) {
    clearMatrixFrame();
    return;
  }

  int effectVariant = -1;
  if (strcmp(activeMatrixPattern->id, kMatrixSolidPatternId) == 0) {
    renderMatrixSolid();
  } else if (strcmp(activeMatrixPattern->id, "scanner") == 0) {
    renderMatrixScanner(now);
  } else if (strcmp(activeMatrixPattern->id, "spectrum-scan") == 0) {
    renderMatrixSpectrumScan(now);
  } else if (strcmp(activeMatrixPattern->id, "spectrum-storm") == 0) {
    renderMatrixSpectrumStorm(now);
  } else if (strcmp(activeMatrixPattern->id, "lightning") == 0) {
    renderMatrixLightning(now);
  } else if (strcmp(activeMatrixPattern->id, "storm-ripple") == 0) {
    renderMatrixStormRipple(now);
  } else if (strcmp(activeMatrixPattern->id, "storm-raster") == 0) {
    renderMatrixStormRaster(now);
  } else if (strcmp(activeMatrixPattern->id, "pride-wave") == 0) {
    renderMatrixPrideWave(now);
  } else if (strcmp(activeMatrixPattern->id, "heart-throb") == 0) {
    renderMatrixHeartThrob(now);
  } else if (strcmp(activeMatrixPattern->id, "beat-wave") == 0) {
    renderMatrixBeatWave(now);
  } else if (strcmp(activeMatrixPattern->id, "tunnel-bloom") == 0) {
    renderMatrixTunnelBloom(now);
  } else if (strcmp(activeMatrixPattern->id, "shard-rain") == 0) {
    renderMatrixShardRain(now);
  } else if (strcmp(activeMatrixPattern->id, "orbit-dots") == 0) {
    renderMatrixOrbitDots(now);
  } else if (strcmp(activeMatrixPattern->id, "pinwheel") == 0) {
    renderMatrixPinwheel(now);
  } else if (strcmp(activeMatrixPattern->id, "lattice-bloom") == 0) {
    renderMatrixLatticeBloom(now);
  } else if (strcmp(activeMatrixPattern->id, "sunburst") == 0) {
    renderMatrixSunburst(now);
  } else if (strcmp(activeMatrixPattern->id, "circuit-trace") == 0) {
    renderMatrixCircuitTrace(now);
  } else if (strcmp(activeMatrixPattern->id, "vortex") == 0) {
    renderMatrixVortex(now);
  } else if (strcmp(activeMatrixPattern->id, "skyline-pulse") == 0) {
    renderMatrixSkylinePulse(now);
  } else if (strcmp(activeMatrixPattern->id, "crystal-scan") == 0) {
    renderMatrixCrystalScan(now);
  } else if (strcmp(activeMatrixPattern->id, "pulse-diamond") == 0) {
    renderMatrixPulseDiamond(now);
  } else if (strcmp(activeMatrixPattern->id, "comet-orbit") == 0) {
    renderMatrixCometOrbit(now);
  } else if (strcmp(activeMatrixPattern->id, "chase") == 0) {
    renderMatrixChase(now);
  } else if (strcmp(activeMatrixPattern->id, "checker") == 0) {
    renderMatrixChecker(now);
  } else if (strcmp(activeMatrixPattern->id, "sparkle") == 0) {
    renderMatrixSparkle(now);
  } else if (strcmp(activeMatrixPattern->id, "plasma") == 0) {
    renderMatrixPlasma(now);
  } else if (strcmp(activeMatrixPattern->id, "ripple") == 0) {
    renderMatrixRipple(now);
  } else if (strcmp(activeMatrixPattern->id, "ripple-dual") == 0) {
    renderMatrixDualRipple(now);
  } else if (strcmp(activeMatrixPattern->id, "ripple-core") == 0) {
    renderMatrixRippleCore(now);
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
  } else if (strcmp(activeMatrixPattern->id, "lava") == 0) {
    renderMatrixLava(now);
  } else if (strcmp(activeMatrixPattern->id, "spiral") == 0) {
    renderMatrixSpiral(now);
  } else if (strcmp(activeMatrixPattern->id, "radar") == 0) {
    renderMatrixRadar(now);
  } else if (strcmp(activeMatrixPattern->id, "helix") == 0) {
    renderMatrixHelix(now);
  } else if (strcmp(activeMatrixPattern->id, "raster-trace") == 0) {
    renderMatrixRasterTrace(now);
  } else if (strcmp(activeMatrixPattern->id, "zigzag-trace") == 0) {
    renderMatrixZigzagTrace(now);
  } else if (strcmp(activeMatrixPattern->id, "spiral-trace") == 0) {
    renderMatrixSpiralTrace(now);
  } else if ((effectVariant = matrixEffectVariant3(activeMatrixPattern->id, "aurora-drift", "aurora-fold", "aurora-borealis")) >= 0) {
    renderMatrixAurora(now, static_cast<uint8_t>(effectVariant));
  } else if ((effectVariant = matrixEffectVariant3(activeMatrixPattern->id, "tidal-pulse", "undertow", "crosscurrent")) >= 0) {
    renderMatrixTidalFlow(now, static_cast<uint8_t>(effectVariant));
  } else if ((effectVariant = matrixEffectVariant3(activeMatrixPattern->id, "gravity-well", "graviton-lens", "singularity-burst")) >= 0) {
    renderMatrixGravityField(now, static_cast<uint8_t>(effectVariant));
  } else if ((effectVariant = matrixEffectVariant3(activeMatrixPattern->id, "ember-updraft", "forge-bloom", "ashfall")) >= 0) {
    renderMatrixEmberField(now, static_cast<uint8_t>(effectVariant));
  } else if ((effectVariant = matrixEffectVariant3(activeMatrixPattern->id, "prism-fan", "prism-slice", "prism-cascade")) >= 0) {
    renderMatrixPrismBurst(now, static_cast<uint8_t>(effectVariant));
  } else if ((effectVariant = matrixEffectVariant3(activeMatrixPattern->id, "swarm", "swarm-weave", "swarm-burst")) >= 0) {
    renderMatrixSwarmEffect(now, static_cast<uint8_t>(effectVariant));
  } else if ((effectVariant = matrixEffectVariant3(activeMatrixPattern->id, "solar-flare", "corona-spin", "eclipse-ring")) >= 0) {
    renderMatrixSolarField(now, static_cast<uint8_t>(effectVariant));
  } else if ((effectVariant = matrixEffectVariant3(activeMatrixPattern->id, "data-rain", "data-stream", "data-burst")) >= 0) {
    renderMatrixDataFlow(now, static_cast<uint8_t>(effectVariant));
  } else if ((effectVariant = matrixEffectVariant3(activeMatrixPattern->id, "dune-shift", "dune-rush", "dune-vault")) >= 0) {
    renderMatrixDuneFlow(now, static_cast<uint8_t>(effectVariant));
  } else if ((effectVariant = matrixEffectVariant3(activeMatrixPattern->id, "quake-lines", "fault-pulse", "fracture-field")) >= 0) {
    renderMatrixQuake(now, static_cast<uint8_t>(effectVariant));
  } else if ((effectVariant = matrixEffectVariant3(activeMatrixPattern->id, "sonar-ping", "sonar-fan", "sonar-net")) >= 0) {
    renderMatrixSonar(now, static_cast<uint8_t>(effectVariant));
  } else if ((effectVariant = matrixEffectVariant3(activeMatrixPattern->id, "starfield-warp", "hyperspace", "orbital-slip")) >= 0) {
    renderMatrixStarfield(now, static_cast<uint8_t>(effectVariant));
  } else if ((effectVariant = matrixEffectVariant3(activeMatrixPattern->id, "strobe-grid", "strobe-slices", "strobe-tunnel")) >= 0) {
    renderMatrixStrobe(now, static_cast<uint8_t>(effectVariant));
  } else if ((effectVariant = matrixEffectVariant3(activeMatrixPattern->id, "wave-lattice", "phase-knot", "caustic-flow")) >= 0) {
    renderMatrixPhaseField(now, static_cast<uint8_t>(effectVariant));
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
