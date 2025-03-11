#include "Robot.h"
#include "CommonGatewayInterface.h"
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
    base.home();
    shoulder.home();
    elbow.home();
    // delay after homing; motor positioning unreliable until afterwards. 
    sleep_ms(5000);
}

int Robot::loop() {
    // main program loop
    char* data[4];
    CommonGatewayInterface::read(data);
    printf("Left: (%f, %f), Right: (%f, %f)\n", atof(data[0]), atof(data[1]), atof(data[2]), atof(data[3]));
    sleep_ms(250);
    return 0;
}
