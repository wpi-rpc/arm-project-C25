#include "Servo.h"
#include <algorithm>
#include <unistd.h>
#include <stdio.h>
#include <pigpio.h>

Servo::Servo(const int PIN)
    :   PIN(PIN) {
    gpioSetMode(PIN, PI_OUTPUT); // set pin mode
}

int Servo::getPin() const {
    return PIN;
}

void Servo::position(int degrees) {
    // clamp degrees to mapped range of [0, 180] from [-90, +90]
    degrees = std::max(std::min(degrees + 90, 180), 0);  
    // compute servo position value from PWM pulse range 
    double delta = PWM_RANGE[1] - PWM_RANGE[0];
    int value = (double)PWM_RANGE[0] + ((double)degrees / 180.0) * delta;
    gpioServo(PIN, value);
}