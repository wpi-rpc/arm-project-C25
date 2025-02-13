#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <softPwm.h>

#define SERVO_PIN   5

int main() {
    // initialize setupt; exit if failure
    if(wiringPiSetup() == -1) {
        exit(EXIT_FAILURE);
    }
    // create servo on pin; value range [0, 100]
    softPwmCreate(SERVO_PIN, 0, 100);
    double angle = 100.0; // angle in range [0, 180]
    int value = (int)((angle / 180.0) * 100.0); // map angle -> value
    
    // set value of angle
    softPwmWrite(SERVO_PIN, value);
    // end program after setting position
    std::cout<<"done!\n";
    delay(1000);
    return 0;
}