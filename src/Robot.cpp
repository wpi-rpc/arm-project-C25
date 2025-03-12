#include "Robot.h"
#include "CommonGatewayInterface.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/time.h"
#include <cstdio>
#include <algorithm>

void Robot::digitalWrite(int pin, bool state) {
    cyw43_arch_gpio_put(pin, state);
}

bool Robot::digitalRead(int pin) {
    return cyw43_arch_gpio_get(pin);
}

uint32_t Robot::getMillis() {
    return to_ms_since_boot(get_absolute_time());
}

std::tuple<double, double, double, double> Robot::fetchJoystickData() {
    char* data[4]; // fetch 4D data
    CommonGatewayInterface::read(data);
    // cast to numerical values
    double num_data[4] = {atof(data[0]), atof(data[1]), atof(data[2]), atof(data[3])};
    // map numerical values to percentages 
    for(int i = 0; i < 4; i++) {
        num_data[i] = std::clamp((double)MAX_JOYSTICK_VALUE, (double)MAX_JOYSTICK_VALUE, std::abs((double)num_data[i])) / (double)MAX_JOYSTICK_VALUE;
    }

    // return tuple; invert y-values
    return std::make_tuple(num_data[0], -num_data[1], num_data[2], -num_data[3]);
}

void Robot::runJoysticks() {
    static int fetch_counter = 0; // used to regulate freq
    const int FETCH_FREQ_WRAP = DELAY_MILLIS / JOYSTICK_FETCH_FREQUENCY_HZ; // value to reset counter at 
    // only fetch when counter wraps
    if(fetch_counter >= FETCH_FREQ_WRAP) { 
        // reset counter and fetch data
        fetch_counter = 0;
        std::tuple<double, double, double, double> data = fetchJoystickData(); // fetch data
        x_y_joystick[0] = std::get<0>(data); // update joysticks
        x_y_joystick[1] = std::get<1>(data);
        z_EE_joystick[0] = std::get<2>(data);
        z_EE_joystick[1] = std::get<3>(data);
    }

    // increment counter
    fetch_counter++;
}

void Robot::setup() {
    // robot initilization code
    base.home();
    shoulder.home();
    elbow.home();
    // delay after homing; motor positioning unreliable until afterwards. 
    sleep_ms(5000);
}

int Robot::loop() {
    // main program loop
    runJoysticks();
    return 0;
}
