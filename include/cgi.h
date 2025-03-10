#include "lwip/apps/httpd.h"
#include "pico/cyw43_arch.h"
#include "Robot.h"

const char* cgiHandler(int i_index, int i_num_params, char* pc_params[], char* pc_value[]) {
    float left_delta_x = 0;
    float left_delta_y = 0;
    float right_delta_x = 0;
    float right_delta_y = 0;

    for(int i = 0; i < i_num_params; i++) { 
        // check any parameters belonging to .cgi entities
        if(strcmp(pc_params[i], "joyl_dx") == 0) 
            left_delta_x = atof(pc_value[i]);
        if(strcmp(pc_params[i], "joyl_dy") == 0) 
            left_delta_y = atof(pc_value[i]);
        if(strcmp(pc_params[i], "joyr_dx") == 0) 
            right_delta_x = atof(pc_value[i]);
        if(strcmp(pc_params[i], "joyr_dy") == 0)
            right_delta_y = atof(pc_value[i]);
    }

    printf("Left Joystick: (X,Y) = (%f, %f)\n", left_delta_x, left_delta_y);
    printf("Right Joystick: (X,Y) = (%f, %f)\n", right_delta_x, right_delta_y);
    // send the index page
    return "/index.shtml";
}

// .cgi entities
static const tCGI cgi_handlers[] = { 
    {"/joy.cgi", cgiHandler}
};

void cgi_init() {
    http_set_cgi_handlers(cgi_handlers, 1);
}