//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"
#include "wipers.h"
#include "ignition.h"
#include "display.h"
#include "main.h"
#include <string>

//=====[Declaration of private defines]========================================

#define PERIOD 0.02
#define DUTY_MIN 0.0325
#define DUTY_MAX 0.0675

#define HD1_TIME 140
#define HD2_TIME 140

#define LD1_TIME 280
#define LD2_TIME 280

#define STEP_SIZE 50
#define LO_DELAY LD1_TIME / STEP_SIZE

//=====[Declaration of private data types]=====================================

typedef enum {
    HI, LO, INT, W_OFF
} wiperSelector_t;

typedef enum {
   SHORT, MEDIUM, LONG
} intervalSelector_t;

typedef enum {
    IDLE, HI_DIR1, HI_DIR2, LO_DIR1, LO_DIR2, INT_DIR1, INT_DIR2, INT_WAIT
} motorState_t;

//=====[Declaration and initialization of public global objects]===============

// Inputs

AnalogIn wiperModeSelector(A0);
AnalogIn intervalModeSelector(A1);

// Outputs
PwmOut wiperMotor(PF_9);

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

wiperSelector_t selectedMode = W_OFF;
intervalSelector_t selectedIntMode = SHORT;
motorState_t motorState = IDLE;

int accumulatedTime = 0;
float accumulatedLoStepTime = 0;

float currLoStep = DUTY_MIN;

const float stepSize = (DUTY_MAX - DUTY_MIN) / STEP_SIZE;

//=====[Declarations (prototypes) of private functions]========================

void readWiperSelectorPot();
void readIntervalSelectorPot();
void displaySelectedMode();
string modeToString();
string intervalToString();
int mapIntervalToMs(intervalSelector_t intervalMode);

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
    wiperMotor.period(PERIOD);
}

/***
    Run logic for checking wiper subsystem
***/
void checkWiperSubsystem() {

    readWiperSelectorPot();
    readIntervalSelectorPot();
    if(isEngineOn()) {
        displaySelectedMode();
    }


    switch(motorState) {
        case IDLE:
            accumulatedTime = 0;
            accumulatedLoStepTime = 0;
            currLoStep = DUTY_MIN;

            if(!isEngineOn() || selectedMode == W_OFF) {
                motorState = IDLE;
            } else {
                if(selectedMode == HI) {
                    motorState = HI_DIR1;
                } else if(selectedMode == LO) {
                    motorState = LO_DIR1;
                } else if(selectedMode == INT) {
                    motorState = INT_DIR1;
                }
            }
            wiperMotor.write(DUTY_MIN);
            break;
        case HI_DIR1:
            if(accumulatedTime >= HD1_TIME) {
                motorState = HI_DIR2;
            }

            accumulatedTime += SYSTEM_DELAY_TIME;

            wiperMotor.write(DUTY_MIN);
            break;
        case HI_DIR2:
            if(accumulatedTime >= HD2_TIME + HD1_TIME) {
                if(!isEngineOn()) {
                    motorState = IDLE;
                } else if(selectedMode == HI) {
                    accumulatedTime = 0;
                    motorState = HI_DIR1;
                } else {
                    motorState = IDLE;
                }
            }

            accumulatedTime += SYSTEM_DELAY_TIME;

            wiperMotor.write(DUTY_MAX);
            break;
        case LO_DIR1:
            if(accumulatedTime >= LD1_TIME) {
                motorState = LO_DIR2;
                currLoStep = DUTY_MAX;
            } else if(accumulatedLoStepTime >= (int)LO_DELAY) {
                accumulatedLoStepTime = 0;
                if(currLoStep < DUTY_MAX) {
                    currLoStep += stepSize;
                }
            }

            accumulatedTime += SYSTEM_DELAY_TIME;
            accumulatedLoStepTime += SYSTEM_DELAY_TIME;

            wiperMotor.write(currLoStep);
            break;
        case LO_DIR2:
            if(accumulatedTime >= LD2_TIME + LD1_TIME) {
                motorState = IDLE;
                accumulatedTime = 0;            
                currLoStep = DUTY_MIN;
            } else if(accumulatedLoStepTime >= (int)LO_DELAY) {
                accumulatedLoStepTime = 0;
                if(currLoStep > DUTY_MIN) {
                    currLoStep -= stepSize;
                }
            }

            accumulatedTime += SYSTEM_DELAY_TIME;
            accumulatedLoStepTime += SYSTEM_DELAY_TIME;

            wiperMotor.write(currLoStep);
            break; 
        case INT_DIR1:

            if(accumulatedTime >= LD1_TIME) {
                motorState = INT_DIR2;
                currLoStep = DUTY_MAX;
            } else if(accumulatedLoStepTime >= (int)LO_DELAY) {
                accumulatedLoStepTime = 0;
                if(currLoStep < DUTY_MAX) {
                    currLoStep += stepSize;
                }
            }

            accumulatedTime += SYSTEM_DELAY_TIME;
            accumulatedLoStepTime += SYSTEM_DELAY_TIME;

            wiperMotor.write(currLoStep);
            break;
        case INT_DIR2:

            if(accumulatedTime >= LD2_TIME + LD1_TIME) {
                motorState = INT_WAIT;
                accumulatedTime = 0;            
                currLoStep = DUTY_MIN;
            } else if(accumulatedLoStepTime >= (int)LO_DELAY) {
                accumulatedLoStepTime = 0;
                if(currLoStep > DUTY_MIN) {
                    currLoStep -= stepSize;
                }
            }

            accumulatedTime += SYSTEM_DELAY_TIME;
            accumulatedLoStepTime += SYSTEM_DELAY_TIME;

            wiperMotor.write(currLoStep);
            break;
        case INT_WAIT:
            if(!isEngineOn() || selectedMode != INT) {
                motorState = IDLE;
            }

            if(accumulatedTime >= mapIntervalToMs(selectedIntMode)) {
                accumulatedTime = 0;
                motorState = INT_DIR1;
            }

            accumulatedTime += SYSTEM_DELAY_TIME;
    }

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
            return "Low ";
        case INT:
            return "Int.";
        default:
            return "Off ";
    }
}

string intervalToString() {
    switch(selectedIntMode) {
        case SHORT:
            return "Short ";
        case MEDIUM:
            return "Medium";
        default:
            return "Long  ";
    }
}

int mapIntervalToMs(intervalSelector_t intervalMode) {
    switch(intervalMode) {
        case SHORT:
            return 3 * 100;
        case MEDIUM:
            return 6 * 100;
        default: 
            return 8 * 100;
    }
}


