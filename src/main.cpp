#include "Robot.h"
#include "Servo.h"
#include "AltThread.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/cyw43_arch.h"
#include "hardware/pwm.h"

int main() {
    // initialize std pico libraries
    stdio_init_all();
    // initialize Wi-Fi chip
    cyw43_arch_init();  
    // initialize multicore for servo driving
    AltThread::init();
    // create robot and initialize
    Robot robot = Robot();
    robot.setup();
    // run program and spin
    while(robot.loop() == 0) {
        sleep_ms(Robot::DELAY_MILLIS);
    }
    
    return 0;
}