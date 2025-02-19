#include "Robot.h"
#include <cstdio>
#include <pigpio.h>

void Robot::setup() {
    // robot initilization code
    shoulder.home();
    elbow.home();
    // delay after homing; motor positioning unreliable until afterwards. 
    time_sleep(1);
}

int Robot::loop() {
    // main program loop
 
    printf("Driving...\n");
    shoulder.drive(-45, 100);
    elbow.drive(45, 100);
    return 0;
}
