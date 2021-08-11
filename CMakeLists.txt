cmake_minimum_required(VERSION 3.5)

set(BOARD "raspberry_pi_pico")
set(FAMILY "rp2040")

include(pico_sdk_import.cmake)
#include($ENV{PICO_EXAMPLES_PATH}/example_auto_set_url.cmake)
include($ENV{PICO_SDK_PATH}/lib/tinyusb/hw/bsp/family_support.cmake)
include_directories($ENV{PICO_SDK_PATH}/lib/tinyusb/lib/tinyusb/src)

project("raspberry_pi_pico-picowrite")

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()
#family_initialize_project("raspberry_pi_pico-picowrite" ${CMAKE_CURRENT_LIST_DIR})

# add a compilation subdirectory
add_subdirectory(src/waveshare_eink/c/lib/Config)
add_subdirectory(src/waveshare_eink/c/lib/e-Paper)
add_subdirectory(src/waveshare_eink/c/lib/Fonts)
add_subdirectory(src/waveshare_eink/c/lib/GUI)
add_subdirectory(src/waveshare_eink/c/examples)

# add a header directory
include_directories(src/waveshare_eink/c/examples)
include_directories(src/waveshare_eink/c/lib/e-Paper)
include_directories(src/waveshare_eink/c/lib/Config)
include_directories(src/waveshare_eink/c/lib/GUI)

add_executable(raspberry_pi_pico-picowrite src/picowriter/picowriter.c src/picowriter/hid_app.c src/picowriter/eink.c)

include_directories(${CMAKE_CURRENT_LIST_DIR}/src/picowriter)

# Add pico_stdlib library which aggregates commonly used features
#target_link_libraries(raspberry_pi_pico-picowrite pico_stdlib tinyusb_host tinyusb_board)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(raspberry_pi_pico-picowrite)
#example_auto_set_url(test-usb_host_hid)
#family_configure_host_example("raspberry_pi_pico-picowrite")
target_link_libraries(raspberry_pi_pico-picowrite examples ePaper GUI Fonts Config pico_stdlib tinyusb_host tinyusb_board pico_multicore hardware_adc)
