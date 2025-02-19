#include "Robot.h"
#include <cstdio>
#include <pigpio.h>

void Robot::setup() {
    // robot initilization code
    shoulder.position(0);
    time_sleep(1);
}

int Robot::loop() {
    // main program loop
 
    printf("Driving...\n");
    shoulder.drive(-90, 115);
    shoulder.drive(90, 115);
    shoulder.drive(0, 115);
    return 1;
}