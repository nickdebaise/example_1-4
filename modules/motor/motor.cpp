//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"
#include "ignition.h"
#include "motor.h"
#include "main.h"


//=====[Declaration of private defines]========================================

#define STOP_DC 1.5 / 20
#define HIGH_D1 1.65 / 20
#define HIGH_D2 1.45 / 20
#define DELAY_1_TIME 20 // Replace with amount of time it takes to go from right side to left side
#define DELAY_2_TIME 20 // Replace with amount of time it takes to go from left side to right side

#define PERIOD 0.02

//=====[Declaration of private data types]=====================================

typedef enum {
    IDLE, DIRECTION1, DIRECTION2
} HighSpeedState_t;

//=====[Declaration and initialization of public global objects]===============

// Inputs

// Outputs

PwmOut wiperMotor(PF_9);

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

HighSpeedState_t highSpeedMotorState = IDLE;
int highSpeedTotalTime = 0;

//=====[Declarations (prototypes) of private functions]========================


//=====[Implementations of public functions]===================================

void motorInit();

void performHighSpeedCycle();
void performLowSpeedCycle();

//=====[Implementations of private functions]==================================

void motorInit() {
    wiperMotor.period(PERIOD);
}

void performHighSpeedCycle() {
    // in a non-blocking way, go from one side to the other and back again no matter what
    switch(highSpeedMotorState) {
        case IDLE:
            wiperMotor.write(STOP_DC);
            highSpeedTotalTime = 0;
            highSpeedMotorState = DIRECTION1;
        case DIRECTION1:
            if(highSpeedTotalTime > DELAY_1_TIME) {
                highSpeedMotorState = DIRECTION2;
            }
            highSpeedTotalTime += SYSTEM_DELAY_TIME;
        case DIRECTION2:
            if(highSpeedTotalTime > DELAY_2_TIME) {
                highSpeedMotorState = IDLE;
            }
            highSpeedTotalTime += SYSTEM_DELAY_TIME;
        default:
            break;
    }
}

void performLowSpeedCycle() {
    // in a non-blocking way, go from one side to the other and back again no matter what

}