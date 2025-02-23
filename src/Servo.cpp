#include "Servo.h"
#include <math.h>
#include <cstdio>
#include <memory>
#include <pigpio.h>
#include "handler.h"

Servo::Servo(const int PIN, const motor_type TYPE, const int HOME_POSITION)
    :   PIN(PIN), TYPE(TYPE), HOME_POSITION(HOME_POSITION) {
    gpioSetMode(PIN, PI_OUTPUT); // set pin mode
    // initialize the motor driver
    driver_thread = std::make_unique<std::thread>(std::thread([this](){
        // spin thread; wait for drive commands, run, and then dispose of. 
        while(spin_driver_thread) {
            time_sleep(0.010);
            // critical region: only allow this thread access to drive_command contents/address
            driver_lock.lock();
            // check if command exists and run it if so
            if(drive_command) {
                // supply driving command parameters (position, and acceleration for motion profiling)
                int degrees = std::get<0>(*drive_command);
                double acceleration = std::get<1>(*drive_command);

                // begin servo threaded-driving
                driveBlocking(degrees, acceleration); 
                // command done; empty drive command
                drive_command.reset();
            }

            // end of critical region: done handling drive_command
            driver_lock.unlock();
        }
    }));
}

Servo::~Servo() {
    // check for existing driving thread; wait for its completion and reset ptr
    if(driver_thread && (*driver_thread).joinable()) {
        spin_driver_thread = false; // flag down to terminate thread
        (*driver_thread).join(); // wait for thread
        driver_thread.reset(); // reset
    }
    // reset any remaining command
    if(drive_command) {
        drive_command.reset();
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

std::tuple<int, int> Servo::getRange(motor_type type) {
    switch(type) {
        //motor in range 0,180 degrees
        case(motortypes::MG995) {
            return std::make_tuple(0, 180);
        }
        //motor in range 0,270 degrees
        case(motortypes::MS62) {
            return std::make_tuple(0,270);
        }
    }
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
    // critical region: only allow the main thread access to drive_command contents/address
    driver_lock.lock(); // updating the command; only happens in critical region
    // check for and ignore noise
    if(std::abs(servo_position - degrees) <= 1) {
        driver_lock.unlock();
        return;
    }
    // check for and ignore duplicate sequential commands
    else if(drive_command && std::get<0>(*drive_command) == degrees && std::get<1>(*drive_command) == acceleration) {
        driver_lock.unlock();
        return;
    }
    // check for current command; delete it if present
    else if(drive_command) {
        drive_command.reset();
    }
    
    // create new command with given parameters
    drive_command = std::make_unique<std::tuple<int, double>>();
    *drive_command = std::make_tuple(degrees, acceleration);
    
    // end of critical region: done handling drive_command
    driver_lock.unlock();
}    

void Servo::position(int degrees) {
    // clamp degrees to mapped range of [0, 180] from [-90, +90],
    // clamp degrees to mapped range of [0, 270] from [-135, +135],
    // based off of the motor type
    std::tuple<int, int> range = getRange(TYPE);
    degrees = std::max(std::min(degrees + (std::get<1>(range) / 2), std::get<1>(range)), std::get<0>(range));  
    // compute servo position value from PWM pulse range 
    double delta = PWM_RANGE[1] - PWM_RANGE[0];
    int value = (double)PWM_RANGE[0] + ((double)degrees / (double)std::get<1>(range)) * delta;
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