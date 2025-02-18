#include "Robot.h"
#include <pigpio.h>

int main() {
    gpioInitialise();
    // create robot and initialize
    Robot robot = Robot();
    robot.setup();
    // run program and spin
    while(robot.loop() == 0) {
        time_sleep(robot.delay);
    }
    gpioTerminate();
    return 0;
}