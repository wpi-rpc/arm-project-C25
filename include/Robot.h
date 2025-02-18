#ifndef ROBOT_H
#define ROBOT_H

/**
 * @brief Represents the high-level infrastructure of the Robot Arm. 
 */
class Robot {
    private:

    public:

    // robot loop delay
    const double delay = 0.01;

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