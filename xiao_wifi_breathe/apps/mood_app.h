#pragma once

bool moodAppAvailable() {
  return matrixReady;
}

String buildMoodJson() {
  String json;
  json.reserve(1024);

  const MoodDefinition *selectedMood = findMoodById(matrixMoodId);
  const char *selectedLabel = selectedMood == nullptr ? "Unknown" : selectedMood->label;

  json += F("{\"available\":");
  json += matrixReady ? F("true") : F("false");
  json += F(",\"selectedPatternId\":\"");
  json += activeMatrixPattern->id;
  json += F("\",\"selectedMoodId\":\"");
  json += jsonEscape(matrixMoodId);
  json += F("\",\"selectedMoodLabel\":\"");
  json += jsonEscape(String(selectedLabel));
  json += F("\",\"moods\":[");

  for (size_t index = 0; index < sizeof(kMoods) / sizeof(kMoods[0]); ++index) {
    if (index > 0) {
      json += ',';
    }

    json += F("{\"id\":\"");
    json += kMoods[index].id;
    json += F("\",\"label\":\"");
    json += kMoods[index].label;
    json += F("\"}");
  }

  json += F("]}");
  return json;
}

void handleMoodState() {
  sendJson(200, buildMoodJson());
}

void handleMoodChange() {
  String moodId;
  if (webServer.hasArg(F("plain"))) {
    moodId = extractJsonStringField(webServer.arg(F("plain")), "id");
  }

  if (moodId.isEmpty() && webServer.hasArg(F("id"))) {
    moodId = webServer.arg(F("id"));
  }

  if (moodId.isEmpty()) {
    sendJsonError(400, F("Request body must include a non-empty mood id."));
    return;
  }

  if (!activateMatrixMood(moodId, true)) {
    sendJsonError(400, String(F("Unknown mood id: ")) + moodId);
    return;
  }

  applyMatrixFrameNow();
  sendJson(200, buildMoodJson());
}

void registerMoodAppRoutes() {
  webServer.on(F("/api/mood"), HTTP_GET, handleMoodState);
  webServer.on(F("/api/mood"), HTTP_POST, handleMoodChange);
}
