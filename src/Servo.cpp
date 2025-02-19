#include "Servo.h"
#include <algorithm>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>
#include <math.h>
#include "handler.h"

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
    position(servo_position + degrees);
    // assume servo is at new position; update position.
    servo_position += degrees;
}

void Servo::drive(int degrees, double acceleration) {
    int initial_position = getPosition(); 
    int final_position = degrees; 
    int delta_position = std::abs(final_position - initial_position);
    int sign = (final_position - initial_position >= 0) ? 1 : -1;

    while(std::abs(getPosition() - initial_position) <= delta_position) {
        double time_step_millis = timeStep(initial_position, final_position, getPosition(), acceleration, 75);
        std::cout << getPosition() << std::endl;
        //printf("%f\n", getPosition());//time_step_millis);
        step(sign);
        time_sleep(time_step_millis / 1000.0);
    }
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
    position = std::abs(position - initial_position);
    int time_step_millis = 10;

    // compute time-step by if speeding up or down (based on if position is in 1st or 2nd half of the traveling)
    if(position < distance / 2) {
        int acclerating_time_step = 1000.0 * (1.0/2.0) * std::pow(2.0 / (acceleration * nonZero(position, 0.1)), 0.5);
        time_step_millis = std::min(max_time_step, acclerating_time_step);
    }
    else {
        int decelerating_time_step = 1000.0 * (1.0/2.0) * std::pow(2.0 / (acceleration * (distance - nonZero(position, 0.01))), 0.5);
        time_step_millis = std::min(max_time_step, decelerating_time_step);
    }

    return time_step_millis;
}