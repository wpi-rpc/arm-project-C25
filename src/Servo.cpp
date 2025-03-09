#include "Servo.h"
#include <math.h>
#include <cstdio>
#include <memory>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "pico/cyw43_arch.h"
#include "handler.h"
#include "Robot.h"
#include "AltThread.h"

void Servo::driverThread() {
    if(this->spin_driver_thread) {
        // critical region: only allow this thread access to drive_command contents/address
        mutex_enter_blocking(&(this->driver_lock));
        // check if command exists and run it if so
        if(this->drive_command && !this->init_drive_step) {
            // supply driving command parameters (position, and acceleration for motion profiling)
            int degrees = std::get<0>(*drive_command);
            double acceleration = std::get<1>(*drive_command);
            initDriveStep(degrees, acceleration);
        }
<<<<<<< HEAD
<<<<<<< Updated upstream
    }));
=======
=======
>>>>>>> 9928a749435f0d4475f42a6aebcc8a4f2c75b7a8
        else if(drive_command && init_drive_step) { 
            this->spin_driver_thread = driveStep();
        }

        // end of critical region: done handling drive_command
        mutex_exit(&(this->driver_lock));
    }
}

<<<<<<< HEAD
Servo::Servo(const int PIN, const int HOME_POSITION)
=======
Servo::Servo(Robot::ServoID PIN, const int HOME_POSITION)
>>>>>>> 9928a749435f0d4475f42a6aebcc8a4f2c75b7a8
    :   PIN((int)PIN), PIN_SLICE(pwm_gpio_to_slice_num((int)PIN)), PIN_CHANNEL(pwm_gpio_to_channel((int)PIN)), HOME_POSITION(HOME_POSITION) { 
    // set pin mode:
    gpio_set_function((int)PIN, GPIO_FUNC_PWM); 
    mutex_init(&(this->driver_lock));
    // set pin pwm frequency:
    const int MAX_INTEGER = 65535; // max number on 16-bit pico system
    int pico_clock_step = 2500000; // <=> Robot::DEFAULT_CLOCK_SPEED / Servo::CLOCK_FREQUENCY => step size to count from main pico clock counter
    // Notice: pico_clock_step is too large for the 16-bit pwm wrap value; rescale and map the step by the max 16-bit integer to be within allowable range
    // set pwm conditions; 
    pwm_set_wrap(PIN_SLICE, MAX_INTEGER); // set wrap to needed time for servo frequency; 
    // if sys clock is rescaled below to use full 16-bit range of the wrap, then let the wrap be the max 16-bit integer
    pwm_set_clkdiv(PIN_SLICE, (double)pico_clock_step / (double)MAX_INTEGER); // rescale sys clock by 2 magnitudes 
    pwm_set_phase_correct(PIN_SLICE, false);
    pwm_set_enabled(PIN_SLICE, true);

    // register servo driving thread with the Servo multicore
    AltThread::registerThread([this](){driverThread();});
<<<<<<< HEAD
>>>>>>> Stashed changes
=======
>>>>>>> 9928a749435f0d4475f42a6aebcc8a4f2c75b7a8
}

Servo::~Servo() {
    // check for existing driving thread; wait for its completion and reset ptr    
    this->spin_driver_thread = false; // flag down to terminate this servo "thread"
    // reset any remaining command
    if(this->drive_command) {
        this->drive_command.reset();
    }
}

void Servo::home() {
    position(HOME_POSITION);
    this->servo_position = HOME_POSITION;
}

int Servo::getPin() const {
    return PIN;
}

int Servo::getPosition() {
    return this->servo_position;
}

void Servo::step(int degrees) {
    // clamp step in position to a local change in degrees
    degrees = std::max(-10, std::min(10, degrees));
    // drive to local position; assume time is instantaneous. 
    position(this->servo_position + degrees);
    // assume servo is at new position; update position.
    this->servo_position += degrees;
}

void Servo::initDriveStep(int target_degrees, double acceleration) {
    if(!this->init_drive_step) {
        // define start and end angles with acceleration
        this->initial_drive_position = servo_position; 
        this->final_drive_position = target_degrees; 
        this->drive_acceleration = acceleration;
        this->init_drive_step = true;
    }
}

bool Servo::driveStep() {
    // consider driving as a problem of traveling over a distance (magnitude) ignoring direction
    int delta_position = std::abs(this->final_drive_position - this->initial_drive_position); // distance to travel
    // take note of the direction
    int sign = (this->final_drive_position - this->initial_drive_position >= 0) ? 1 : -1;

    // drive by stepping position with dynamic delays until distance has been traveled
    if(this->init_drive_step && std::abs(this->servo_position - this->initial_drive_position) <= delta_position) {
        // find dynamic time based on "how far along" current position `getPosition()` is from initial going to final position
        double time_step_millis = timeStep(this->initial_drive_position, this->final_drive_position, this->servo_position, this->drive_acceleration, 13);
        // move motor by step and wait
        step(sign);
        sleep_ms(time_step_millis);
        return true;
    }

    // done driving
    this->init_drive_step = false;
    // command done; empty drive command
    this->drive_command.reset();
    return false;
}

void Servo::drive(int degrees, double acceleration) {
    // critical region: only allow the main thread access to drive_command contents/address
    mutex_enter_blocking(&(this->driver_lock)); // updating the command; only happens in critical region
    // check for and ignore noise
    if(std::abs(this->servo_position - degrees) <= 1) {
        mutex_exit(&(this->driver_lock));
        return;
    }
    // check for and ignore duplicate sequential commands
    else if(this->drive_command && std::get<0>(*(this->drive_command)) == degrees && std::get<1>(*(this->drive_command)) == acceleration) {
        mutex_exit(&(this->driver_lock));
        return;
    }
    // check for current command; delete it if present
    else if(this->drive_command) {
        this->drive_command.reset();
    }
    
    // create new command with given parameters
    this->drive_command = std::make_unique<std::tuple<int, double>>();
    *(this->drive_command) = std::make_tuple(degrees, acceleration);
    this->spin_driver_thread = true; // begin to spin thread for this servo

    // end of critical region: done handling drive_command
    mutex_exit(&(this->driver_lock));
}    

void Servo::position(int degrees) {
    // clamp degrees to mapped range of [0, 180] from [-90, +90]
    degrees = std::max(std::min(degrees + 90, 180), 0);  
    // compute servo position value from PWM pulse range 
    double delta = PWM_RANGE[1] - PWM_RANGE[0];
    int level = (double)PWM_RANGE[0] + ((double)degrees / 180.0) * delta;
    pwm_set_chan_level(PIN_SLICE, PIN_CHANNEL, level);
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