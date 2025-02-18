#include "Robot.h"
#include <cstdio>
#include <pigpio.h>

void Robot::setup() {
    // robot initilization code
    //shoulder.drive(-90, 5);
    time_sleep(1);

    int init_p = 0;
    int final_p = 90; 
    double a = 2.0;

    int times[200]; 
    int count = 0;
    for(int i = init_p; i < final_p; i++) {
        double time = shoulder.timeStep(init_p, final_p, i, a, 50);
        times[count++] = time;
        printf("deg, time = %d, %d\n", i, time);
    }
}

int Robot::loop() {
    // main program loop
    static int pos = -90; 
    static double ms = 0.05;
    static double accel = 5;
    
    //shoulder.drive(90, accel);

    /*shoulder.position(pos++);
    time_sleep(ms);
    if(pos < 0)
        ms -= accel;
    else 
        ms += accel;
    */
    return (pos < 90) ? 0 : 1;
}