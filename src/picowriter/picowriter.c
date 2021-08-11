/* 
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include <stdio.h>
#include <string.h>

//pico-sdk
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/irq.h"
//#include "hardware/adc.h"

//tinyusb
#include "bsp/board.h"
#include "tusb.h"

//waveshare eink
#include "EPD_Test.h"
#include "EPD_2in9_V2.h"

void print_greeting(void);
void led_blinking_task(void);
extern void hid_app_task(void);
extern void eink_init();
extern void eink_print();

//global variables
//uint8_t newchar;
char model[512] = {0};
char unprinted[128] = {0};
int unprintedLoc = 0;
int cursorLoc = 0;
int printLoc = 0;
bool needsPaint = false;

void key_press(uint8_t key){
  //newchar = key;
  //input key press into the model
  unprinted[unprintedLoc]=key; 
  //TODO special case for delete, backspace, arrow to reposition the cursor.
  unprintedLoc +=1; 
  needsPaint = true;

}

void core1_entry(){

    // Infinte While Loop to wait for the model to change
    while (1){
        if(needsPaint){
          char printText[128] = {0};
          strcpy(printText, unprinted);
          memset(unprinted, 0, sizeof unprinted);
          unprintedLoc=0;
          needsPaint=false;
          printf("printing: %s\n", printText);
          eink_print(printText);
        }else{
          sleep_ms(20);
	}
    }

}

int main(void) {
    stdio_init_all();

    //double core1 launch needed to reset core 1 during a rebuild
    multicore_launch_core1(core1_entry);
    sleep_ms(1);
    multicore_reset_core1();
    multicore_launch_core1(core1_entry);

    eink_init();

    board_init();
    tusb_init();

    //wait for keyboard input
    while (1) {
      tuh_task();
      #if CFG_TUH_HID_KEYBOARD
        hid_app_task();
      #endif
    }

    return 0;
}

//--------------------------------------------------------------------+
// BLINKING TASK
//--------------------------------------------------------------------+
void led_blinking_task(void) {
    const uint32_t interval_ms = 250;
    static uint32_t start_ms = 0;

    static bool led_state = false;

    // Blink every interval ms
    if (board_millis() - start_ms < interval_ms) return; // not enough time
    start_ms += interval_ms;

    board_led_write(led_state);
    led_state = 1 - led_state; // toggle
}

//--------------------------------------------------------------------+
// HELPER FUNCTION
//--------------------------------------------------------------------+
void print_greeting(void) {
    printf("Picowriter is running, scanning for HID Keyboard.\n");
    if (CFG_TUH_HID_KEYBOARD) puts("  - HID Keyboard");
}
