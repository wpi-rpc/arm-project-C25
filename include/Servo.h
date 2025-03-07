#include "pico/multicore.h"
#include "pico/mutex.h"
#include <functional>
#include <memory>
#include <tuple>
#include <set>

#ifndef SERVO_H
#define SERVO_H

/**  
 * @brief Represents an SG995 servo motor control. 
 **/
class Servo : public std::enable_shared_from_this<Servo> {
    public: 
    
    static const int CLOCK_FREQUENCY = 50; //Hz
    const int PIN; // BCM pin number on the PI
    const int PIN_SLICE; // PWM slice and channel of the pin
    const int PIN_CHANNEL;
    const int HOME_POSITION; // home degree position of the servo
    const int PWM_RANGE[2] = {1638, 8192};//{3277, 6553}; //{500, 2500}; // usecs; range of PWM levels 
    bool spin_driver_thread = true; // allows threaded motor control to remain active; terminates thread when false
    mutex_t driver_lock; // mutex for handling access to drive_command ptr
    std::function<void()> driver_thread = std::function<void()>(); // handles threaded motor control
    std::unique_ptr<std::tuple<int, double>> drive_command = std::unique_ptr<std::tuple<int, double>>(); 
    int initial_drive_position = 0; // initial position when motion profiling
    int final_drive_position = 0; // destination position when motion profiling
    double drive_acceleration = 0; // acceleration when motion profiling
    int servo_position = 0; // current servo position [-90,+90]
    bool init_drive_step = false; // flags if the drive stepping for motion profiling has been initialized

    /**
     * @brief Iterative method for handling non-blocking driving for servo motion profiling control. 
     */
    void driverThread();

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
     * @brief Initializes the servo to begin driving with motion profile by iterative drive stepping.
     * @param target_degrees (int) : The specified target position to drive to after initializing
     * @param acceleratoin (double) : The specified acceleration when driving
     */
    void initDriveStep(int target_degrees, double acceleration);

     /**
      * @brief Drives the servo to a given position with a specified acceleration. This is done
      * by iteratively calling this method in a control-flow algorithm. 
      * @return Whether or not driving is done. 
      */
     bool driveStep();

    public: 

    /**
     * @brief Constructs a Servo instance of the SG995 servo motor on the 
     * specified BCM pin. 
     * @param PIN (const inst) : The specified BCM pin
     * @param HOME_POSITION (const int) : The specified home position of the servo in degrees
     */
    Servo(const int PIN, const int HOME_POSITION);
    
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