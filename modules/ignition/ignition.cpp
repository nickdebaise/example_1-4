//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"
#include "ignition.h"

//=====[Declaration of private defines]========================================

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

// Inputs

DigitalIn driverSeatSensor(D10);
DigitalIn ignitionButton(USER_BUTTON);

// Outputs

DigitalOut ignitionLed(LED2);

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

bool engineOn = OFF;

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
    if(driverSeatSensor == ON && ignitionButton == ON && !isEngineOn()) {
        while(ignitionButton == ON) {} // delay until it goes off
        // Ignition is released
        engineOn = ON;
    }

    if(ignitionButton == ON && isEngineOn()) {
        while(ignitionButton == ON) {}
        engineOn = OFF;
    }

    ignitionLed = engineOn;
}

bool isEngineOn() {
    return engineOn;
}