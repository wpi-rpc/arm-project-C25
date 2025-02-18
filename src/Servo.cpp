#include "Servo.h"
#include <algorithm>
#include <unistd.h>
#include <pigpio.h>

Servo::Servo(const int PIN)
    :   PIN(PIN) {
}

int Servo::getPin() const {
    return PIN;
}

void Servo::drive(int degrees) {
    degrees = std::max(std::min(degrees + 90, 180), 0);  
    double delta = PWM_RANGE[1] - PWM_RANGE[0];
    int value = (double)PWM_RANGE[0] + ((double)degrees / 180.0) * delta;
    gpioServo(PIN, value);
}