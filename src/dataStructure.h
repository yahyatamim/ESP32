#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include "configPortal.h"

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
  dataTypes type;  // Type of the variable (DigitalInput, DigitalOutput,
                   // AnalogInput, SoftIO, Timer)
  uint8_t gpio;    // GPIO pin if applicable (used for physical inputs/outputs)
  operationMode mode;  // Additional operating mode attribute (defines behavior
                       // based on type)
  char name[20];       // Human-friendly name for the variable (e.g., "Sensor1",
                       // "Relay1")
  bool state;     // Boolean state (e.g., ON/OFF for Digital IO, active/inactive
                  // for Timer)
  int32_t value;  // Primary value (e.g., current reading for AnalogInput,
                  // counter value for SoftIO)
  bool flag;      // Timer completed, Digital Input Inturrpt Flag
};

// Create a default Map
IOVariable defaultIOVariables[] = {
    {1, DigitalInput, 4, none, "Input1", false, 0, false},
    {2, DigitalInput, 2, none, "Input2", false, 0, false},
    {3, DigitalInput, 15, none, "Input3", false, 0, false},
    {4, DigitalInput, 13, none, "Input4", false, 0, false},
    {5, DigitalInput, 12, none, "Input5", false, 0, false},
    {6, DigitalInput, 14, none, "Input6", false, 0, false},
    {1, DigitalOutput, 27, none, "Output1", false, 0, false},
    {2, DigitalOutput, 26, none, "Output2", false, 0, false},
    {3, DigitalOutput, 25, none, "Output3", false, 0, false},
    {4, DigitalOutput, 33, none, "Output4", false, 0, false},
    {1, AnalogInput, 35, none, "Analog1", false, 0, false},
    {2, AnalogInput, 34, none, "Analog2", false, 0, false},
    {3, AnalogInput, 39, none, "Analog3", false, 0, false},
    {4, AnalogInput, 36, none, "Analog4", false, 0, false},
    {1, SoftIO, 0, none, "SIO1", false, 0, false},
    {2, SoftIO, 0, none, "SIO2", false, 0, false},
    {3, SoftIO, 0, none, "SIO3", false, 0, false},
    {4, SoftIO, 0, none, "SIO4", false, 0, false},
    {5, SoftIO, 0, none, "SIO5", false, 0, false},
    {6, SoftIO, 0, none, "SIO6", false, 0, false},
    {7, SoftIO, 0, none, "SIO7", false, 0, false},
    {8, SoftIO, 0, none, "SIO8", false, 0, false},
    {9, SoftIO, 0, none, "SIO9", false, 0, false},
    {10, SoftIO, 0, none, "SIO10", false, 0, false},
    {11, SoftIO, 0, none, "SIO11", false, 0, false},
    {12, SoftIO, 0, none, "SIO12", false, 0, false},
    {13, SoftIO, 0, none, "SIO13", false, 0, false},
    {14, SoftIO, 0, none, "SIO14", false, 0, false},
    {15, SoftIO, 0, none, "SIO15", false, 0, false},
    {16, SoftIO, 0, none, "SIO16", false, 0, false},
    {17, SoftIO, 0, none, "SIO17", false, 0, false},
    {18, SoftIO, 0, none, "SIO18", false, 0, false},
    {19, SoftIO, 0, none, "SIO19", false, 0, false},
    {20, SoftIO, 0, none, "SIO20", false, 0, false},
    {1, Timer, 0, oneShot, "Timer1", false, 0, false},
    {2, Timer, 0, oneShot, "Timer2", false, 0, false},
    {3, Timer, 0, oneShot, "Timer3", false, 0, false},
    {4, Timer, 0, oneShot, "Timer4", false, 0, false},
    {5, Timer, 0, oneShot, "Timer5", false, 0, false},
    {6, Timer, 0, oneShot, "Timer6", false, 0, false},
    {7, Timer, 0, oneShot, "Timer7", false, 0, false},
    {8, Timer, 0, oneShot, "Timer8", false, 0, false},
    {9, Timer, 0, oneShot, "Timer9", false, 0, false},
    {10, Timer, 0, oneShot, "Timer10", false, 0, false},
    {11, Timer, 0, oneShot, "Timer11", false, 0, false},
    {12, Timer, 0, oneShot, "Timer12", false, 0, false},
    {13, Timer, 0, oneShot, "Timer13", false, 0, false},
    {14, Timer, 0, oneShot, "Timer14", false, 0, false},
    {15, Timer, 0, oneShot, "Timer15", false, 0, false},
    {16, Timer, 0, oneShot, "Timer16", false, 0, false},
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
  uint8_t conNum;  // Unique identifier for the condition
  char name[20];   // Human-readable name for the condition
  dataTypes
      Type;  // Type of the first variable (e.g., DigitalInput, Timer, etc.)
  uint8_t targetNum;  // Identifier for the first variable
  comparisons comp;   // Comparison type (e.g., isEqual, isGreater, etc.)
  int32_t value;      // Constant value for comparison
};
condition conditions[MAX_CONDITIONS];

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
  uint8_t actNum;  // Unique identifier for the action
  char name[20];   // Human-readable name for the action (e.g., "Increment
                   // Counter")
  dataTypes
      Type;  // Type of the first variable (e.g., DigitalInput, Timer, etc.)
  uint8_t targetNum;  // Target variable number (index in the IOVariables array)
  actionType
      action;  // Type of action to perform (e.g., set, increment, toggle)
  int32_t
      value;  // Value to use for the action (e.g., increment amount, threshold)
};
action actions[MAX_ACTIONS];

enum combineLogic {
  andLogic,  // All conditions in the group must be true
  orLogic    // At least one condition in the group must be true
};

struct conditionGroup {
  uint8_t num;                 // Unique identifier for the condition group
  char name[20];               // Human-readable name for the group
  uint8_t conditionArray[10];  // Array of condition indices (from `Conditions`)
  bool resultArray[10];        // Array of condition results (true/false)
  combineLogic Logic;          // Logic to combine conditions (andLogic/orLogic)
};
conditionGroup conditionGroups[MAX_CONDITION_GROUPS];

struct actionGroup {
  uint8_t num;              // Unique identifier for the action group
  char name[20];            // Human-readable name for the group
  uint8_t actionArray[10];  // Array of action indices (from `Actions`)
};
actionGroup actionGroups[MAX_ACTION_GROUPS];

struct rule {
  uint8_t num;             // Unique identifier for the rule
  char name[20];           // Human-readable name for the rule
  uint8_t conditionGroup;  // Index of the associated condition group
  uint8_t actionGroup;     // Index of the associated action group
  bool status;             // Status of the rule (active/inactive)
};
rule rules[MAX_RULES];

uint8_t ruleSequence[MAX_RULES];

#endif