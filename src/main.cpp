#include "Robot.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/pwm.h"

int main() {
    //gpioInitialise();
    // create robot and initialize
    Robot robot = Robot();
    robot.setup();
    // run program and spin
    while(robot.loop() == 0) {
        sleep_ms(robot.delay);
    }
    //gpioTerminate();
    return 0;
}