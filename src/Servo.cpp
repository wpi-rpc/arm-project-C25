#include "Servo.h"
#include <math.h>
#include <cstdio>
#include <memory>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "handler.h"
#include "Robot.h"

// define static member list
const std::shared_ptr<std::vector<std::function<void()>>> Servo::SERVO_THREADS = std::make_shared<std::vector<std::function<void()>>>();
// define multicore pico thread
void Servo::SERVO_MULTICORE() {
    while(true) { // spin indefinitely
        if(SERVO_THREADS && !(*SERVO_THREADS).empty()) {
            // step through all threads of registered / constructed servos
            for(const auto& thread : *SERVO_THREADS) {
                // run servo driving thread
                thread();
            }
        }
    }
}

Servo::Servo(const int PIN, const int HOME_POSITION)
    :   PIN(PIN), HOME_POSITION(HOME_POSITION), PIN_SLICE(pwm_gpio_to_slice_num(PIN)), PIN_CHANNEL(pwm_gpio_to_channel(PIN)) {
    // set pin mode:
    gpio_set_function(PIN, GPIO_FUNC_PWM); 
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
    
    // initialize the motor driver
    driver_thread = [this](){
        // run thread; wait for drive commands, run, and then dispose of. 
        if(spin_driver_thread) {
            // critical region: only allow this thread access to drive_command contents/address
            mutex_enter_blocking(&driver_lock);
            // check if command exists and run it if so
            if(drive_command) {
                // supply driving command parameters (position, and acceleration for motion profiling)
                int degrees = std::get<0>(*drive_command);
                double acceleration = std::get<1>(*drive_command);

                // begin servo threaded-driving
                if(!init_drive_step) 
                    initDriveStep(degrees);
                if(init_drive_step)
                    spin_driver_thread = driveStep(degrees, acceleration); 
            }

            // end of critical region: done handling drive_command
            mutex_exit(&driver_lock);
        }
    };

    // register servo driving thread with the Servo multicore
    (*SERVO_THREADS).push_back(driver_thread);
}

Servo::~Servo() {
    // check for existing driving thread; wait for its completion and reset ptr    
    spin_driver_thread = false; // flag down to terminate this servo "thread"
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

void Servo::step(int degrees) {
    // clamp step in position to a local change in degrees
    degrees = std::max(-10, std::min(10, degrees));
    // drive to local position; assume time is instantaneous. 
    position(servo_position + degrees);
    // assume servo is at new position; update position.
    servo_position += degrees;
}

void Servo::initDriveStep(int target_degrees) {
    if(!init_drive_step) {
        // define start and end angles 
        initial_drive_position = servo_position; 
        final_drive_position = target_degrees; 
        init_drive_step = true;
    }
}

bool Servo::driveStep(int degrees, double acceleration) {
    // consider driving as a problem of traveling over a distance (magnitude) ignoring direction
    int delta_position = std::abs(final_drive_position - initial_drive_position); // distance to travel
    // take note of the direction
    int sign = (final_drive_position - initial_drive_position >= 0) ? 1 : -1;

    // drive by stepping position with dynamic delays until distance has been traveled
    if(init_drive_step && std::abs(servo_position - initial_drive_position) <= delta_position) {
        // find dynamic time based on "how far along" current position `getPosition()` is from initial going to final position
        double time_step_millis = timeStep(initial_drive_position, final_drive_position, servo_position, acceleration, 13);
        // move motor by step and wait
        step(sign);
        sleep_ms(time_step_millis);
        return true;
    }

    // done driving
    init_drive_step = false;
    // command done; empty drive command
    drive_command.reset();
    return false;
}

void Servo::drive(int degrees, double acceleration) {
    // critical region: only allow the main thread access to drive_command contents/address
    mutex_enter_blocking(&driver_lock); // updating the command; only happens in critical region
    // check for and ignore noise
    if(std::abs(servo_position - degrees) <= 1) {
        mutex_exit(&driver_lock);
        return;
    }
    // check for and ignore duplicate sequential commands
    else if(drive_command && std::get<0>(*drive_command) == degrees && std::get<1>(*drive_command) == acceleration) {
        mutex_exit(&driver_lock);
        return;
    }
    // check for current command; delete it if present
    else if(drive_command) {
        drive_command.reset();
    }
    
    // create new command with given parameters
    drive_command = std::make_unique<std::tuple<int, double>>();
    *drive_command = std::make_tuple(degrees, acceleration);
    spin_driver_thread = true; // begin to spin thread for this servo

    // end of critical region: done handling drive_command
    mutex_exit(&driver_lock);
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