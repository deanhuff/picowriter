cmake_minimum_required(VERSION 3.5)

#set(BOARD "raspberry_pi_pico")
#set(FAMILY "rp2040")

include(pico_sdk_import.cmake)

#include($ENV{PICO_SDK_PATH}/lib/tinyusb/hw/bsp/family_support.cmake)
#include_directories($ENV{PICO_SDK_PATH}/lib/tinyusb/lib/tinyusb/src)
#family_get_project_name(PROJECT ${CMAKE_CURRENT_LIST_DIR})
project("picowrite")

pico_sdk_init()
#family_initialize_project(${PROJECT} ${CMAKE_CURRENT_LIST_DIR})


add_executable(test-usb_host_hid src/test/usb_host_hid.c)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(test-usb_host_hid pico_stdlib)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(test-usb_host_hid)

#family_configure_host_example(${PROJECT})
