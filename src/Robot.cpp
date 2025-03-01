#include "Robot.h"
#include "pico/stdlib.h"
#include <cstdio>

void Robot::setup() {
    // robot initilization code
    shoulder.home();
    elbow.home();
    // delay after homing; motor positioning unreliable until afterwards. 
    sleep_ms(1000);
}

int Robot::loop() {
    // main program loop
 
    //printf("Driving...\n");
    shoulder.drive(45, 100);
    elbow.drive(-45, 100);

    shoulder.drive(-45, 100);
    elbow.drive(45, 100);

    return 0;
}
