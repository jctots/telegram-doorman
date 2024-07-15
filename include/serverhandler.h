#pragma once
#include <Arduino.h>

extern void handleIndexPage(AsyncWebServerRequest *request);
extern String loadConfigValues(const String& var);
extern void handleGet(AsyncWebServerRequest *request);
extern void handle404(AsyncWebServerRequest *request);
