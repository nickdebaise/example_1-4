//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"
#include "wipers.h"
#include "ignition.h"
#include "display.h"

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
AnalogIn intervalSelector(A1);

// Outputs

PwmOut wipers(PF_9);

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

wiperSelector_t selectedMode = W_OFF;
intervalSelector_t selectedIntMode = SHORT;

//=====[Declarations (prototypes) of private functions]========================

void readWiperSelectorPot();
void readIntervalSelectorPot();
void displaySelectedMode();
String modeToString();
String intervalToString();

//=====[Implementations of public functions]===================================

void wiperInit();
void checkWiperSubsystem();

//=====[Implementations of private functions]==================================

void wiperInit() {
    displayInit();
    displayCharPositionWrite ( 0,0 );
    displayStringWrite( "Mode:" );
    displayCharPositionWrite ( 1,0 );
    displayStringWrite( "Speed:" );
}

/***
    Run logic for checking wiper subsystem
***/
void checkWiperSubsystem() {
    if(isEngineOn()) {
        readWiperSelectorPot();
    } else {
        selectedMode = W_OFF;
    }

    displaySelectedMode();
}

void displaySelectedMode() {
    displayCharPositionWrite ( 0,6 );
    displayStringWrite( modeToString() );
    displayCharPositionWrite ( 1,7 );
    displayStringWrite( intervalToString() );
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
    float selectorPot = intervalSelector.read();

    if(selectorPot < 0.33) {
        selectedIntMode = SHORT;
    } else if(selectorPot < 0.66) {
        selectedIntMode = MEDIUM;
    } else {
        selectedIntMode = LONG;
    }
}

String modeToString() {
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

String intervalToString() {
    switch(selectedIntMode) {
        case SHORT:
            return "Short";
        case MEDIUM:
            return "Medium";
        default:
            return "Long";
    }
}