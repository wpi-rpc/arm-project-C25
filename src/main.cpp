#include "Robot.h"
#include <pigpio.h>

int main() {
    // create robot and initialize
    Robot robot = Robot();
    robot.setup();
    // run program and spin
    while(robot.loop() == 0) {
        time_sleep(robot.delay);
    }

    return 0;
}