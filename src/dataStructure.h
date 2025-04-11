#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include <Arduino.h>

// Define maximum number of variables
#define MAX_DIGITAL_IN 6
#define MAX_DIGITAL_OUT 4
#define MAX_ANALOG_IN 4
#define MAX_SOFTIO 20
#define MAX_TIMERS 16
#define MAX_CONDITIONS 50
#define MAX_CONDITION_GROUPS 20
#define MAX_ACTIONS 50
#define MAX_ACTION_GROUPS 20
#define MAX_RULES 20

enum dataTypes {
  DigitalInput,   // Represents a physical digital input (e.g., buttons,
                  // switches)
  DigitalOutput,  // Represents a physical digital output (e.g., LEDs, relays)
  AnalogInput,    // Represents a physical analog input (e.g., sensors like
                  // temperature, light)
  SoftIO,  // Represents a virtual input/output controlled by software (e.g.,
           // counters, virtual flags)
  Timer    // Represents a timer for scheduling or time-based operations
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
  uint8_t num;     // Variable number or index (unique identifier)
  dataTypes type;  // Type of the variable
  uint8_t gpio;    // GPIO pin if applicable (used for physical inputs/outputs)
  operationMode mode;  // Additional operating mode attribute
  char name[30];       // Human-friendly name for the variable
  bool state;          // Boolean state
  int32_t value;       // Primary value
  bool flag;           // Timer completed, Digital Input Inturrpt Flag
  bool status;         // Indicate if this variable is initiated or deleted.
};

enum comparisons {
  isTrue,       // Checks if the state is true
  isFalse,      // Checks if the state is false
  isEqual,      // Checks if two values are equal
  isLess,       // Checks if a value is less than another
  isGreater,    // Checks if a value is greater than another
  flagIsTrue,   // Checks if a flag is set
  flagIsFalse,  // Checks if a flag is set
};

struct condition {
  uint8_t conNum = 0;          // Unique identifier for the condition
  char name[20];               // Human-readable name for the condition
  dataTypes Type = SoftIO;     // Type of the first variable
  uint8_t targetNum = 0;       // Identifier for the first variable
  comparisons comp = isFalse;  // Comparison type
  int32_t value = 0;           // Constant value for comparison
  bool status = false;  // Indicate if this condition is initiated or deleted.
};

enum actionType {
  set,        // Set state to true
  reset,      // Set state to false
  setValue,   // Set value1 to a number
  increment,  // Increase value by specified number
  decrement,  // Decrease value1 by specified
  setFlag,    // Set flag to true
  clear,      // Clear flag
};

struct action {
  uint8_t actNum = 0;         // Unique identifier for the action
  char name[20];              // Human-readable name for the action
  dataTypes Type = SoftIO;    // Type of the first variable
  uint8_t targetNum = 0;      // Target variable number
  actionType action = clear;  // Type of action to perform
  int32_t value = 0;          // Value to use for the action
  bool status = false;  // Indicate if this action is initiated or deleted.
};

enum combineLogic {
  andLogic,  // All conditions in the group must be true
  orLogic    // At least one condition in the group must be true
};

struct conditionGroup {
  uint8_t num = 0;                // Unique identifier for the condition group
  char name[20];                  // Human-readable name for the group
  uint8_t conditionArray[10];     // Array of condition indices
  bool resultArray[10];           // Array of condition results (true/false)
  combineLogic Logic = andLogic;  // Logic to combine conditions
  bool status = false;  // Indicate if this group is initiated or deleted.
};

struct actionGroup {
  uint8_t num = 0;          // Unique identifier for the action group
  char name[20];            // Human-readable name for the group
  uint8_t actionArray[10];  // Array of action indices (from `Actions`)
  bool status = false;      // Indicate if this group is initiated or deleted.
};

struct rule {
  uint8_t num = 0;         // Unique identifier for the rule
  char name[20];           // Human-readable name for the rule
  uint8_t conditionGroup;  // Index of the associated condition group
  uint8_t actionGroup;     // Index of the associated action group
  bool status = false;     // Status of the rule (active/inactive)
};

extern IOVariable defaultIOVariables[];
extern condition conditions[MAX_CONDITIONS];
extern conditionGroup conditionGroups[MAX_CONDITION_GROUPS];
extern action actions[MAX_ACTIONS];
extern actionGroup actionGroups[MAX_ACTION_GROUPS];
extern rule rules[MAX_RULES];
extern uint8_t ruleSequence[MAX_RULES];

#endif