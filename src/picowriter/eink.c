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


// Note: This function returns a pointer to a substring of the original string.
// If the given string was allocated dynamically, the caller must not overwrite
// that pointer with the returned value, since the original pointer must be
// deallocated using the same allocator with which it was allocated.  The return
// value must NOT be deallocated using free() etc.
char *trimwhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator character
  end[1] = '\0';

  return str;
}

void eink_print(char printText[]){

  //the canvas is 6 rows by 17 columns at font 24.

  if(cursorCol==0 && row==0){
    Paint_NewImage(BlackImage, EPD_2IN9_V2_WIDTH, EPD_2IN9_V2_HEIGHT, 90, WHITE);
    Paint_Clear(WHITE);
    Paint_SelectImage(BlackImage);
  }

//  char str[2] = "\0"; //give {\0,\0}
//  str[0] = ch;

  int printLen = strlen(printText);
  
  if(cursorCol + printLen > 17){
    //TODO if the text to be printed contains a space, splice on that and see if it can be written.
    
    char * pch;
    pch = strtok (printText, " ");
    pch = trimwhitespace(pch);

    while (pch != NULL){
      printLen = strlen(pch);
      if(cursorCol + printLen > 17){
        break;
        //the default print statement will take care of the rest of the drawing 
        //on the next line.
      }else{
         Paint_DrawString_EN((cursorCol*Font24.Width),row*25, pch, &Font24, WHITE, BLACK);
         cursorCol+=printLen;
      }

      pch = strtok (NULL, " ");
      pch = trimwhitespace(pch);
    }

    row+=1;
    cursorCol=0;
    if(row > 4){
      row=0;
      Paint_Clear(WHITE);
    }

    strcpy(printText, pch);
  }

  //send keystroke to epaper
  //Paint_ClearWindows((cursorCol*Font24.Width), row*25, Font24.Width, row*25 + Font24.Height, WHITE);
  Paint_DrawString_EN((cursorCol*Font24.Width),row*25, printText, &Font24, WHITE, BLACK);

  //resolution is 296x128=37888
  //each byte in the image contains 8bit so there are 
  //4736 "bytes" in the image
  //each "column" of 8 pixels wide is 128bytes in length
  //so there are 4736/128 37 columns of pixels
  
  EPD_2IN9_V2_Display_Partial(BlackImage);
  cursorCol+=printLen;
  if(cursorCol > 16){
    cursorCol=0;
    row+=1;
  }

  if(row > 4){
    row=0;
    cursorCol=0;
    Paint_Clear(WHITE);
    
  }
}
