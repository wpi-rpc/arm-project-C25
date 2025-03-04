#include "Robot.h"
#include "Servo.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/pwm.h"

int main() {
    // initialize libraries
    stdio_init_all();
    // initialize multicore for servo driving
    multicore_launch_core1(Servo::SERVO_MULTICORE);
    // create robot and initialize
    Robot robot = Robot();
    robot.setup();
    // run program and spin
    while(robot.loop() == 0) {
        sleep_ms(Robot::DELAY_MILLIS);
    }
    
    return 0;
}