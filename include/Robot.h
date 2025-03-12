#include "Servo.h"
#include "pico/cyw43_arch.h"
#include "tusb.h"
#include <tuple>

#ifndef ROBOT_H
#define ROBOT_H

/**
 * @brief Represents the high-level infrastructure of the Robot Arm. 
 */
class Robot {
    public:

    // Robot joystick mapping
    const int JOYSTICK_FETCH_FREQUENCY_HZ = 4; // joystick data fetch frequency in Hz 
    const double MAX_JOYSTICK_VALUE = 15.0; // max cap at 25.0;
    double x_y_joystick[2] = {0.0, 0.0}; // X/Y End-Effector (EE) speeds 
    double z_EE_joystick[2] = {0.0, 0.0};// Z End-Effector (EE) speed and EE speed
    // Defined robot servos 
    Servo end_effector = Servo((int)ServoID::END_EFFECTOR, 0); // (joint 4)
    Servo wrist = Servo((int)ServoID::WRIST, 0); // (joint 3)
    Servo elbow = Servo((int)ServoID::ELBOW, 0); // (joint 2)
    Servo shoulder = Servo((int)ServoID::SHOULDER, 0); // (joint 1)
    Servo base = Servo((int)ServoID::BASE, 0); // base to rotate about (joint 0)

    /**
     * @brief Motod IDs for each robot arm joint.
     */
    enum class ServoID {
        BASE = 0, // wrist = 4, shoulder = 3, end-effector = 2, elbow = 1, base = 0   
        SHOULDER = 3, 
        ELBOW = 1, 
        WRIST = 4, 
        END_EFFECTOR = 2
    };

    // robot loop delay in millis
    static const int DELAY_MILLIS = 5;
    // robot on-board Pico W LED
    static const int BOARD_LED = CYW43_WL_GPIO_LED_PIN;
    static const int DEFAULT_CLOCK_SPEED = 125000000; //MHz clock frequency

    /**
     * @brief Writes a GPIO Pico W pin to HIGH or LOW by specifying true or false respectively. 
     * @param state (bool) : The specified state
     */
    static void digitalWrite(int pin, bool state);

    /**
     * @brief Reads the GPIO Pico W pin state that should be HIGH or LOW by returning true or false respectively. 
     * @returns The pin state
     */
    static bool digitalRead(int pin);

    /** 
     * @brief Used to return the elapsed time in milliseconds since booting up the Pico W. 
    */
    static uint32_t getMillis();

    /**
     * @brief Fetches most recent joystick data and maps to clamped [-1.0,+1.0] range along both axis. 
     * @return A 4D tuple of mapped x/y percents along both controllers (x1, y1, x2, y2).
     */
    std::tuple<double, double, double, double> fetchJoystickData(); 

    /**
     * @brief Runs and updates the joystick controller data when iteratively called. 
     * Stores updated data in joystick objects `x_y_joystick` and `z_EE_joystick`
     */
    void runJoysticks();

    /**
     * @brief The setup for the robot. This is only called once upon running the progam. 
     */
    void setup();

    /**
     * @brief Runs the robot at runtime when iteratively called; the loop stops iterating if a unsuccessful result is returned. 
     * @returns The program status such that zero is a successful result. 
     */
    int loop();
};

#endif