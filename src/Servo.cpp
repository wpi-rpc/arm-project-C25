#include "Servo.h"
#include <math.h>
#include <cstdio>
#include <memory>
#include <pigpio.h>
#include "handler.h"

Servo::Servo(const int PIN, const int HOME_POSITION)
    :   PIN(PIN), HOME_POSITION(HOME_POSITION) {
    gpioSetMode(PIN, PI_OUTPUT); // set pin mode
}

Servo::~Servo() {
    if(driving_thread && (*driving_thread).joinable()) {
        (*driving_thread).join();
        driving_thread.reset();
    }
}

void Servo::home() {
    position(HOME_POSITION);
    servo_position = HOME_POSITION;
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

void Servo::driveBlocking(int degrees, double acceleration) {
    // define start and end angles 
    int initial_position = servo_position; 
    int final_position = degrees; 
    // consider driving as a problem of traveling over a distance (magnitude) ignoring direction
    int delta_position = std::abs(final_position - initial_position); // distance to travel
    // take note of the direction
    int sign = (final_position - initial_position >= 0) ? 1 : -1;

    // drive by stepping position with dynamic delays until distance has been traveled
    while(std::abs(servo_position - initial_position) <= delta_position) {
        // find dynamic time based on "how far along" current position `getPosition()` is from initial going to final position
        double time_step_millis = timeStep(initial_position, final_position, servo_position, acceleration, 13);
        // move motor by step and wait
        step(sign);
        time_sleep(time_step_millis / 1000.0);
    }
}

void Servo::drive(int degrees, double acceleration) {
    if(std::abs(degrees - servo_position) <= 1) {
        return;
    }
    else if(driving_thread && (*driving_thread).joinable()) {
        (*driving_thread).join();
        driving_thread.reset();
    }

    driving_thread = std::make_unique<std::thread>(std::thread( [this, degrees, acceleration](){ 
        driveBlocking(degrees, acceleration); 
    } ));
}    

void Servo::position(int degrees) {
    // clamp degrees to mapped range of [0, 180] from [-90, +90]
    degrees = std::max(std::min(degrees + 90, 180), 0);  
    // compute servo position value from PWM pulse range 
    double delta = PWM_RANGE[1] - PWM_RANGE[0];
    int value = (double)PWM_RANGE[0] + ((double)degrees / 180.0) * delta;
    gpioServo(PIN, value);
}

int Servo::timeStep(int initial_position, int final_position, int position, double acceleration, double max_speed) {
    // clamp positions
    initial_position = std::max(-90, std::min(90, initial_position));
    final_position = std::max(-90, std::min(90, final_position));
    // compute distances
    position = std::abs(position - initial_position); // position "already traveled to" from `position` from starting point ignoring direction
    int distance = std::abs(final_position - initial_position); // overall distance to travel
    int max_time_step_millis = 1000 / max_speed; // computed upper limit max time step to delay by given max speed
    int time_step_millis = 10;  

    // compute time-step by if speeding up or down (based on if position is in 1st or 2nd half of the traveling)
    if(position < distance / 2) {
        int acclerating_time_step = 1000.0 * (1.0/2.0) * std::pow(2.0 / (acceleration * nonZero(position, 0.1)), 0.5);
        time_step_millis = std::min(max_time_step_millis, acclerating_time_step);
    }
    else {
        int decelerating_time_step = 1000.0 * (1.0/2.0) * std::pow(2.0 / (acceleration * (distance - nonZero(position, 0.01))), 0.5);
        time_step_millis = std::min(max_time_step_millis, decelerating_time_step);
    }

    return time_step_millis;
}