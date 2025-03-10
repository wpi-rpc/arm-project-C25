# specify conditions 
cmake_minimum_required(VERSION 3.12)
set(PICO_BOARD pico_w) # SPECIFY BEFORE INCLUDE SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# define project
project(rpc_arm_C25 C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# If you want debug output from USB (pass -DPICO_STDIO_USB=1) this ensures you don't lose any debug output while USB is set up
set(PICO_STDIO_USB_CONNECT_WAIT_TIMEOUT_MS 3000)

# Initialize the SDK
pico_sdk_init()

add_compile_options(-Wall
    -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
    -Wno-unused-function # we have some for the docs that aren't called
)

add_compile_options(-Wno-maybe-uninitialized)
include_directories(include)
file(GLOB src_files src/*.cpp)
file(GLOB header_files include/*.h)
add_executable(rpc_arm_C25
    ${src_files} 
    ${header_files}
)

target_include_directories(rpc_arm_C25 PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR} 
    ${CMAKE_SOURCE_DIR}/include 
)

# pull in common dependencies
target_link_libraries(rpc_arm_C25 
    pico_cyw43_arch_lwip_threadsafe_background 
    pico_cyw43_arch_none 
    pico_lwip_http 
    pico_stdlib 
    hardware_pwm 
    pico_multicore 
    pico_time 
)

# enable comms
pico_enable_stdio_usb(rpc_arm_C25 1)
pico_enable_stdio_uart(rpc_arm_C25 0)
# create map/bin/hex file etc.
pico_add_extra_outputs(rpc_arm_C25)