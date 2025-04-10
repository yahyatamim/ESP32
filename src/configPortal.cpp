#include "configPortal.h"

/*
Color Cue Notes:
----------------
1. Gradient Header Background:
   - Linear Gradient: #8E24AA, #6A1B9A, #4A148C
   - Used for the header card background (#headerCard).

2. Primary Button Color:
   - Background: #7B1FA2
   - Border: #6A1B9A
   - Used for primary buttons (e.g., Save & Reboot button).

3. Softer Card Background:
   - Background: #f3e5f5 (Soft Lavender)
   - Used for custom cards (e.g., Configuration Settings, Live Sensor Data).

4. Text Colors:
   - Header Text: White (#FFFFFF)
   - Footer Text: Muted Gray (default Bootstrap muted text).

5. Icon Colors (from external sources):
   - Temperature Icon: Colorful thermometer icon from https://img.icons8.com
   - Humidity Icon: Colorful hygrometer icon from https://img.icons8.com
   - VOC Icon: Colorful test tube icon from https://img.icons8.com

6. Button States (Dynamic):
   - Primary: #7B1FA2 (default state)
   - Warning: Bootstrap warning color (e.g., yellow/orange)
   - Info: Bootstrap info color (e.g., light blue)
   - Danger: Bootstrap danger color (e.g., red)

7. Text Shadow for Header:
   - Shadow: 2px 2px 4px rgba(0, 0, 0, 0.3)
   - Used for header text to enhance readability.
*/

ConfigPortal::ConfigPortal() : server(80), portalActive(false)
{
    // Setup web server routes
    server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request)
              { this->handleRoot(request); });

    server.on("/config", HTTP_GET, [this](AsyncWebServerRequest *request)
              { this->handleGetConfig(request); });

    server.on("/config", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
              { this->handleSaveConfig(request, data, len); });

    server.on("/status", HTTP_GET, [this](AsyncWebServerRequest *request)
              { this->handleGetStatus(request); });
}

void ConfigPortal::begin()
{
    // Initialize SPIFFS
    if (!SPIFFS.begin(true))
    {
        Serial.println("Error mounting SPIFFS");
        return;
    }

    // Serve static files
    server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

    // Start the server
    server.begin();
    portalActive = true;
    Serial.println("Configuration portal started");
}

void ConfigPortal::stop()
{
    server.end();
    portalActive = false;
}

void ConfigPortal::loadConfig()
{
    if (SPIFFS.exists("/config.json"))
    {
        File configFile = SPIFFS.open("/config.json", "r");
        if (configFile)
        {
            String jsonString = configFile.readString();
            configFile.close();
            parseJsonConfig(jsonString);
        }
    }
}

void ConfigPortal::saveConfig()
{
    File configFile = SPIFFS.open("/config.json", "w");
    if (configFile)
    {
        String jsonString = generateJsonConfig();
        configFile.print(jsonString);
        configFile.close();
    }
}

void ConfigPortal::handleRoot(AsyncWebServerRequest *request)
{
    request->send(SPIFFS, "/index.html", "text/html");
}

void ConfigPortal::handleGetConfig(AsyncWebServerRequest *request)
{
    String jsonConfig = generateJsonConfig();
    request->send(200, "application/json", jsonConfig);
}

void ConfigPortal::handleSaveConfig(AsyncWebServerRequest *request, uint8_t *data, size_t len)
{
    String jsonString = String((char *)data).substring(0, len);

    // Parse the JSON data
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, jsonString);
    if (error)
    {
        request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"Invalid JSON\"}");
        return;
    }

    // Extract WiFi credentials
    String ssid = doc["ssid"];
    String password = doc["password"];

    // Save the credentials to SPIFFS
    File configFile = SPIFFS.open("/wifi_config.json", "w");
    if (configFile)
    {
        configFile.print(jsonString);
        configFile.close();
        request->send(200, "application/json", "{\"status\":\"success\",\"message\":\"Configuration saved. Rebooting...\"}");
        delay(1000);
        ESP.restart(); // Reboot the ESP32
    }
    else
    {
        request->send(500, "application/json", "{\"status\":\"error\",\"message\":\"Failed to save configuration\"}");
    }
}

void ConfigPortal::handleGetStatus(AsyncWebServerRequest *request)
{
    String status = getSystemStatus();
    request->send(200, "application/json", status);
}

String ConfigPortal::generateJsonConfig()
{
    // Placeholder: Return an empty JSON object
    return "{}";
}

void ConfigPortal::parseJsonConfig(const String &jsonString)
{
    // Placeholder: Parse the JSON string (not implemented yet)
    Serial.println("Parsing JSON config: " + jsonString);
}

String ConfigPortal::getSystemStatus()
{
    // Placeholder: Return a dummy system status
    return "{\"status\":\"ok\"}";
}

// Create a global instance
ConfigPortal configPortal;