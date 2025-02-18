#include "Robot.h"
#include <cstdio>
#include <pigpio.h>

void Robot::setup() {
    // robot initilization code
    shoulder.position(-90);
    time_sleep(1);
}

int Robot::loop() {
    // main program loop
    static int pos = -90; 
    static double ms = 0.05;
    static double accel = 0.00075;
    shoulder.position(pos++);
    time_sleep(ms);
    if(pos < 0)
        ms -= accel;
    else 
        ms += accel;
    return (pos < 90) ? 0 : 1;
}