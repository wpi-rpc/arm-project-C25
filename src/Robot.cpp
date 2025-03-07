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
    shoulder.home();
    // delay after homing; motor positioning unreliable until afterwards. 
    sleep_ms(1000);
}

int Robot::loop() {
    // main program loop
 
   /**/ //printf("Driving...\n");
    shoulder.drive(45, 100);
    //shoulder.position(-90);
    //sleep_ms(1000);
    //shoulder.position(90);
    //sleep_ms(5000);
    //shoulder.position(-45);
    //sleep_ms(1000);
    //shoulder.position(45);
    //sleep_ms(1000);*/
    shoulder.drive(-45, 100);

    //shoulder.drive(-45, 100);
    //elbow.drive(45, 100);
    
    return 0;
}
