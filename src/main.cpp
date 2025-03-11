#include "Robot.h"
#include "AltThread.h"
#include "CommonGatewayInterface.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/pwm.h"

#include "lwip/apps/httpd.h"
#include "lwipopts.h"

// server ip: autoreg-6781393.dyn.wpi.edu
const char WIFI_SSID[] = "Galaxy_A53";//"WPI-PSK"; 
const char WIFI_PASSWORD[] = "cngj3643";//"{WtSu\":owj0Ts9I{"; 

int main() {
    // initialize std pico libraries
    stdio_init_all();
    // initialize Wi-Fi chip
    cyw43_arch_init();
    cyw43_arch_enable_sta_mode();
    
    // attempt to connect to WiFi
    while(cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("attempting to connect...\n");
    }

    printf("connected to %s\n", WIFI_SSID);
    // initialize web server and CGI
    httpd_init(); 
    CommonGatewayInterface::init();
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