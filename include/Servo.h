#ifndef SERVO_H
#define SERVO_H

/**  
 * @brief Represents an SG995 servo motor control. 
 **/
class Servo {
    private: 
    
    const int PIN; // BCM pin number on the PI
    const int PWM_RANGE[2] = {500, 2500}; // usecs; range of PWM 
    int servo_position = 0;
    
    /**
     * @brief Calculates the time step delay at a given degree position provided the path the servo needs to rotate. 
     * Times steps are also determined by a desired acceleration
     * @param initial_position (int) : The specified initial degree position of the servo
     * @param final_position (int) : The specified final degree position of the servo
     * @param position (int) : The specified position of the time step. 
     * @param acceleration (double) : The specified acceleration in [degrees / sec^2]
     * @param max_time_step (int) : The specified maximum the time step in milliseconds that it can be
     * @return The calculated time step in milliseconds  
     */
    int timeStep(int initial_position, int final_position, int position, double acceleration, int max_time_step); 

    /**
     * @brief Drives the motor to the specified degrees position. Drives within a given
     * range of [-90,+90] degrees such that zero degrees is the initial position and positive is counter-clockwise. 
     * @param degrees (int) : The specified degree position
     */
    void position(int degrees);

    public: 

    /**
     * @brief Constructs a Servo instance of the SG995 servo motor on the 
     * specified BCM pin. 
     * @param PIN (const inst) : The specified BCM pin
     */
    Servo(const int PIN);
    
    /**
     * @brief The assigned BCM pin of the servo. 
     * @return The servo BCM pin.
     */
    int getPin() const;

    /**
     * @brief The approximate feed-forward position of the servo in degrees. 
     * @return The servo position in degrees.
     */
    int getPosition();

    /**
     * @brief Drives the servo to a new local relative position. Pushing the servo position 
     * in the direction and distance of the specified degrees. This is a relative local change
     * in positioning when driving the servo, and as such, the magnitude of degrees should be small. 
     * @note The specified degrees is clamped in range of [-10, +10] degrees. 
     * @param degrees (int) : The specified relative change in position
     */
    void step(int degrees);
};

#endif