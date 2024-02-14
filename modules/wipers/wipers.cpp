//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"
#include "wipers.h"
#include "ignition.h"
#include "display.h"
#include "motor.h"
#include <string>

//=====[Declaration of private defines]========================================

//=====[Declaration of private data types]=====================================

typedef enum {
    HI, LO, INT, W_OFF
} wiperSelector_t;

typedef enum {
   SHORT, MEDIUM, LONG
} intervalSelector_t;

//=====[Declaration and initialization of public global objects]===============

// Inputs

AnalogIn wiperModeSelector(A0);
AnalogIn intervalModeSelector(A1);

// Outputs

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

wiperSelector_t selectedMode = W_OFF;
intervalSelector_t selectedIntMode = SHORT;

//=====[Declarations (prototypes) of private functions]========================

void readWiperSelectorPot();
void readIntervalSelectorPot();
void displaySelectedMode();
string modeToString();
string intervalToString();

//=====[Implementations of public functions]===================================

void wipersInit();
void checkWiperSubsystem();

//=====[Implementations of private functions]==================================

void wipersInit() {
    displayInit();
    displayCharPositionWrite ( 0,0 );
    displayStringWrite( "Mode:" );
    displayCharPositionWrite ( 0,1 );
    displayStringWrite( "Speed:" );
}

/***
    Run logic for checking wiper subsystem
***/
void checkWiperSubsystem() {
    if(isEngineOn()) {
        readWiperSelectorPot();

        if(selectedMode == INT) {
            
        } else if(selectedMode == LO) {
            performLowSpeedCycle();
        } else if(selectedMode == HI) {
            performHighSpeedCycle();  
        } else {
            // do nothing
        }

    } else {
        selectedMode = W_OFF;
    }

    displaySelectedMode();
}

void displaySelectedMode() {
    displayCharPositionWrite ( 6,0 );
    displayStringWrite( modeToString().c_str() );
    displayCharPositionWrite ( 7,1 );
    displayStringWrite( intervalToString().c_str() );
}

void readWiperSelectorPot() {
    float selectorPot = wiperModeSelector.read();

    if(selectorPot < 0.25) {
        selectedMode = HI;
    } else if(selectorPot < 0.5) {
        selectedMode = LO;
    } else if(selectorPot < 0.75) {
        selectedMode = INT;
    } else {
        selectedMode = W_OFF;
    }
}

void readIntervalSelectorPot() {
    float selectorPot = intervalModeSelector.read();

    if(selectorPot < 0.33) {
        selectedIntMode = SHORT;
    } else if(selectorPot < 0.66) {
        selectedIntMode = MEDIUM;
    } else {
        selectedIntMode = LONG;
    }
}

string modeToString() {
    switch(selectedMode) {
        case HI:
            return "High";
        case LO:
            return "Low";
        case INT:
            return "Int.";
        default:
            return "Off";
    }
}

string intervalToString() {
    switch(selectedIntMode) {
        case SHORT:
            return "Short";
        case MEDIUM:
            return "Medium";
        default:
            return "Long";
    }
}