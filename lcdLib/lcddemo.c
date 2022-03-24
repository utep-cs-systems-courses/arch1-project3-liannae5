/** \file lcddemo.c
 *  \brief A simple demo that draws a string and square
 */

#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

/** Initializes everything, clears the screen, draws "hello" and a square */
int
main()
{
  configureClocks();
  lcd_init();
  u_char width = screenWidth, height = screenHeight;

  clearScreen(COLOR_BLUE);

  drawString5x7(20,20, "hello", COLOR_GREEN, COLOR_RED);

  fillRectangle(30,30, 60, 60, COLOR_ORANGE);
  /* ~~~~Square~~~~
 int col =0, row =0;
  int fcol = 50, frow =50;
  for(int j = 0; j < 10;j++){
    // drawPixel(fcol+j,frow+j,COLOR_WHITE);
    // drawPixel((fcol+10)-j,frow+j,COLOR_WHITE);
    drawPixel(fcol+j,frow,COLOR_BLUE); //top
    drawPixel(fcol+j,frow+10,COLOR_BLUE);//bottom
    drawPixel(fcol,frow+j,COLOR_BLUE); //left
    // drawPixel(fcol+10,frow+j,COLOR_BLUE);//right
    
    
    }*/
  int a, length = 20;
  int fCol =50, fRow = 50;
  /*~~~~Right Triangle~~~~
 for(a = 0;a<length;a++){
    drawPixel(fCol+a,fRow+a,COLOR_BLUE);
    drawPixel(fCol+a,fRow,COLOR_PURPLE);
    drawPixel((fCol+length),fRow+a,COLOR_BLACK);

    if(a%2 == 1){
      for(int v = 0;v<a;v++){
	drawPixel(fCol+a,fRow+v,COLOR_GREEN);
	drawPixel((fCol+length)-v,(fRow+length)-a,COLOR_BLUE);
      }
    }
  
    }*/
  int s, sCol = 50, sRow =50;
  /* ~~~~Asterisk~~~~
  for(s=0;s<length;s++){
    drawPixel(sCol+s,sRow,COLOR_BLACK);//horizontal
    drawPixel(sCol+(length/2),(sRow-(length/2))+s,COLOR_BLACK);//vertical
    drawPixel(sCol+s,(sRow-(length/2))+s,COLOR_BLACK);//left diagonal
    drawPixel((sCol+length)-s,(sRow-(length/2))+s,COLOR_BLACK);//right diagonal
   
    }*/
  /*~~~~Hour Glass~~~~
  for(s=0;s<length;s++){
    drawPixel(sCol+s,sRow,COLOR_BLUE);//top horizontal
    drawPixel(sCol+s,sRow+length,COLOR_BLUE);//bottom horizontal
    if(s<=10){
      drawPixel(sCol+s,sRow+s,COLOR_BLUE);//top left diag
      drawPixel((sCol+length)-s,sRow+s,COLOR_BLUE);//top right diag
      drawPixel(sCol+s,(sRow+length)-s,COLOR_BLUE);//bot left diag
      drawPixel((sCol+length)-s,(sRow+length)-s,COLOR_BLUE);//bot right diag
    }
    }*/
}
