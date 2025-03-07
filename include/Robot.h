#include "Servo.h"
#include "pico/cyw43_arch.h"

#ifndef ROBOT_H
#define ROBOT_H

/**
 * @brief Represents the high-level infrastructure of the Robot Arm. 
 */
class Robot {
    private:

    // Defined robot servos 
    Servo end_effector = Servo(ServoIDs::END_EFFECTOR, 0); // (joint 4)
    Servo wrist = Servo(ServoIDs::WRIST, 0); // (joint 3)
    Servo elbow = Servo(ServoIDs::ELBOW, 0); // (joint 2)
    Servo shoulder = Servo(ServoIDs::SHOULDER, 0); // (joint 1)
    Servo base = Servo(ServoIDs::BASE, 0); // base to rotate about (joint 0)

    public:

    /**
     * @brief Motod IDs for each robot arm joint.
     */
    enum class ServoID {
        BASE = 0, 
        SHOULDER = 2, 
        ELBOW = 1, 
        WRIST = 3, 
        END_EFFECTOR = 0
    } typedef ServoIDs;

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