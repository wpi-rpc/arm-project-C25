#include "Robot.h"
#include <cstdio>
#include <pigpio.h>

void Robot::setup() {
    // robot initilization code
<<<<<<< Updated upstream
    shoulder.home();
    elbow.home();
=======
    base.home();
    shoulder.home();
    elbow.home();
    wrist.home();
    end_effector.home();
>>>>>>> Stashed changes
    // delay after homing; motor positioning unreliable until afterwards. 
    time_sleep(1);
}

int Robot::loop() {
    // main program loop
 
<<<<<<< Updated upstream
    //printf("Driving...\n");
    shoulder.drive(45, 100);
    elbow.drive(-45, 100);

    shoulder.drive(-45, 100);
    elbow.drive(45, 100);
=======
   /**/ //printf("Driving...\n");
    //base.drive(45, 200);
    elbow.drive(90, 200);
    elbow.drive(-45, 200);
    //shoulder.position(-90);
    //sleep_ms(1000);
    //shoulder.position(90);
    //sleep_ms(5000);
    //shoulder.position(-45);
    //sleep_ms(1000);
    //shoulder.position(45);
    //sleep_ms(1000);*/
    //base.drive(-45, 200);
    //wrist.drive(90, 50);
>>>>>>> Stashed changes

    return 0;
}
