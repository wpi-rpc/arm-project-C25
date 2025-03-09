#include "Robot.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/time.h"
#include <cstdio>

void Robot::digitalWrite(int pin, bool state) {
    cyw43_arch_gpio_put(pin, state);
}

bool Robot::digitalRead(int pin) {
    return cyw43_arch_gpio_get(pin);
}

uint32_t Robot::getMillis() {
    return to_ms_since_boot(get_absolute_time());
}

void Robot::setup() {
    // robot initilization code
<<<<<<< HEAD
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
=======
    base.home();
>>>>>>> 9928a749435f0d4475f42a6aebcc8a4f2c75b7a8
    // delay after homing; motor positioning unreliable until afterwards. 
    sleep_ms(5000);
}

int Robot::loop() {
    // main program loop
 
<<<<<<< HEAD
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
=======
   /**/ //printf("Driving...\n");
    base.drive(45, 100);
>>>>>>> 9928a749435f0d4475f42a6aebcc8a4f2c75b7a8
    //shoulder.position(-90);
    //sleep_ms(1000);
    //shoulder.position(90);
    //sleep_ms(5000);
    //shoulder.position(-45);
    //sleep_ms(1000);
    //shoulder.position(45);
    //sleep_ms(1000);*/
<<<<<<< HEAD
    //base.drive(-45, 200);
    //wrist.drive(90, 50);
>>>>>>> Stashed changes
=======
    base.drive(-45, 100);
>>>>>>> 9928a749435f0d4475f42a6aebcc8a4f2c75b7a8

    //shoulder.drive(-45, 100);
    //elbow.drive(45, 100);
    
    return 0;
}
