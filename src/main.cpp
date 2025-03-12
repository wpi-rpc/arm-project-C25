#include "Robot.h"
#include "AltThread.h"
#include "CommonGatewayInterface.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/pwm.h"

#include "lwip/apps/httpd.h"
#include "lwipopts.h"

// wifi always enabled and accessed up running.
// macro either enables campus or other wifi connection
// #define ENABLE_WPI_WIRELESS_WIFI 

// campus and other wifi network info
#define WPI_WIFI_SSID           "WPI-PSK"
#define WPI_WIFI_PASSWORD       "{WtSu\":owj0Ts9I{"
#define OTHER_WIFI_SSID         "Galaxy_A53"
#define OTHER_WIFI_PASSWORD     "cngj3643"

/**************************************************/
/* DEVICE IP ADDRESS / WEBSITE URL:               */
/*                                                */
/* WPI WIFI:   http:/autoreg-6781393.dyn.wpi.edu  */
/* OTHER WIFI: http://192.168.194.145/            */
/**************************************************/ 

// device wifi ssid
const char WIFI_SSID[] = 
#ifdef ENABLE_WPI_WIRELESS_WIFI 
    WPI_WIFI_SSID; 
#else 
    OTHER_WIFI_SSID; 
#endif
// device wifi password
const char WIFI_PASSWORD[] = 
#ifdef ENABLE_WPI_WIRELESS_WIFI 
    WPI_WIFI_SSID; 
#else 
    OTHER_WIFI_PASSWORD; 
#endif

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