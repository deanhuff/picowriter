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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bsp/board.h"
#include "tusb.h"
#include "EPD_Test.h"
#include "EPD_2in9_V2.h"

void print_greeting(void);
void led_blinking_task(void);
extern void hid_app_task(void);
extern void eink_init();

int main(void) {
    //intialize waveshare Eink
    eink_init(); 

//    DEV_Delay_ms(500);
//    //play demo on screen
//    //TODO get rid of the waveshare pictures
//    //EPD_2in9_V2_test();
//    DEV_Module_Init();
//    EPD_2IN9_V2_Init();
//    EPD_2IN9_V2_Clear();
//
//    //Create a new image cache
//    UWORD Imagesize = ((EPD_2IN9_V2_WIDTH % 8 == 0)? (EPD_2IN9_V2_WIDTH / 8 ): (EPD_2IN9_V2_WIDTH / 8 + 1)) * EPD_2IN9_V2_HEIGHT;
//    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
//        printf("Failed to apply for black memory...\r\n");
//        return -1;
//    }
//    printf("Paint_NewImage\r\n");
//    Paint_NewImage(BlackImage, EPD_2IN9_V2_WIDTH, EPD_2IN9_V2_HEIGHT, 90, WHITE);
//    Paint_Clear(WHITE);
//    Paint_DrawString_EN(10, 0, "Picowriter", &Font24, WHITE, BLACK);
//
//    EPD_2IN9_V2_Display_Base(BlackImage);
//
    //initialize pico
    board_init();
   
    print_greeting();

    tusb_init();

    while (1) {
        // tinyusb host task
        tuh_task();
        //led_blinking_task();

#if CFG_TUH_HID_KEYBOARD || CFG_TUH_HID_MOUSE
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
    printf("This Host demo is configured to support:\n");
    if (CFG_TUH_HID_KEYBOARD) puts("  - HID Keyboard");
    if (CFG_TUH_HID_MOUSE) puts("  - HID Mouse");
}
