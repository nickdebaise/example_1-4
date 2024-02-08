#include "mbed.h"
#include "arm_book_lib.h"

#define DUTY_MIN 0.029
#define DUTY_MAX 0.118
#define PERIOD 0.02

PwmOut servo(PF_9);

void inputsInit();


int main()
{   
    inputsInit();
    while (true) {
        servo.write(DUTY_MIN);
        delay(3000);
        servo.write(DUTY_MAX);
        delay(3000);
    }
}

void inputsInit() {
    servo.period(PERIOD);
}