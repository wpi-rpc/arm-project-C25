#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>
#include <unistd.h>
#include <algorithm>

/**
 * @brief Represents an SG995 servo motor control. 
 */
class Servo {
    private: 
    
    const int PIN; // BCM pin number on the PI
    const int PWM_RANGE[2] = {500, 2500}; // usecs; range of PWM 

    public: 

    /**
     * @brief Constructs a Servo instance of the SG995 servo motor on the 
     * specified BCM pin. 
     * @param PIN (const inst) : The specified BCM pin
     */
    Servo(const int PIN) 
        :   PIN(PIN) {
        gpioSetMode(PIN, PI_OUTPUT);
    }

    /**
     * @brief Drives the motor to the specified degrees position. Drives within a given
     * range of [-90,+90] degrees such that zero degrees is the initial position and positive is counter-clockwise. 
     * @param degrees (int) : The specified degree position
     */
    void drive(int degrees) {
        degrees = std::max(std::min(degrees + 90, 180), 0); //std::clamp(degrees + 90, 0, 180); 
        double delta = PWM_RANGE[1] - PWM_RANGE[0];
        int value = (double)PWM_RANGE[0] + ((double)degrees / 180.0) * delta;
        gpioServo(PIN, value);
    }
};

int main() {
    // initialize pigpio library setup; exit if failure
    if(gpioInitialise() == -1) {
        exit(EXIT_FAILURE);
    }

    // construct motors
    Servo motor_A = Servo(6); 
    Servo motor_B = Servo(13);

    // drive with pauses
    motor_A.drive(-90);
    motor_B.drive(-90);
    sleep(2);
    motor_A.drive(90);
    motor_B.drive(90);
    sleep(2);
    motor_A.drive(0);
    motor_B.drive(0);
    sleep(2);

    printf("Done");
    // end use of pigpio library
    gpioTerminate();
    return 0;
}