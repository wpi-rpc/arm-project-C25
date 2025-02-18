#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>
#include <unistd.h>
#include <algorithm>
#include "Servo.h"

int main() {
    // initialize pigpio library setup; exit if failure
    //if(gpioInitialise() == -1) {
    //    exit(EXIT_FAILURE);
    //}

    // construct motors
    Servo motor_A = Servo(6); 
    Servo motor_B = Servo(13);

    pid_t pid = fork();
    if(pid < 0) {
        printf("Fork failed!\n");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0) {
        gpioInitialise();
        gpioSetMode(motor_B.getPin(), PI_OUTPUT);
        motor_B.drive(0);
        time_sleep(0.500);
        gpioTerminate();
        exit(0);
    }

    // drive with pauses
   // motor_A.drive(-90);
    //motor_B.drive(-55);
    //time_sleep(0);
    //motor_A.drive(90);
    //motor_B.drive(45);
    //time_sleep(0);
    //motor_A.drive(0);
    //motor_B.drive(0);
    //time_sleep(0);

    printf("Done");
    // end use of pigpio library
    //gpioTerminate();
    return 0;
}