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
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/irq.h"
#include "hardware/adc.h"
#include <string.h>

#include "bsp/board.h"
#include "tusb.h"
#include "EPD_Test.h"
#include "EPD_2in9_V2.h"

void print_greeting(void);
void led_blinking_task(void);
extern void hid_app_task(void);
extern void eink_init();
extern void eink_print(uint8_t ch);

uint8_t newchar;
bool needsPaint = false;
bool busy = true;

void key_press(uint8_t key){
  newchar = key;
  needsPaint = true;
}

void core1_entry(){

    // Infinte While Loop to wait for interrupt
    while (1){
        if(needsPaint && !busy){
        //if(needsPaint){
          needsPaint=false;
          printf("printing: %c\n", newchar);
          eink_print(newchar);
        }else{
          sleep_ms(300);
	}
    }

}

int main(void) {
    stdio_init_all();
    multicore_launch_core1(core1_entry);
    sleep_ms(1);
    multicore_reset_core1();
    multicore_launch_core1(core1_entry);

    eink_init();
    busy=false;

//    adc_init();
//    adc_gpio_init(13);
//    adc_select_input(0);    


    board_init();
    tusb_init();

    while (1) {

      tuh_task();

      #if CFG_TUH_HID_KEYBOARD || CFG_TUH_HID_MOUSE
        hid_app_task();
      #endif
       

     //  multicore_fifo_push_blocking(123);
     //  sleep_ms(1000);

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
    printf("This Host demo is configured to support:\n");
    if (CFG_TUH_HID_KEYBOARD) puts("  - HID Keyboard");
    if (CFG_TUH_HID_MOUSE) puts("  - HID Mouse");
}
