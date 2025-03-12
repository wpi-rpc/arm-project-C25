#include "CommonGatewayInterface.h"

// CGI definitions
const char* CommonGatewayInterface::QUERY_PARAMETERS[CommonGatewayInterface::QUERY_PARAMETER_COUNT] = {"joyl_dx", "joyl_dy", "joyr_dx", "joyr_dy"};
std::string CommonGatewayInterface::query_parameter_data[CommonGatewayInterface::QUERY_PARAMETER_COUNT] = {"", "", "", ""};
const tCGI CommonGatewayInterface::CGI_SCRIPTS[CommonGatewayInterface::CGI_SCRIPT_COUNT] = {{"/joy.cgi", CommonGatewayInterface::handler}};

const char* CommonGatewayInterface::handler(int i_index, int i_num_params, char* pc_params[], char* pc_value[]) {
    for(int i = 0; i < i_num_params; i++) { 
        // check any parameters belonging to .cgi scripts
        for(int j = 0; j < QUERY_PARAMETER_COUNT; j++) {
            if(strcmp(pc_params[i], QUERY_PARAMETERS[j]) == 0) {
                query_parameter_data[j] = std::string(pc_value[i]);
            }
        }
    }

    return "/index.shtml";
}

void CommonGatewayInterface::init() {
    http_set_cgi_handlers(CGI_SCRIPTS, CGI_SCRIPT_COUNT);
}

void CommonGatewayInterface::read(char* buffer[]) {
    for(int i = 0; i < QUERY_PARAMETER_COUNT; i++) {
        buffer[i] = (char*)query_parameter_data[i].c_str();
    }
}