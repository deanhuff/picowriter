#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "EPD_Test.h"
#include "EPD_2in9_V2.h"

static UBYTE *BlackImage;
static int cursorCol=0;
static int row=0;

void eink_init(){
    //intiialize waveshare Eink
    //DEV_Delay_ms(500);
    //play demo on screen
    //TODO get rid of the waveshare pictures
    //EPD_2in9_V2_test();
    DEV_Delay_ms(200);
    DEV_Module_Init();
    EPD_2IN9_V2_Init();
    EPD_2IN9_V2_Clear();

//    //Create a new image cache
    UWORD Imagesize = ((EPD_2IN9_V2_WIDTH % 8 == 0)? (EPD_2IN9_V2_WIDTH / 8 ): (EPD_2IN9_V2_WIDTH / 8 + 1)) * EPD_2IN9_V2_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
//        return -1;
    }

    Paint_NewImage(BlackImage, EPD_2IN9_V2_WIDTH, EPD_2IN9_V2_HEIGHT, 90, WHITE);
    Paint_Clear(WHITE);
    Paint_DrawString_EN(55, 40, "Welcome to", &Font24, WHITE, BLACK);
    Paint_DrawString_EN(55, 60, "Picowriter", &Font24, WHITE, BLACK);

    EPD_2IN9_V2_Display_Base(BlackImage);

}


void eink_print(uint8_t ch){

  //the canvas is 6 rows by 17 columns at font 24.

  if(cursorCol==0 && row==0){
    Paint_NewImage(BlackImage, EPD_2IN9_V2_WIDTH, EPD_2IN9_V2_HEIGHT, 90, WHITE);
    Paint_Clear(WHITE);
    Paint_SelectImage(BlackImage);
  }

  char str[2] = "\0"; //give {\0,\0}
  str[0] = ch;
  //send keystroke to epaper
  Paint_ClearWindows((cursorCol*Font24.Width), row*20, Font24.Width, row*20 + Font24.Height, WHITE);
  Paint_DrawString_EN((cursorCol*Font24.Width),row*20, str, &Font24, WHITE, BLACK);
  //Paint_DrawString_EN(5+(cursorCol*Font24.Width),row*20, "a", &Font24, WHITE, BLACK);
  EPD_2IN9_V2_Display_Partial(BlackImage);
  cursorCol+=1;
  if(cursorCol > 16){
    cursorCol=0;
    row+=1;
  }
}
