#include <thread>
#include <mutex>
#include <memory>
#include <tuple>

#ifndef SERVO_H
#define SERVO_H

/**  
 * @brief Represents an SG995 servo motor control. 
 **/
class Servo {
    public: 
    
    const int PIN; // BCM pin number on the PI
    const motor_type TYPE; // type of motor_type for a servo
    const int HOME_POSITION; // home degree position of the servo
    const int PWM_RANGE[2] = {500, 2500}; // usecs; range of PWM 
    bool spin_driver_thread = true; // allows threaded motor control to remain active; terminates thread when false
    std::mutex driver_lock = std::mutex(); // mutex for handling access to drive_command ptr
    std::unique_ptr<std::thread> driver_thread = std::unique_ptr<std::thread>(); // handles threaded motor control
    std::unique_ptr<std::tuple<int, double>> drive_command = std::unique_ptr<std::tuple<int, double>>(); 
    int servo_position = 0;
    
    /***
     * @brief Defines the motor type
     */
    enum class motor_type {
        MS62 = 0, // motor position range [0,270] degrees
        MG995 = 1 // motor position range [0,180] degrees
    } typedef motor_types;

    /**
     * @brief gets the range of the motor type in degrees
     * @param type (motor_type) : the sepcified type of servo being used
     * @return the range of the motor in degrees
     */
    std::tuple<int, int> getRange(motor_type type);
    /**
     * @brief Calculates the time step delay at a given degree position provided the path the servo needs to rotate. 
     * Times steps are also determined by a desired acceleration
     * @param initial_position (int) : The specified initial degree position of the servo
     * @param final_position (int) : The specified final degree position of the servo
     * @param position (int) : The specified position of the time step. 
     * @param acceleration (double) : The specified acceleration in [degrees / sec^2]
     * @param max_speed (double) : The specified max speed in [degrees / sec]
     * @return The calculated time step in milliseconds  
     */
    int timeStep(int initial_position, int final_position, int position, double acceleration, double max_speed); 

    /**
     * @brief Drives the motor to the specified degrees position. Drives within a given
     * range of [-90,+90] degrees such that zero degrees is the initial position and positive is counter-clockwise. 
     * @param degrees (int) : The specified degree position
     */
    void position(int degrees);

    /**
     * @brief Drives the servo to a new local relative position. Pushing the servo position 
     * in the direction and distance of the specified degrees. This is a relative local change
     * in positioning when driving the servo, and as such, the magnitude of degrees should be small. 
     * @note The specified degrees is clamped in range of [-10, +10] degrees. 
     * @param degrees (int) : The specified relative change in position
     */
     void step(int degrees);

     /**
      * @brief Drives the servo to a given position with a specified acceleration. This is done
      * as a blocking control-flow algorithm. 
      * @param degrees (int) : The specified target position 
      * @param acceleration (double) : The specified acceleration in [degrees / sec^2]
      */
     void driveBlocking(int degrees, double acceleration);

    public: 

    /**
     * @brief Constructs a Servo instance of the SG995 servo motor on the 
     * specified BCM pin. 
     * @param PIN (const inst) : The specified BCM pin
     * @param TYPE (const motor_type) : the specified motor type
     * @param HOME_POSITION (const int) : The specified home position of the servo in degrees
     */
    Servo(const int PIN, const motor_type TYPE, const int HOME_POSITION);
    
    /**
     * @brief Deconstructs a Servo instance. 
     */
    ~Servo();

    /**
     * @brief Homes the servo to drive to its home position in degrees. 
     */
    void home();

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
     * @brief Drives the servo to a given position with a specified acceleration. 
     * @param degrees (int) : The specified target position
     * @param acceleration (double) : The specified acceleration in [degrees / sec^2]
     */
    void drive(int degrees, double acceleration);
};

#endif