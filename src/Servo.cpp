#include "Servo.h"
#include <algorithm>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>
#include <math.h>

Servo::Servo(const int PIN)
    :   PIN(PIN) {
    gpioSetMode(PIN, PI_OUTPUT); // set pin mode
}

int Servo::getPin() const {
    return PIN;
}

int Servo::getPosition() {
    return servo_position;
}

void Servo::step(int degrees) {
    // clamp step in position to a local change in degrees
    degrees = std::max(-10, std::min(10, degrees));
    // drive to local position; assume time is instantaneous. 
    position(degrees);
    // assume servo is at new position; update position.
    servo_position += degrees;
}

void Servo::position(int degrees) {
    // clamp degrees to mapped range of [0, 180] from [-90, +90]
    degrees = std::max(std::min(degrees + 90, 180), 0);  
    // compute servo position value from PWM pulse range 
    double delta = PWM_RANGE[1] - PWM_RANGE[0];
    int value = (double)PWM_RANGE[0] + ((double)degrees / 180.0) * delta;
    gpioServo(PIN, value);
}

int Servo::timeStep(int initial_position, int final_position, int position, double acceleration, int max_time_step) {
    // clamp positions
    initial_position = std::max(-90, std::min(90, initial_position));
    final_position = std::max(-90, std::min(90, final_position));
    // compute distances
    int distance = std::abs(final_position - initial_position);
    position = std::abs(position);
    int time_step_millis = 10;

    // compute time-step by if speeding up or down (based on if position is in 1st or 2nd half of the traveling)
    if(position < distance) {
        int acclerating_time_step = 1000 * (1/2) * std::pow(2.0 / (acceleration * distance), 0.5);
        time_step_millis = acclerating_time_step;
    }
    else {
        int decelerating_time_step = 1000 * (1/2) * std::pow(2.0 / (acceleration * (distance - distance)), 0.5);
        time_step_millis = decelerating_time_step;
    }

    return time_step_millis;
}