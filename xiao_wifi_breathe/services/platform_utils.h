#pragma once

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

RgbColor addColors(const RgbColor &first, const RgbColor &second) {
  const uint16_t red = static_cast<uint16_t>(first.red) + second.red;
  const uint16_t green = static_cast<uint16_t>(first.green) + second.green;
  const uint16_t blue = static_cast<uint16_t>(first.blue) + second.blue;
  return {
    static_cast<uint8_t>(red > 255U ? 255U : red),
    static_cast<uint8_t>(green > 255U ? 255U : green),
    static_cast<uint8_t>(blue > 255U ? 255U : blue),
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

void setLedBrightness(uint8_t brightness) {
  const uint8_t pwmValue = kLedActiveLow ? (kPwmMax - brightness) : brightness;
  analogWrite(kLedPin, pwmValue);
}
