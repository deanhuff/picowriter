#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "EPD_Test.h"
#include "EPD_2in9_V2.h"

static UBYTE *BlackImage;
static int cursorCol=0;
static int row=0;

void eink_init(){
    DEV_Delay_ms(200);
    DEV_Module_Init();
    EPD_2IN9_V2_Init();
    EPD_2IN9_V2_Clear();

    //Create a new image cache
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

char line0[17] = {0};
char line1[17] = {0};
char line2[17] = {0};
char line3[17] = {0};
char line4[17] = {0};

void eink_print(char printText[]){

  //the canvas is 5 rows by 17 columns at font 24.

  if(cursorCol==0 && row==0){
    Paint_NewImage(BlackImage, EPD_2IN9_V2_WIDTH, EPD_2IN9_V2_HEIGHT, 90, WHITE);
    Paint_Clear(WHITE);
    Paint_SelectImage(BlackImage);
  }

  int printLen = strlen(printText);
  int i=0;
  for (; i< printLen; i++){

    char c = printText[i];

    //erase the cursor '_' always
    Paint_DrawChar((cursorCol*Font24.Width),row*25,'_', &Font24, WHITE, WHITE);
    //printf("character is %d", c); 
    //??=delete 8=backspace
    if(c == 8){ 
      //decrement the cursor location
      cursorCol -=1;
      //check to see if we wrapped backwards.
      if(cursorCol <0){
        cursorCol=16;
        //bring the rows down.
         
        //TODO get prior rows from the buffer and redraw them.
        Paint_Clear(WHITE);
        strcpy(line4, line3);
        Paint_DrawString_EN(0,75, line3, &Font24, WHITE, BLACK);
        strcpy(line3, line2);
        Paint_DrawString_EN(0,50, line2, &Font24, WHITE, BLACK);
        strcpy(line2, line1);
        Paint_DrawString_EN(0,25, line1, &Font24, WHITE, BLACK);
        strcpy(line1, line0);
        Paint_DrawString_EN(0,0, line0, &Font24, WHITE, BLACK);
        row-=1;
        if(row<0){row=0;}
      }
      
      if(row==0){
        c = line0[cursorCol];
        line0[cursorCol]='\0';
      }else if(row == 1){
        c = line1[cursorCol];
        line1[cursorCol]='\0';
      }else if(row == 2){
        c = line2[cursorCol];
        line2[cursorCol]='\0';
      }else if(row == 3){
        c = line3[cursorCol];
        line3[cursorCol]='\0';
      }else if(row == 4){
        c = line4[cursorCol];
        line4[cursorCol]='\0';
      }
      //erase the character.
      Paint_DrawChar((cursorCol*Font24.Width),row*25,c, &Font24, WHITE, WHITE);
      
    }else{
      //draw a character.
      Paint_DrawChar((cursorCol*Font24.Width),row*25, c, &Font24, BLACK, WHITE);

      if(row==0){
        line0[cursorCol]=printText[i];
      }else if(row == 1){
        line1[cursorCol]=printText[i];
      }else if(row == 2){
        line2[cursorCol]=printText[i];
      }else if(row == 3){
        line3[cursorCol]=printText[i];
      }else if(row == 4){
        line4[cursorCol]=printText[i];
      }

      cursorCol +=1;

      if(cursorCol > 16){
        cursorCol=0;
        row+=1;
        if(row > 4){
          //we've hit bottom, advance all up 1.
          Paint_Clear(WHITE);
          strcpy(line0, line1);
          Paint_DrawString_EN(0,0, line0, &Font24, WHITE, BLACK);
          strcpy(line1, line2);
          Paint_DrawString_EN(0,25, line1, &Font24, WHITE, BLACK);
          strcpy(line2, line3);
          Paint_DrawString_EN(0,50, line2, &Font24, WHITE, BLACK);
          strcpy(line3, line4);
          Paint_DrawString_EN(0,75, line3, &Font24, WHITE, BLACK);
          row=4;
        }
      }
    }

  }
 
  //draw the cursor. 
  Paint_DrawChar((cursorCol*Font24.Width),row*25, '_', &Font24, BLACK, WHITE);

  EPD_2IN9_V2_Display_Partial(BlackImage);

}

void eink_print_old(char printText[]){

  //the canvas is 5 rows by 17 columns at font 24.

  if(cursorCol==0 && row==0){
    Paint_NewImage(BlackImage, EPD_2IN9_V2_WIDTH, EPD_2IN9_V2_HEIGHT, 90, WHITE);
    Paint_Clear(WHITE);
    Paint_SelectImage(BlackImage);
  }

  int printLen = strlen(printText);
  
  if(cursorCol + printLen > 17){
    
    char * pch;
    pch = strtok (printText, " ");

    while (pch != NULL){
      printLen = strlen(pch);
      if(cursorCol + printLen > 17){
        break;
        //the default print statement will take care of the rest of the drawing 
        //on the next line.
      }else{
         Paint_DrawString_EN((cursorCol*Font24.Width),row*25, pch, &Font24, WHITE, BLACK);
         cursorCol+=printLen;
         Paint_DrawString_EN((cursorCol*Font24.Width),row*25,"_", &Font24, WHITE, WHITE);
      }

      pch = strtok (NULL, " ");
    }

    row+=1;
    cursorCol=0;
    if(row > 4){
      row=0;
      Paint_Clear(WHITE);
      Paint_DrawString_EN((cursorCol*Font24.Width),row*25,"_", &Font24, WHITE, BLACK);
    }

    strcpy(printText, pch);
  }

  //send keystroke to epaper
//  Paint_ClearWindows((cursorCol*Font24.Width), row*25, Font24.Width, row*25 + Font24.Height, WHITE);
  Paint_DrawString_EN((cursorCol*Font24.Width),row*25,"_", &Font24, WHITE, WHITE);
  Paint_DrawString_EN((cursorCol*Font24.Width),row*25, printText, &Font24, WHITE, BLACK);

  //resolution is 296x128=37888
  //each byte in the image contains 8bit so there are 
  //4736 "bytes" in the image
  //each "column" of 8 pixels wide is 128bytes in length
  //so there are 4736/128 37 columns of pixels
  
  cursorCol+=printLen-1;
  if(cursorCol > 16){
    Paint_DrawString_EN((cursorCol*Font24.Width),row*25,"_", &Font24, WHITE, WHITE);
    cursorCol=0;
    row+=1;
    Paint_DrawString_EN((cursorCol*Font24.Width),row*25,"_", &Font24, WHITE, BLACK);
  }

  if(row > 4){
    Paint_DrawString_EN((cursorCol*Font24.Width),row*25,"_", &Font24, WHITE, WHITE);
    row=0;
    cursorCol=0;
    Paint_Clear(WHITE);
    Paint_DrawString_EN((cursorCol*Font24.Width),row*25,"_", &Font24, WHITE, BLACK);
  }
  EPD_2IN9_V2_Display_Partial(BlackImage);
}
