#include "Servo.h"

#ifndef ROBOT_H
#define ROBOT_H

/**
 * @brief Represents the high-level infrastructure of the Robot Arm. 
 */
class Robot {
    private:

    Servo shoulder = Servo(13, 0);
    Servo elbow = Servo(6, 0);

    public:

    // robot loop delay in millis
    static const int DELAY_MILLIS = 5;
    static const int DEFAULT_CLOCK_SPEED = 125000000; //MHz clock frequency

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