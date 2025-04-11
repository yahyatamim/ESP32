/*
  Name: Advanced Timer
  Author: Yahya Tamim
  Date: 08 April, 2025

  This firmware leverages the ESP32's local server hosting to create a self-contained,
  basic PLC. The project aims to make automation accessible to non-programmers unfamiliar
  with C or ladder logic, targeting users who rely on relay logic and simple timer modules
  for complex tasks. Named "Advanced Timer," it offers more than a traditional timer—essentially
  a PLC with a unique name and an intuitive drop-down menu interface instead of ladder logic.

  The configuration portal, backend server, and execution engine are fully embedded in the
  ESP32 firmware, requiring no internet, external software, or vendor lock-in.
*/

#include "configPortal.h"        // Will Host our configuration portal
#include <WiFi.h>                // Needed for WiFi
#include <ntpclient.h>           // Network Time
#include <RTClib.h>              // timekeeping for scheduling
#include <WiFiUdp.h>             // Needed for ntpClient
#include <ESP32TimerInterrupt.h> // Hardware Timer
#include <esp_task_wdt.h>        // Watchdog timer
#include <TickTwo.h>

// python -m esptool --chip esp32 erase_flash

// Network task declaration
TaskHandle_t networkTask;
void networkTaskFunction(void *pvParameters);
#define wdtTimeout 300 // Watchdog timeout (seconds)

#define SSID "FactoryNext1"
#define PASS "FactoryNext20$22#"
// #define SSID "Desktop_tamim"
// #define PASS "12345678"

RTC_Millis rtc;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 21600, 60000);



void setup()
{
  Serial.begin(115200);

  // Create network task on core 0 with 32K stack size
  xTaskCreatePinnedToCore(networkTaskFunction, "networkTask", 32768, NULL, 1,
                          &networkTask, 0);

  esp_task_wdt_init(wdtTimeout, true);
  esp_task_wdt_add(NULL);
}

void loop()
{
  delay(1000);
  esp_task_wdt_reset();
}

void networkTaskFunction(void *pvParameters)
{
  String ssid, password;

  // Load WiFi credentials from NVS
  loadWiFiCredentials(ssid, password);

  if (!ssid.isEmpty() && !password.isEmpty())
  {
    WiFi.begin(ssid.c_str(), password.c_str());
    Serial.print("Connecting to WiFi");
    for (int i = 0; i < 20 && WiFi.status() != WL_CONNECTED; i++)
    {
      delay(1000);
      Serial.print(".");
    }
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println();
      Serial.print("Connected with IP: ");
      Serial.println(WiFi.localIP());
    }
    else
    {
      Serial.println("\nFailed to connect to WiFi");
    }
  }
  else
  {
    Serial.println("No WiFi credentials found in NVS.");
  }

  // If WiFi is not configured, start in Access Point mode
  if (WiFi.status() != WL_CONNECTED)
  {
    WiFi.softAP("AdvancedTimer", "12345678");
    Serial.print("Started Access Point with IP: ");
    Serial.println(WiFi.softAPIP());
  }

  // Begin Config Portal
  configPortal.begin();

  esp_task_wdt_init(wdtTimeout, true);
  esp_task_wdt_add(NULL);

  for (;;)
  {
    delay(1000);
    esp_task_wdt_reset();
  }
}