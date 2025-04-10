#ifndef CONFIG_PORTAL_H
#define CONFIG_PORTAL_H

#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

// Define maximum number of variables
#define MAX_DIGITAL_IN 8
#define MAX_DIGITAL_OUT 8
#define MAX_ANALOG_IN 4
#define MAX_SOFTIO 20
#define MAX_TIMERS 16
#define MAX_CONDITIONS 50
#define MAX_ACTIONS 50
#define MAX_RULES 20

class ConfigPortal {
private:
    AsyncWebServer server;
    bool portalActive;

    // Configuration management
    void loadConfig();
    void saveConfig();
    String generateJsonConfig();
    void parseJsonConfig(const String& jsonString);

    // Web request handlers
    void handleRoot(AsyncWebServerRequest *request);
    void handleGetConfig(AsyncWebServerRequest *request);
    void handleSaveConfig(AsyncWebServerRequest *request, uint8_t *data, size_t len);
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