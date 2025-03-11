#include "lwip/apps/httpd.h"
#include "pico/cyw43_arch.h"
#include <string.h>
#include "Robot.h"

#ifndef COMMON_GATEWAY_INTERFACE
#define COMMON_GATEWAY_INTERFACE

/**
 * @brief Represents the interface for retrieving data from the project web server. 
 */
class CommonGatewayInterface {
    public: 

    // hard-coded CGI querying setup
    static const int QUERY_PARAMETER_COUNT = 4; // expected parameters 
    static const int CGI_SCRIPT_COUNT = 1; // exepected scripts; only one script and handler 

    private:

    static const char* QUERY_PARAMETERS[QUERY_PARAMETER_COUNT]; // raw param names
    static std::string query_parameter_data[QUERY_PARAMETER_COUNT]; // raw param data
    static const tCGI CGI_SCRIPTS[CGI_SCRIPT_COUNT]; 

    /**
     * @brief Parses CGI script query parameters fetched from the web server.
     * @param i_index (int) : The index of the CGI script (e.g. /example.cgi)
     * @param i_num_params (int) : The number of query parameters from the CGI script 
     * @param pc_params (char**) : The array of query parameter variable names 
     * @param pc_value (char**) : The array of corresponding query parameter data
     */
    static const char* handler(int i_index, int i_num_params, char* pc_params[], char* pc_value[]);

    public:

    /**
     * @brief Initializes CGI for reading web server data.
     */
    static void init();

    /**
     * @brief Reads all CGI query parameters' data pulled from the web server. 
     * Stores parameter data into given buffer assuming min length of `QUERY_PARAMETER_COUNT`
     * @param buffer (char**) : The specified buffer
     */
    static void read(char* buffer[]);
};

#endif