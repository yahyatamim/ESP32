#include "dataStructure.h"

#include <ArduinoJson.h>  // Include ArduinoJson
#include <LittleFS.h>     // Include LittleFS

// Create a default Map
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

// Helper function to convert dataTypes enum to string
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

// Helper function to convert operationMode enum to string
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

// Helper function to convert comparison enum to string
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

// Helper function to convert actionType enum to string
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

// Helper function to convert combineLogic enum to string
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

/**
 * @brief Initializes the configuration file in LittleFS if it doesn't exist.
 *        Creates a default config based on the initial state of global
 * variables.
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
    return true;  // File exists, initialization not needed, but successful
                  // state
  } else {
    Serial.printf(
        "Configuration file %s not found. Creating default "
        "configuration...\n",
        CONFIG_FILE);

    // --- Use ArduinoJson v7+ API ---
    // JsonDocument automatically manages memory (dynamically by default)
    // No need to pre-calculate capacity with JSON_OBJECT_SIZE/JSON_ARRAY_SIZE
    JsonDocument doc;

    // --- Populate ioVariables Array ---
    // Use doc[key].to<Type>() syntax to create/get arrays/objects
    JsonArray ioVarsArray = doc["ioVariables"].to<JsonArray>();

    // Calculate the actual number of elements in defaultIOVariables
    size_t numDefaultIOs = sizeof(defaultIOVariables) / sizeof(IOVariable);
    for (size_t i = 0; i < numDefaultIOs; ++i) {
      // Use array.add<Type>() to add elements to an array
      JsonObject ioVar = ioVarsArray.add<JsonObject>();
      ioVar["num"] = defaultIOVariables[i].num;
      ioVar["type"] = dataTypeToString(defaultIOVariables[i].type);
      ioVar["gpio"] = defaultIOVariables[i].gpio;
      ioVar["mode"] = operationModeToString(defaultIOVariables[i].mode);
      ioVar["name"] = defaultIOVariables[i].name;
      ioVar["state"] = defaultIOVariables[i].state;
      ioVar["value"] = defaultIOVariables[i].value;
      ioVar["flag"] = defaultIOVariables[i].flag;
      ioVar["status"] = defaultIOVariables[i].status;
    }

    JsonArray conditionsArray = doc["conditions"].to<JsonArray>();
    for (size_t i = 0; i < MAX_CONDITIONS; ++i) {
      JsonObject condition = conditionsArray.add<JsonObject>();
      condition["conNum"] = i;
      condition["name"] = String("Condition ") + String(i + 1);
      condition["Type"] = dataTypeToString(conditions[i].Type);
      condition["targetNum"] = conditions[i].targetNum;
      condition["comp"] = comparisonToString(conditions[i].comp);
      condition["value"] = conditions[i].value;
      condition["status"] = conditions[i].status;
    }

    JsonArray actionsArray = doc["actions"].to<JsonArray>();
    for (size_t i = 0; i < MAX_ACTIONS; ++i) {
      JsonObject action = actionsArray.add<JsonObject>();
      action["actNum"] = i;
      action["name"] = String("Action ") + String(i + 1);
      action["Type"] = dataTypeToString(actions[i].Type);
      action["targetNum"] = actions[i].targetNum;
      action["action"] = actionTypeToString(actions[i].action);
      action["value"] = actions[i].value;
      action["status"] = actions[i].status;
    }

    // --- Populate Condition Groups Array ---
    JsonArray conditionGroupsArray = doc["conditionGroups"].to<JsonArray>();
    for (size_t i = 0; i < MAX_CONDITION_GROUPS; ++i) {
      JsonObject group = conditionGroupsArray.add<JsonObject>();
      group["num"] = i;  // Use index as the default number/ID
      group["name"] = String("Cond Group ") + String(i + 1);  // Default name
      // Create empty arrays for conditions and results within the group
      group["conditionArray"].to<JsonArray>();  // Empty array initially
      group["resultArray"].to<JsonArray>();     // Empty array initially
      // Use default logic (e.g., andLogic) or read from global default if
      // needed
      group["Logic"] = combineLogicToString(
          conditionGroups[i].Logic);  // Assumes global array is initialized
      group["status"] =
          conditionGroups[i].status;  // Default status (likely false)
    }

    // --- Populate Action Groups Array ---
    JsonArray actionGroupsArray = doc["actionGroups"].to<JsonArray>();
    for (size_t i = 0; i < MAX_ACTION_GROUPS; ++i) {
      JsonObject group = actionGroupsArray.add<JsonObject>();
      group["num"] = i;  // Use index as the default number/ID
      group["name"] = String("Act Group ") + String(i + 1);  // Default name
      // Create empty array for actions within the group
      group["actionArray"].to<JsonArray>();  // Empty array initially
      group["status"] =
          actionGroups[i].status;  // Default status (likely false)
    }

    // --- Populate Rules Array ---
    JsonArray rulesArray = doc["rules"].to<JsonArray>();
    for (size_t i = 0; i < MAX_RULES; ++i) {
      JsonObject rule = rulesArray.add<JsonObject>();
      rule["num"] = i;  // Use index as the default number/ID
      rule["name"] = String("Rule ") + String(i + 1);  // Default name
      // Default to the first group (index 0) or an invalid index like -1 if
      // preferred
      rule["conditionGroup"] =
          rules[i]
              .conditionGroup;  // Assumes global array is initialized (often 0)
      rule["actionGroup"] =
          rules[i]
              .actionGroup;  // Assumes global array is initialized (often 0)
      rule["status"] = rules[i].status;  // Default status (likely false)
    }

    // --- Populate Rule Sequence Array ---
    // This array typically holds the execution order of *enabled* rules.
    // Populating it 0 to MAX_RULES-1 implies all rules are initially sequenced.
    // An empty array [] might be more logical, but following the request to
    // populate:
    JsonArray ruleSequenceArray = doc["ruleSequence"].to<JsonArray>();
    for (size_t i = 0; i < MAX_RULES; ++i) {
      ruleSequenceArray.add(i);  // Add the rule index (0, 1, 2, ...)
    }

    // --- Write to File ---
    File file = LittleFS.open(CONFIG_FILE, "w");
    if (!file) {
      Serial.printf("ERROR: Failed to open %s for writing.\n", CONFIG_FILE);
      return false;
    }

    size_t bytesWritten = serializeJson(doc, file);
    file.close();  // Close the file immediately after writing

    if (bytesWritten > 0) {
      Serial.printf(
          "Successfully created default configuration file %s (%d bytes).\n",
          CONFIG_FILE, bytesWritten);
      return true;
    } else {
      Serial.printf("ERROR: Failed to write default configuration to %s.\n",
                    CONFIG_FILE);
      // Attempt to remove the potentially corrupted file
      LittleFS.remove(CONFIG_FILE);
      return false;
    }
  }
}
