//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"
#include "ignition.h"


//=====[Declaration of private defines]========================================

#define PERIOD 20

//=====[Declaration of private data types]=====================================


typedef enum {
    TRANSITIONING,
    HI,
    LO,
    INT,
    W_OFF
} motorState_t;

//=====[Declaration and initialization of public global objects]===============

// Ibnnputs

// Outputs

PwmOut wiperMotor(PF_9);

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

//=====[Declarations (prototypes) of private functions]========================


//=====[Implementations of public functions]===================================

void motorInit();
void checkMotorSubsystem();

//=====[Implementations of private functions]==================================

void motorInit() {

}

/***
    Run logic for checking motor subsystem
***/
void checkMotorSubsystem() {
    if(isEngineOn()) {

    }
}
