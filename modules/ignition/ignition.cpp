//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"
#include "ignition.h"

//=====[Declaration of private defines]========================================

//=====[Declaration of private data types]=====================================

typedef enum {
    BUTTON_IDLE,
    BUTTON_PRESSED,
    BUTTON_RELEASED
} ignitionButtonState_t;

//=====[Declaration and initialization of public global objects]===============

// Inputs

DigitalIn driverSeatSensor(D14);
DigitalIn ignitionButton(USER_BUTTON);

// Outputs

DigitalOut ignitionLed(LED2);

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

bool engineOn = OFF;

ignitionButtonState_t buttonState = BUTTON_IDLE;

//=====[Declarations (prototypes) of private functions]========================

void ignitionInit();
void checkIgnitionSubsystem();

bool isEngineOn();

//=====[Implementations of public functions]===================================

//=====[Implementations of private functions]==================================

void ignitionInit() {
    driverSeatSensor.mode(PullDown);
    ignitionLed = OFF;
}

/***
    Run logic for checking ignition subsystem
    - logic for turning engine on/off
    - logic for turning blue indicator on/off
***/
void checkIgnitionSubsystem() {
    switch(buttonState) {
        case BUTTON_IDLE:
            if(driverSeatSensor == ON && ignitionButton == ON) {
                buttonState = BUTTON_PRESSED;
            }
            break;
        case BUTTON_PRESSED:
            if(ignitionButton == OFF) {
                if(driverSeatSensor == ON) {
                    buttonState = BUTTON_RELEASED;
                } else {
                    buttonState = BUTTON_IDLE;
                }
            }
            break;
        case BUTTON_RELEASED:
            engineOn = !engineOn;
            buttonState = BUTTON_IDLE;
    }
    ignitionLed = isEngineOn();
}

bool isEngineOn() {
    return engineOn;
}