#include "dataStructure.h"

#include <ArduinoJson.h>  // Include ArduinoJson
#include <LittleFS.h>     // Include LittleFS

// === Define Global Arrays (matching extern declarations in .h) ===
IOVariable ioVariables[MAX_DIGITAL_IN + MAX_DIGITAL_OUT + MAX_ANALOG_IN +
                       MAX_SOFTIO + MAX_TIMERS];
condition conditions[MAX_CONDITIONS];
conditionGroup conditionGroups[MAX_CONDITION_GROUPS];
action actions[MAX_ACTIONS];
actionGroup actionGroups[MAX_ACTION_GROUPS];
rule rules[MAX_RULES];
uint8_t ruleSequence[MAX_RULES];
// ==============================================================

// Create a default Map for initial population if config doesn't exist
IOVariable defaultIOVariables[] = {
    {1, DigitalInput, 4, none, "Digital Input 1", false, 0, false, false},
    {2, DigitalInput, 2, none, "Digital Input 2", false, 0, false, false},
    {3, DigitalInput, 15, none, "Digital Input 3", false, 0, false, false},
    {4, DigitalInput, 13, none, "Digital Input 4", false, 0, false, false},
    {5, DigitalInput, 12, none, "Digital Input 5", false, 0, false, false},
    {6, DigitalInput, 14, none, "Digital Input 6", false, 0, false, false},
    {1, DigitalOutput, 27, none, "Digital Outpot 1", false, 0, false, false},
    {2, DigitalOutput, 26, none, "Digital Outpot 2", false, 0, false, false},
    {3, DigitalOutput, 25, none, "Digital Outpot 3", false, 0, false, false},
    {4, DigitalOutput, 33, none, "Digital Outpot 4", false, 0, false, false},
    {1, AnalogInput, 35, none, "Analog Input 1", false, 0, false, false},
    {2, AnalogInput, 34, none, "Analog Input 2", false, 0, false, false},
    {3, AnalogInput, 39, none, "Analog Input 3", false, 0, false, false},
    {4, AnalogInput, 36, none, "Analog Input 4", false, 0, false, false},
    {1, SoftIO, 0, none, " Soft IO 1", false, 0, false, false},
    {2, SoftIO, 0, none, " Soft IO 2", false, 0, false, false},
    {3, SoftIO, 0, none, " Soft IO 3", false, 0, false, false},
    {4, SoftIO, 0, none, " Soft IO 4", false, 0, false, false},
    {5, SoftIO, 0, none, " Soft IO 5", false, 0, false, false},
    {6, SoftIO, 0, none, " Soft IO 6", false, 0, false, false},
    {7, SoftIO, 0, none, " Soft IO 7", false, 0, false, false},
    {8, SoftIO, 0, none, " Soft IO 8", false, 0, false, false},
    {9, SoftIO, 0, none, " Soft IO 9", false, 0, false, false},
    {10, SoftIO, 0, none, " Soft IO 10", false, 0, false, false},
    {11, SoftIO, 0, none, " Soft IO 11", false, 0, false, false},
    {12, SoftIO, 0, none, " Soft IO 12", false, 0, false, false},
    {13, SoftIO, 0, none, " Soft IO 13", false, 0, false, false},
    {14, SoftIO, 0, none, " Soft IO 14", false, 0, false, false},
    {15, SoftIO, 0, none, " Soft IO 15", false, 0, false, false},
    {16, SoftIO, 0, none, " Soft IO 16", false, 0, false, false},
    {17, SoftIO, 0, none, " Soft IO 17", false, 0, false, false},
    {18, SoftIO, 0, none, " Soft IO 18", false, 0, false, false},
    {19, SoftIO, 0, none, " Soft IO 19", false, 0, false, false},
    {20, SoftIO, 0, none, " Soft IO 20", false, 0, false, false},
    {1, Timer, 0, oneShot, "Timer 1", false, 0, false, false},
    {2, Timer, 0, oneShot, "Timer 2", false, 0, false, false},
    {3, Timer, 0, oneShot, "Timer 3", false, 0, false, false},
    {4, Timer, 0, oneShot, "Timer 4", false, 0, false, false},
    {5, Timer, 0, oneShot, "Timer 5", false, 0, false, false},
    {6, Timer, 0, oneShot, "Timer 6", false, 0, false, false},
    {7, Timer, 0, oneShot, "Timer 7", false, 0, false, false},
    {8, Timer, 0, oneShot, "Timer 8", false, 0, false, false},
    {9, Timer, 0, oneShot, "Timer 9", false, 0, false, false},
    {10, Timer, 0, oneShot, "Timer 10", false, 0, false, false},
    {11, Timer, 0, oneShot, "Timer 11", false, 0, false, false},
    {12, Timer, 0, oneShot, "Timer 12", false, 0, false, false},
    {13, Timer, 0, oneShot, "Timer 13", false, 0, false, false},
    {14, Timer, 0, oneShot, "Timer 14", false, 0, false, false},
    {15, Timer, 0, oneShot, "Timer 15", false, 0, false, false},
    {16, Timer, 0, oneShot, "Timer 16", false, 0, false, false},
};

const char* CONFIG_FILE = "/config.json";

// --- Helper Enum to String Converters (Keep these) ---
const char* dataTypeToString(dataTypes type) {
  switch (type) {
    case DigitalInput:
      return "DigitalInput";
    case DigitalOutput:
      return "DigitalOutput";
    case AnalogInput:
      return "AnalogInput";
    case SoftIO:
      return "SoftIO";
    case Timer:
      return "Timer";
    default:
      return "Unknown";
  }
}
const char* operationModeToString(operationMode mode) {
  switch (mode) {
    case none:
      return "none";
    case rising:
      return "rising";
    case falling:
      return "falling";
    case stateChange:
      return "stateChange";
    case pulse:
      return "pulse";
    case oneShot:
      return "oneShot";
    case repeating:
      return "repeating";
    case countdown:
      return "countdown";
    default:
      return "Unknown";
  }
}
const char* comparisonToString(comparisons comp) {
  switch (comp) {
    case isTrue:
      return "isTrue";
    case isFalse:
      return "isFalse";
    case isEqual:
      return "isEqual";
    case isLess:
      return "isLess";
    case isGreater:
      return "isGreater";
    case flagIsTrue:
      return "flagIsTrue";
    case flagIsFalse:
      return "flagIsFalse";
    default:
      return "Unknown";
  }
}
const char* actionTypeToString(actionType action) {
  switch (action) {
    case set:
      return "set";
    case reset:
      return "reset";
    case setValue:
      return "setValue";
    case increment:
      return "increment";
    case decrement:
      return "decrement";
    case setFlag:
      return "setFlag";
    case clear:
      return "clear";
    default:
      return "Unknown";
  }
}
const char* combineLogicToString(combineLogic logic) {
  switch (logic) {
    case andLogic:
      return "andLogic";
    case orLogic:
      return "orLogic";
    default:
      return "Unknown";
  }
}
// --- End Helper Enum to String Converters ---

// --- Serialization Helper Functions ---

/**
 * @brief Serializes the global ioVariables array into a JsonArray.
 * @param ioVarsArray The JsonArray to populate.
 */
void serializeIOVariablesToJson(JsonArray& ioVarsArray) {
  size_t totalIOs = MAX_DIGITAL_IN + MAX_DIGITAL_OUT + MAX_ANALOG_IN +
                    MAX_SOFTIO + MAX_TIMERS;
  for (size_t i = 0; i < totalIOs; ++i) {
    // Add all variables, active or not, for completeness in config/status
    JsonObject ioVar = ioVarsArray.add<JsonObject>();
    ioVar["num"] = ioVariables[i].num;
    ioVar["type"] = dataTypeToString(ioVariables[i].type);
    ioVar["gpio"] = ioVariables[i].gpio;
    ioVar["mode"] = operationModeToString(ioVariables[i].mode);
    ioVar["name"] = ioVariables[i].name;
    ioVar["state"] = ioVariables[i].state;
    ioVar["value"] = ioVariables[i].value;
    ioVar["flag"] = ioVariables[i].flag;
    ioVar["status"] = ioVariables[i].status;
  }
}

/**
 * @brief Serializes the logic-related configuration arrays (conditions,
 * actions, groups, rules, sequence) into the provided JsonDocument.
 * @param doc The JsonDocument to populate.
 */
void serializeLogicConfigurationToJson(JsonDocument& doc) {
  // --- Serialize Conditions ---
  JsonArray conditionsArray = doc["conditions"].to<JsonArray>();
  for (size_t i = 0; i < MAX_CONDITIONS; ++i) {
    JsonObject condition = conditionsArray.add<JsonObject>();
    condition["conNum"] = conditions[i].conNum;
    condition["name"] = conditions[i].name;
    condition["Type"] = dataTypeToString(conditions[i].Type);
    condition["targetNum"] = conditions[i].targetNum;
    condition["comp"] = comparisonToString(conditions[i].comp);
    condition["value"] = conditions[i].value;
    condition["status"] = conditions[i].status;
  }

  // --- Serialize Actions ---
  JsonArray actionsArray = doc["actions"].to<JsonArray>();
  for (size_t i = 0; i < MAX_ACTIONS; ++i) {
    JsonObject action = actionsArray.add<JsonObject>();
    action["actNum"] = actions[i].actNum;
    action["name"] = actions[i].name;
    action["Type"] = dataTypeToString(actions[i].Type);
    action["targetNum"] = actions[i].targetNum;
    action["action"] = actionTypeToString(actions[i].action);
    action["value"] = actions[i].value;
    action["status"] = actions[i].status;
  }

  // --- Serialize Condition Groups ---
  JsonArray conditionGroupsArray = doc["conditionGroups"].to<JsonArray>();
  for (size_t i = 0; i < MAX_CONDITION_GROUPS; ++i) {
    JsonObject group = conditionGroupsArray.add<JsonObject>();
    group["num"] = conditionGroups[i].num;
    group["name"] = conditionGroups[i].name;
    JsonArray condArr = group["conditionArray"].to<JsonArray>();
    // Assuming conditionArray stores indices up to a certain count
    // TODO: Ideally, store count in struct and iterate up to count
    for (uint8_t condIndex : conditionGroups[i].conditionArray) {
      // Add basic validation or just add the stored value
      condArr.add(condIndex);
    }
    // resultArray is runtime state, usually not needed in config/status JSON
    // JsonArray resArr = group["resultArray"].to<JsonArray>();
    // for(bool res : conditionGroups[i].resultArray) { resArr.add(res); }
    group["Logic"] = combineLogicToString(conditionGroups[i].Logic);
    group["status"] = conditionGroups[i].status;
  }

  // --- Serialize Action Groups ---
  JsonArray actionGroupsArray = doc["actionGroups"].to<JsonArray>();
  for (size_t i = 0; i < MAX_ACTION_GROUPS; ++i) {
    JsonObject group = actionGroupsArray.add<JsonObject>();
    group["num"] = actionGroups[i].num;
    group["name"] = actionGroups[i].name;
    JsonArray actArr = group["actionArray"].to<JsonArray>();
    // Assuming actionArray stores indices up to a certain count
    // TODO: Ideally, store count in struct and iterate up to count
    for (uint8_t actIndex : actionGroups[i].actionArray) {
      // Add basic validation or just add the stored value
      actArr.add(actIndex);
    }
    group["status"] = actionGroups[i].status;
  }

  // --- Serialize Rules ---
  JsonArray rulesArray = doc["rules"].to<JsonArray>();
  for (size_t i = 0; i < MAX_RULES; ++i) {
    JsonObject rule = rulesArray.add<JsonObject>();
    rule["num"] = rules[i].num;
    rule["name"] = rules[i].name;
    rule["conditionGroup"] = rules[i].conditionGroup;
    rule["actionGroup"] = rules[i].actionGroup;
    rule["status"] = rules[i].status;
  }

  // --- Serialize Rule Sequence ---
  JsonArray ruleSequenceArray = doc["ruleSequence"].to<JsonArray>();
  // This assumes ruleSequence stores the indices of active rules in order.
  // TODO: Ideally, use a count or dynamic structure for the sequence
  for (size_t i = 0; i < MAX_RULES; ++i) {
    // Add the value stored at this index
    ruleSequenceArray.add(ruleSequence[i]);
  }
}

// --- End Serialization Helper Functions ---

/**
 * @brief Initializes the configuration file in LittleFS if it doesn't exist.
 *        Creates a default config based on the initial state of global
 * variables. Uses helper functions for serialization.
 * @return true if the configuration file exists or was created successfully,
 *         false otherwise.
 */
bool initializeConfiguration() {
  Serial.println("Initializing LittleFS...");
  if (!LittleFS.begin()) {
    Serial.println(
        "ERROR: Failed to mount LittleFS. Configuration cannot be managed.");
    return false;
  }
  Serial.println("LittleFS mounted successfully.");

  if (LittleFS.exists(CONFIG_FILE)) {
    Serial.printf("Configuration file %s already exists.\n", CONFIG_FILE);
    // TODO: Add logic here to LOAD the configuration from the existing file
    // into the global arrays (ioVariables, conditions, actions, etc.).
    // loadConfigurationFromFile(); // Example function call
    return true;
  } else {
    Serial.printf(
        "Configuration file %s not found. Creating default "
        "configuration...\n",
        CONFIG_FILE);

    // --- Populate Global Arrays with Defaults ---
    // 1. Copy default IO Variables into the main global array
    size_t numDefaultIOs = sizeof(defaultIOVariables) / sizeof(IOVariable);
    size_t totalIOs = MAX_DIGITAL_IN + MAX_DIGITAL_OUT + MAX_ANALOG_IN +
                      MAX_SOFTIO + MAX_TIMERS;
    if (numDefaultIOs <= totalIOs) {
      memcpy(ioVariables, defaultIOVariables, sizeof(defaultIOVariables));
      for (size_t i = 0; i < numDefaultIOs; ++i) {
        ioVariables[i].status = true;  // Mark defaults as active
      }
      for (size_t i = numDefaultIOs; i < totalIOs; ++i) {
        ioVariables[i].status = false;  // Mark rest as inactive
                                        // Initialize other fields if necessary
      }
    } else {
      Serial.println("ERROR: Default IO variables exceed total IO capacity!");
    }

    // 2. Initialize other global arrays (Conditions, Actions, etc.)
    for (size_t i = 0; i < MAX_CONDITIONS; ++i) {
      conditions[i].conNum = i;
      snprintf(conditions[i].name, sizeof(conditions[i].name), "Condition %d",
               (int)i + 1);
      conditions[i].status = false;
      conditions[i].Type = SoftIO;   // Default type
      conditions[i].targetNum = 0;   // Default target
      conditions[i].comp = isFalse;  // Default comparison
      conditions[i].value = 0;       // Default value
    }
    for (size_t i = 0; i < MAX_ACTIONS; ++i) {
      actions[i].actNum = i;
      snprintf(actions[i].name, sizeof(actions[i].name), "Action %d",
               (int)i + 1);
      actions[i].status = false;
      actions[i].Type = SoftIO;   // Default type
      actions[i].targetNum = 0;   // Default target
      actions[i].action = clear;  // Default action
      actions[i].value = 0;       // Default value
    }
    for (size_t i = 0; i < MAX_CONDITION_GROUPS; ++i) {
      conditionGroups[i].num = i;
      snprintf(conditionGroups[i].name, sizeof(conditionGroups[i].name),
               "Cond Group %d", (int)i + 1);
      conditionGroups[i].status = false;
      conditionGroups[i].Logic = andLogic;
      memset(conditionGroups[i].conditionArray, 0,
             sizeof(conditionGroups[i].conditionArray));  // Clear array
      memset(conditionGroups[i].resultArray, 0,
             sizeof(conditionGroups[i].resultArray));  // Clear array
    }
    for (size_t i = 0; i < MAX_ACTION_GROUPS; ++i) {
      actionGroups[i].num = i;
      snprintf(actionGroups[i].name, sizeof(actionGroups[i].name),
               "Act Group %d", (int)i + 1);
      actionGroups[i].status = false;
      memset(actionGroups[i].actionArray, 0,
             sizeof(actionGroups[i].actionArray));  // Clear array
    }
    for (size_t i = 0; i < MAX_RULES; ++i) {
      rules[i].num = i;
      snprintf(rules[i].name, sizeof(rules[i].name), "Rule %d", (int)i + 1);
      rules[i].status = false;
      rules[i].conditionGroup = 0;  // Default group index
      rules[i].actionGroup = 0;     // Default group index
    }
    // Initialize ruleSequence (e.g., default order, maybe mark invalid with
    // 255)
    for (size_t i = 0; i < MAX_RULES; ++i) {
      ruleSequence[i] = i;  // Default: sequence all rules initially
                            // Or: ruleSequence[i] = 255; // Indicate empty slot
    }

    // --- Create JSON Document using Helper Functions ---
    JsonDocument doc;  // Use dynamic allocation (default in v7+)

    // *** FIX HERE ***
    // Create the array first and get a reference (lvalue)
    JsonArray ioVarsArray = doc["ioVariables"].to<JsonArray>();
    // Pass the lvalue reference to the function
    serializeIOVariablesToJson(ioVarsArray);

    // Serialize the rest of the logic configuration
    serializeLogicConfigurationToJson(doc);

    // --- Write to File ---
    File file = LittleFS.open(CONFIG_FILE, "w");
    if (!file) {
      Serial.printf("ERROR: Failed to open %s for writing.\n", CONFIG_FILE);
      return false;
    }

    size_t bytesWritten = serializeJson(doc, file);
    file.close();

    if (bytesWritten > 0) {
      Serial.printf(
          "Successfully created default configuration file %s (%d bytes).\n",
          CONFIG_FILE, bytesWritten);
      return true;
    } else {
      Serial.printf("ERROR: Failed to write default configuration to %s.\n",
                    CONFIG_FILE);
      LittleFS.remove(CONFIG_FILE);  // Attempt cleanup
      return false;
    }
  }
}

// --- Add Load/Save Functions (using the helpers) ---

/**
 * @brief Generates a JSON string representing the current configuration/status.
 *        Uses the serialization helper functions.
 * @return String containing the JSON data. Returns "{}" on error.
 */
String generateJsonConfigString() {
  JsonDocument doc;  // Dynamic allocation

  // *** FIX HERE ***
  // Create the array first and get a reference (lvalue)
  JsonArray ioVarsArray = doc["ioVariables"].to<JsonArray>();
  // Pass the lvalue reference to the function
  serializeIOVariablesToJson(ioVarsArray);

  // Serialize the rest of the logic configuration
  serializeLogicConfigurationToJson(doc);  // Single call for the rest

  String output;
  size_t written = serializeJson(doc, output);
  if (written > 0) {
    return output;
  } else {
    Serial.println(
        "ERROR: Failed to serialize current configuration to string.");
    return "{}";  // Return empty JSON object on error
  }
}

// --- Helper String to Enum Converters ---
dataTypes stringToDataType(const char* str) {
  if (strcmp(str, "DigitalInput") == 0) return DigitalInput;
  if (strcmp(str, "DigitalOutput") == 0) return DigitalOutput;
  if (strcmp(str, "AnalogInput") == 0) return AnalogInput;
  if (strcmp(str, "SoftIO") == 0) return SoftIO;
  if (strcmp(str, "Timer") == 0) return Timer;
  Serial.printf("Warning: Unknown data type string '%s'\n", str);
  return SoftIO;  // Default or error value
}

operationMode stringToOperationMode(const char* str) {
  if (strcmp(str, "none") == 0) return none;
  if (strcmp(str, "rising") == 0) return rising;
  if (strcmp(str, "falling") == 0) return falling;
  if (strcmp(str, "stateChange") == 0) return stateChange;
  if (strcmp(str, "pulse") == 0) return pulse;
  if (strcmp(str, "oneShot") == 0) return oneShot;
  if (strcmp(str, "repeating") == 0) return repeating;
  if (strcmp(str, "countdown") == 0) return countdown;
  Serial.printf("Warning: Unknown operation mode string '%s'\n", str);
  return none;  // Default or error value
}

comparisons stringToComparison(const char* str) {
  if (strcmp(str, "isTrue") == 0) return isTrue;
  if (strcmp(str, "isFalse") == 0) return isFalse;
  if (strcmp(str, "isEqual") == 0) return isEqual;
  if (strcmp(str, "isLess") == 0) return isLess;
  if (strcmp(str, "isGreater") == 0) return isGreater;
  if (strcmp(str, "flagIsTrue") == 0) return flagIsTrue;
  if (strcmp(str, "flagIsFalse") == 0) return flagIsFalse;
  Serial.printf("Warning: Unknown comparison string '%s'\n", str);
  return isFalse;  // Default or error value
}

actionType stringToActionType(const char* str) {
  if (strcmp(str, "set") == 0) return set;
  if (strcmp(str, "reset") == 0) return reset;
  if (strcmp(str, "setValue") == 0) return setValue;
  if (strcmp(str, "increment") == 0) return increment;
  if (strcmp(str, "decrement") == 0) return decrement;
  if (strcmp(str, "setFlag") == 0) return setFlag;
  if (strcmp(str, "clear") == 0) return clear;
  Serial.printf("Warning: Unknown action type string '%s'\n", str);
  return clear;  // Default or error value
}

combineLogic stringToCombineLogic(const char* str) {
  if (strcmp(str, "andLogic") == 0) return andLogic;
  if (strcmp(str, "orLogic") == 0) return orLogic;
  Serial.printf("Warning: Unknown combine logic string '%s'\n", str);
  return andLogic;  // Default or error value
}
// --- End Helper String to Enum Converters ---

// --- Parsing Helper Functions ---

/**
 * @brief Parses IOVariables from a JsonArray and updates the global array.
 * @param ioVarsArray The JsonArray containing IOVariable objects.
 * @return true if parsing was successful (or array was empty), false on error.
 */
bool parseIOVariablesFromJson(JsonArray& ioVarsArray) {
  if (ioVarsArray.isNull()) {
    Serial.println("Warning: ioVariables array missing in JSON config.");
    return true;  // Not a fatal error, maybe just not present
  }

  size_t totalIOs = MAX_DIGITAL_IN + MAX_DIGITAL_OUT + MAX_ANALOG_IN +
                    MAX_SOFTIO + MAX_TIMERS;
  size_t parsedCount = 0;

  for (JsonObject ioVarJson : ioVarsArray) {
    dataTypes type = stringToDataType(ioVarJson["type"] | "Unknown");
    uint8_t num = ioVarJson["num"] | 0;

    // Find the corresponding IOVariable in the global array
    bool found = false;
    for (size_t i = 0; i < totalIOs; ++i) {
      if (ioVariables[i].type == type && ioVariables[i].num == num) {
        ioVariables[i].gpio = ioVarJson["gpio"] | 0;
        ioVariables[i].mode = stringToOperationMode(ioVarJson["mode"] | "none");
        strlcpy(ioVariables[i].name, ioVarJson["name"] | "",
                sizeof(ioVariables[i].name));
        // ioVariables[i].state = ioVarJson["state"]; // State is runtime, don't
        // load from config
        ioVariables[i].value = ioVarJson["value"] | 0;
        ioVariables[i].flag = ioVarJson["flag"] | false;
        ioVariables[i].status =
            ioVarJson["status"] | false;  // Load enabled status
        found = true;
        parsedCount++;
        break;
      }
    }
    if (!found) {
      Serial.printf(
          "Warning: IOVariable type %s num %d from JSON not found in global "
          "array.\n",
          dataTypeToString(type), num);
    }
  }
  Serial.printf("Parsed %d IOVariables from JSON.\n", parsedCount);
  return true;  // Consider successful even if some weren't found (maybe
                // defaults changed)
}

/**
 * @brief Parses logic configuration (conditions, actions, groups, rules,
 * sequence) from a JsonDocument and updates the global arrays.
 * @param doc The JsonDocument containing the configuration.
 * @return true if parsing was generally successful, false on major errors.
 */
bool parseLogicConfigurationFromJson(JsonDocument& doc) {
  bool success = true;

  // --- Parse Conditions ---
  JsonArray conditionsArray = doc["conditions"];
  if (!conditionsArray.isNull()) {
    size_t index = 0;
    for (JsonObject conditionJson : conditionsArray) {
      if (index >= MAX_CONDITIONS) {
        Serial.println(
            "ERROR: More conditions in JSON than MAX_CONDITIONS allow.");
        success = false;
        break;
      }
      uint8_t conNum =
          conditionJson["conNum"] | index;  // Use index as fallback
      if (conNum >= MAX_CONDITIONS) {
        Serial.printf("ERROR: Condition conNum %d out of bounds.\n", conNum);
        success = false;
        continue;
      }
      conditions[conNum].conNum = conNum;  // Ensure consistency
      strlcpy(conditions[conNum].name, conditionJson["name"] | "",
              sizeof(conditions[conNum].name));
      conditions[conNum].Type =
          stringToDataType(conditionJson["Type"] | "SoftIO");
      conditions[conNum].targetNum = conditionJson["targetNum"] | 0;
      conditions[conNum].comp =
          stringToComparison(conditionJson["comp"] | "isFalse");
      conditions[conNum].value = conditionJson["value"] | 0;
      conditions[conNum].status = conditionJson["status"] | false;
      index++;
    }
    Serial.printf("Parsed %d Conditions from JSON.\n", index);
  } else {
    Serial.println("Warning: conditions array missing in JSON config.");
  }

  // --- Parse Actions ---
  JsonArray actionsArray = doc["actions"];
  if (!actionsArray.isNull()) {
    size_t index = 0;
    for (JsonObject actionJson : actionsArray) {
      if (index >= MAX_ACTIONS) {
        Serial.println("ERROR: More actions in JSON than MAX_ACTIONS allow.");
        success = false;
        break;
      }
      uint8_t actNum = actionJson["actNum"] | index;  // Use index as fallback
      if (actNum >= MAX_ACTIONS) {
        Serial.printf("ERROR: Action actNum %d out of bounds.\n", actNum);
        success = false;
        continue;
      }
      actions[actNum].actNum = actNum;  // Ensure consistency
      strlcpy(actions[actNum].name, actionJson["name"] | "",
              sizeof(actions[actNum].name));
      actions[actNum].Type = stringToDataType(actionJson["Type"] | "SoftIO");
      actions[actNum].targetNum = actionJson["targetNum"] | 0;
      actions[actNum].action =
          stringToActionType(actionJson["action"] | "clear");
      actions[actNum].value = actionJson["value"] | 0;
      actions[actNum].status = actionJson["status"] | false;
      index++;
    }
    Serial.printf("Parsed %d Actions from JSON.\n", index);
  } else {
    Serial.println("Warning: actions array missing in JSON config.");
  }

  // --- Parse Condition Groups ---
  JsonArray conditionGroupsArray = doc["conditionGroups"];
  if (!conditionGroupsArray.isNull()) {
    size_t index = 0;
    for (JsonObject groupJson : conditionGroupsArray) {
      if (index >= MAX_CONDITION_GROUPS) {
        Serial.println(
            "ERROR: More condition groups in JSON than MAX_CONDITION_GROUPS "
            "allow.");
        success = false;
        break;
      }
      uint8_t num = groupJson["num"] | index;  // Use index as fallback
      if (num >= MAX_CONDITION_GROUPS) {
        Serial.printf("ERROR: Condition Group num %d out of bounds.\n", num);
        success = false;
        continue;
      }
      conditionGroups[num].num = num;  // Ensure consistency
      strlcpy(conditionGroups[num].name, groupJson["name"] | "",
              sizeof(conditionGroups[num].name));
      conditionGroups[num].Logic =
          stringToCombineLogic(groupJson["Logic"] | "andLogic");
      conditionGroups[num].status = groupJson["status"] | false;

      JsonArray condArrJson = groupJson["conditionArray"];
      memset(conditionGroups[num].conditionArray, 0,
             sizeof(conditionGroups[num].conditionArray));  // Clear old values
      size_t condIndex = 0;
      if (!condArrJson.isNull()) {
        for (JsonVariant val : condArrJson) {
          if (condIndex < sizeof(conditionGroups[num].conditionArray) /
                              sizeof(conditionGroups[num].conditionArray[0])) {
            conditionGroups[num].conditionArray[condIndex++] =
                val.as<uint8_t>();
          } else {
            Serial.printf(
                "Warning: Too many conditions in group %d, truncating.\n", num);
            break;
          }
        }
      }
      index++;
    }
    Serial.printf("Parsed %d Condition Groups from JSON.\n", index);
  } else {
    Serial.println("Warning: conditionGroups array missing in JSON config.");
  }

  // --- Parse Action Groups ---
  JsonArray actionGroupsArray = doc["actionGroups"];
  if (!actionGroupsArray.isNull()) {
    size_t index = 0;
    for (JsonObject groupJson : actionGroupsArray) {
      if (index >= MAX_ACTION_GROUPS) {
        Serial.println(
            "ERROR: More action groups in JSON than MAX_ACTION_GROUPS allow.");
        success = false;
        break;
      }
      uint8_t num = groupJson["num"] | index;  // Use index as fallback
      if (num >= MAX_ACTION_GROUPS) {
        Serial.printf("ERROR: Action Group num %d out of bounds.\n", num);
        success = false;
        continue;
      }
      actionGroups[num].num = num;  // Ensure consistency
      strlcpy(actionGroups[num].name, groupJson["name"] | "",
              sizeof(actionGroups[num].name));
      actionGroups[num].status = groupJson["status"] | false;

      JsonArray actArrJson = groupJson["actionArray"];
      memset(actionGroups[num].actionArray, 0,
             sizeof(actionGroups[num].actionArray));  // Clear old values
      size_t actIndex = 0;
      if (!actArrJson.isNull()) {
        for (JsonVariant val : actArrJson) {
          if (actIndex < sizeof(actionGroups[num].actionArray) /
                             sizeof(actionGroups[num].actionArray[0])) {
            actionGroups[num].actionArray[actIndex++] = val.as<uint8_t>();
          } else {
            Serial.printf(
                "Warning: Too many actions in group %d, truncating.\n", num);
            break;
          }
        }
      }
      index++;
    }
    Serial.printf("Parsed %d Action Groups from JSON.\n", index);
  } else {
    Serial.println("Warning: actionGroups array missing in JSON config.");
  }

  // --- Parse Rules ---
  JsonArray rulesArray = doc["rules"];
  if (!rulesArray.isNull()) {
    size_t index = 0;
    for (JsonObject ruleJson : rulesArray) {
      if (index >= MAX_RULES) {
        Serial.println("ERROR: More rules in JSON than MAX_RULES allow.");
        success = false;
        break;
      }
      uint8_t num = ruleJson["num"] | index;  // Use index as fallback
      if (num >= MAX_RULES) {
        Serial.printf("ERROR: Rule num %d out of bounds.\n", num);
        success = false;
        continue;
      }
      rules[num].num = num;  // Ensure consistency
      strlcpy(rules[num].name, ruleJson["name"] | "", sizeof(rules[num].name));
      rules[num].conditionGroup = ruleJson["conditionGroup"] | 0;
      rules[num].actionGroup = ruleJson["actionGroup"] | 0;
      rules[num].status = ruleJson["status"] | false;
      index++;
    }
    Serial.printf("Parsed %d Rules from JSON.\n", index);
  } else {
    Serial.println("Warning: rules array missing in JSON config.");
  }

  // --- Parse Rule Sequence ---
  JsonArray ruleSequenceArray = doc["ruleSequence"];
  if (!ruleSequenceArray.isNull()) {
    size_t index = 0;
    // Initialize sequence with an invalid marker (e.g., 255) or 0 if 0 is
    // invalid
    memset(ruleSequence, 255,
           sizeof(ruleSequence));  // Assuming 255 is not a valid rule index
    for (JsonVariant val : ruleSequenceArray) {
      if (index >= MAX_RULES) {
        Serial.println(
            "ERROR: More rule sequence entries in JSON than MAX_RULES allow.");
        success = false;
        break;
      }
      ruleSequence[index++] = val.as<uint8_t>();
    }
    Serial.printf("Parsed %d Rule Sequence entries from JSON.\n", index);
  } else {
    Serial.println("Warning: ruleSequence array missing in JSON config.");
    // Optionally set a default sequence if missing
    // for (size_t i = 0; i < MAX_RULES; ++i) ruleSequence[i] = i;
  }

  return success;
}

// --- End Parsing Helper Functions ---

/**
 * @brief Parses a JSON configuration string and updates the global data
 * structures.
 * @param jsonString The JSON string containing the configuration.
 * @return true if parsing was successful, false otherwise.
 */
bool parseJsonConfigString(const String& jsonString) {
  if (jsonString.length() == 0) {
    Serial.println("ERROR: Cannot parse empty JSON string.");
    return false;
  }

  // Allocate JsonDocument. Adjust size as needed.
  // Start with 8k, increase if parsing fails due to NoMemory
  // DynamicJsonDocument doc(8192); // ArduinoJson v6
  JsonDocument doc;  // ArduinoJson v7+ (dynamic by default)

  DeserializationError error = deserializeJson(doc, jsonString);

  if (error) {
    Serial.print(F("ERROR: deserializeJson() failed: "));
    Serial.println(error.c_str());
    return false;
  }

  // Parse IO Variables
  JsonArray ioVarsArray = doc["ioVariables"];
  if (!parseIOVariablesFromJson(ioVarsArray)) {
    // Decide if this is fatal - maybe allow continuing?
    // return false;
  }

  // Parse the rest of the logic configuration
  if (!parseLogicConfigurationFromJson(doc)) {
    // Decide if this is fatal
    // return false;
  }

  Serial.println("Successfully parsed JSON configuration string.");
  return true;
}

/**
 * @brief Loads the configuration from the CONFIG_FILE in LittleFS.
 *        If the file exists, it reads it and attempts to parse the JSON content
 *        to populate the global data structures.
 * @return true if the configuration was successfully loaded and parsed,
 *         false if the file doesn't exist, couldn't be opened, or parsing
 * failed.
 */
bool loadConfigurationFromFile() {
  if (!LittleFS.exists(CONFIG_FILE)) {
    Serial.printf("Info: Configuration file %s does not exist. Cannot load.\n",
                  CONFIG_FILE);
    // This might not be an error if initializeConfiguration will run next
    return false;
  }

  File file = LittleFS.open(CONFIG_FILE, "r");
  if (!file) {
    Serial.printf("ERROR: Failed to open %s for reading.\n", CONFIG_FILE);
    return false;
  }

  String jsonString = file.readString();
  file.close();

  if (jsonString.length() == 0) {
    Serial.printf("ERROR: Configuration file %s is empty.\n", CONFIG_FILE);
    return false;
  }

  Serial.printf("Loading configuration from %s (%d bytes)...\n", CONFIG_FILE,
                jsonString.length());
  return parseJsonConfigString(jsonString);
}

/**
 * @brief Saves the current state of the global configuration variables
 *        to the CONFIG_FILE in LittleFS as a JSON string.
 * @return true if the configuration was successfully generated and saved,
 *         false otherwise.
 */
bool saveConfigurationToFile() {
  String jsonString = generateJsonConfigString();

  if (jsonString == "{}" ||
      jsonString.length() < 3) {  // Basic check for empty/error string
    Serial.println(
        "ERROR: Failed to generate JSON config string. Cannot save.");
    return false;
  }

  File file = LittleFS.open(CONFIG_FILE, "w");
  if (!file) {
    Serial.printf("ERROR: Failed to open %s for writing.\n", CONFIG_FILE);
    return false;
  }

  size_t bytesWritten = file.print(jsonString);
  file.close();

  if (bytesWritten == jsonString.length()) {
    Serial.printf("Successfully saved configuration to %s (%d bytes).\n",
                  CONFIG_FILE, bytesWritten);
    return true;
  } else {
    Serial.printf(
        "ERROR: Failed to write complete configuration to %s (wrote %d / %d "
        "bytes).\n",
        CONFIG_FILE, bytesWritten, jsonString.length());
    // Attempt cleanup? Maybe not safe if partially written.
    // LittleFS.remove(CONFIG_FILE);
    return false;
  }
}
