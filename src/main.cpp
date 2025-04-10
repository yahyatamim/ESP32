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

#include "configPortal.h" // Will Host our configuration portal
#include <WiFi.h> // Needed for WiFi
#include <ntpclient.h> // Network Time
#include <RTClib.h> // timekeeping for scheduling
#include <WiFiUdp.h> // Needed for ntpClient
#include <ESP32TimerInterrupt.h> // Hardware Timer


// python -m esptool --chip esp32 erase_flash

#define SSID "FactoryNext1"
#define PASS "FactoryNext20$22#"
// #define SSID "Desktop_tamim"
// #define PASS "12345678"

RTC_Millis rtc;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 21600, 60000);


enum dataTypes {
  DigitalInput,   // Represents a physical digital input (e.g., buttons, switches)
  DigitalOutput,  // Represents a physical digital output (e.g., LEDs, relays)
  AnalogInput,    // Represents a physical analog input (e.g., sensors like temperature, light)
  SoftIO,         // Represents a virtual input/output controlled by software (e.g., counters, virtual flags)
  Timer           // Represents a timer for scheduling or time-based operations
};

enum operationMode { 
  none,         // Disable input interrupt
  rising,       // Interrupt on rising edge
  falling,      // Interrupt on falling edge
  stateChange,  // Interrupt on state change
  pulse,        // Pulse output (Digital Output)
  oneShot,      // One-shot timer
  repeating,    // Repeating timer
  countdown,    // Countdown timer
};

struct IOVariable {
  uint8_t num;          // Variable number or index (unique identifier)
  dataTypes type;       // Type of the variable (DigitalInput, DigitalOutput, AnalogInput, SoftIO, Timer)
  uint8_t gpio;         // GPIO pin if applicable (used for physical inputs/outputs)
  operationMode mode;   // Additional operating mode attribute (defines behavior based on type)
  char name[20];        // Human-friendly name for the variable (e.g., "Sensor1", "Relay1")
  bool state;           // Boolean state (e.g., ON/OFF for Digital IO, active/inactive for Timer)
  int32_t value;        // Primary value (e.g., current reading for AnalogInput, counter value for SoftIO)
  bool flag;            // Timer completed, Digital Input Inturrpt Flag 
};


enum comparisons {
  isTrue,         // Checks if the state is true
  isFalse,        // Checks if the state is false
  isEqual,        // Checks if two values are equal
  isLess,         // Checks if a value is less than another
  isGreater,      // Checks if a value is greater than another
  flagIsTrue,     // Checks if a flag is set
  flagIsFalse,    // Checks if a flag is set
};

struct condition {
  uint8_t conNum;     // Unique identifier for the condition
  char name[20];      // Human-readable name for the condition
  dataTypes Type;     // Type of the first variable (e.g., DigitalInput, Timer, etc.)
  uint8_t targetNum;  // Identifier for the first variable
  comparisons comp;   // Comparison type (e.g., isEqual, isGreater, etc.)
  int32_t value;      // Constant value for comparison
};

enum actionType {
  set,          // Set state to true
  reset,        // Set state to false
  setValue,     // Set value1 to a number
  increment,    // Increase value by specified number
  decrement,    // Decrease value1 by specified 
  setFlag,        // Set flag to true
  clear,        // Clear flag
};

struct action {
  uint8_t actNum;     // Unique identifier for the action
  char name[20];      // Human-readable name for the action (e.g., "Increment Counter")
  dataTypes Type;     // Type of the first variable (e.g., DigitalInput, Timer, etc.)
  uint8_t targetNum;  // Target variable number (index in the IOVariables array)
  actionType action;  // Type of action to perform (e.g., set, increment, toggle)
  int32_t value;      // Value to use for the action (e.g., increment amount, threshold)
};

enum combineLogic {
  andLogic,  // All conditions in the group must be true
  orLogic    // At least one condition in the group must be true
};

struct conditionGroup {
  uint8_t num;                  // Unique identifier for the condition group
  char name[20];                // Human-readable name for the group
  uint8_t conditionArray[10];   // Array of condition indices (from `Conditions`)
  bool resultArray[10];         // Array of condition results (true/false)
  combineLogic Logic;    // Logic to combine conditions (andLogic/orLogic)
};

struct actionGroup {
  uint8_t num;                  // Unique identifier for the action group
  char name[20];                // Human-readable name for the group
  uint8_t actionArray[10];      // Array of action indices (from `Actions`)
};

struct rule {
  uint8_t num;              // Unique identifier for the rule
  char name[20];            // Human-readable name for the rule
  uint8_t conditionGroup;   // Index of the associated condition group
  uint8_t actionGroup;      // Index of the associated action group
  bool status;              // Status of the rule (active/inactive)
};

uint8_t ruleSequence[MAX_RULES];

void setup() {
  Serial.begin(115200);
  WiFi.begin(SSID, PASS);
  Serial.print("\nConnecting to WiFi");
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  if (timeClient.update()){
    rtc.begin(DateTime(timeClient.getEpochTime()));
    Serial.println(rtc.now().timestamp(DateTime::TIMESTAMP_FULL));
  }
}

void loop() {
}