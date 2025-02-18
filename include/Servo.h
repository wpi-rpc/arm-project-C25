#ifndef SERVO_H
#define SERVO_H

/**  
 * @brief Represents an SG995 servo motor control. 
 **/
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
    Servo(const int PIN);
    int getPin() const;

    /**
     * @brief Drives the motor to the specified degrees position. Drives within a given
     * range of [-90,+90] degrees such that zero degrees is the initial position and positive is counter-clockwise. 
     * @param degrees (int) : The specified degree position
     */
    void drive(int degrees);
};

#endif