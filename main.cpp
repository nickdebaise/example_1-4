//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"
#include "ignition.h"
#include "wipers.h"
#include "main.h"

//=====[Declaration of private defines]========================================

#define DUTY_MIN 0.029
#define DUTY_MAX 0.118
#define PERIOD 0.02

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

// Inputs

AnalogIn modeSelector(A2);
AnalogIn intervalSelector(A3);

// Outputs

PwmOut wipers(PF_9);

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

//=====[Declarations (prototypes) of private functions]========================

void initialize();

//=====[Implementations of public functions]===================================

//=====[Implementations of private functions]==================================

int main()
{   
    initialize();

    while (true) {
        checkIgnitionSubsystem();
        checkWiperSubsystem();
        delay(SYSTEM_DELAY_TIME);
    }
}


void initialize() {
    ignitionInit();
    wipersInit();
}

