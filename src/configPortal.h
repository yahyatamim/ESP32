#ifndef CONFIG_PORTAL_H
#define CONFIG_PORTAL_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <Preferences.h>

#include "dataStructure.h"

void loadWiFiCredentials(String &ssid, String &password);
void saveWiFiCredentials(const String &ssid, const String &password);

class ConfigPortal {
 private:
  AsyncWebServer server;
  bool portalActive;

  // Configuration management
  void loadConfig();
  void saveConfig();
  String generateJsonConfig();
  void parseJsonConfig(const String &jsonString);

  // Web request handlers
  void handleRoot(AsyncWebServerRequest *request);
  void handleGetConfig(AsyncWebServerRequest *request);
  void handleSaveConfig(AsyncWebServerRequest *request, uint8_t *data,
                        size_t len);
  void handleGetStatus(AsyncWebServerRequest *request);

 public:
  ConfigPortal();
  void begin();
  void stop();
  bool isActive() { return portalActive; }

  // Configuration validation
  bool validateConfig();

  // Status reporting
  String getSystemStatus();
};

extern ConfigPortal configPortal;

#endif