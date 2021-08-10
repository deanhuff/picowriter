cmake_minimum_required(VERSION 3.12)

include($ENV{PICO_EXAMPLES_PATH}/pico_sdk_import.cmake)

project(pico-multicore)

pico_sdk_init()

add_executable(multicore
        multicore.c
)

target_link_libraries(multicore
        pico_multicore
        pico_stdlib
        hardware_adc
)

pico_enable_stdio_usb(multicore 0)
pico_enable_stdio_uart(multicore 1)

pico_add_extra_outputs(multicore)
