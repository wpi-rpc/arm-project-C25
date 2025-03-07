#include "AltThread.h"
#include "Robot.h"
#include "pico/multicore.h"
#include "pico/time.h"

std::vector<std::function<void()>> AltThread::threads = std::vector<std::function<void()>>();
void AltThread::registerThread(std::function<void()> thread) {
    // append thread
    AltThread::threads.push_back(thread);
}

void AltThread::pulse() {
    static uint32_t cycle_millis = 10000; // cycle time to wait between restarting the pulse sequence
    static uint32_t pulse_millis = Robot::getMillis(); // delay one pulse should remain HIGH and delay to remain LOW 
    static int pulse_count = 0; // the number of pulses executed in "this" sequence

    // if not all pulses for "this" sequence haven't been done or if we're on the last pulse and it's HIGH, then wait the pulse_millis time to change
    // the state between HIGH and LOW
    if((pulse_count < AltThread::threads.size() || Robot::digitalRead(Robot::BOARD_LED)) && Robot::getMillis() - pulse_millis > 1000) {
        Robot::digitalWrite(Robot::BOARD_LED, !Robot::digitalRead(Robot::BOARD_LED)); // to between HIGH and LOW
        pulse_millis = Robot::getMillis(); // update time since last pulse
        // notice if pulse is done, then the signal was LOW and is now HIGH; if high count new pulse
        pulse_count = (Robot::digitalRead(Robot::BOARD_LED)) ? pulse_count + 1 : pulse_count; // count completed pulses
    }
    // if pulses all counted then wait the cycle time to restart
    else if(pulse_count >= AltThread::threads.size() && Robot::getMillis() - pulse_millis > cycle_millis) { 
        pulse_count = 0;
    }
}

void AltThread::init() {
    // append thread handling to multicore
    multicore_launch_core1([](){
        while(true) { // loop indefinitely 
            // run all threads and pulse 
            for(auto thread : AltThread::threads) 
                thread();
            AltThread::pulse();
            sleep_ms(AltThread::DELAY_MILLIS);
        }
    });
}