#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "switches.h"


void update_shape();
int BG_COLOR = COLOR_BLUE;
void main()
{
  P1DIR |= LED;/**< Green led on when CPU on */
  P1OUT |= LED;
  configureClocks();
  lcd_init();
  switch_init();
  enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x8);              /**< GIE (enable interrupts) */
  clearScreen(BG_COLOR);
  while (1) {/* forever */
    if (redrawScreen) {
      redrawScreen = 0;
      update_shape();
    }
    P1OUT &= ~LED;/* led off */
    or_sr(0x10);/**< CPU OFF */
    P1OUT |= LED;/* led on */
  }
}

    
void update_shape()
{
  static unsigned char row = screenHeight / 2, col = screenWidth / 2;
  static char blue = 31, green = 0, red = 31;
  static unsigned char step = 0;
  static char state = 0;
  static char width= 30;
  static int color = COLOR_RED;
  // printf("Screen height is: %d\n",screenHeight);
  // printf("Screen width is: %d\n",screenWidth);
  if (switches & SW4) return;
  if(switches & SW3){
    color = COLOR_GREEN;
  }
  
//four corner appear/dissapear
  switch(state){
  case 0:
    clearScreen(COLOR_BLUE);
    // fillRectangle(0,screenHeight-30,width,width,COLOR_BLUE);
    fillRectangle(0,0,width,width,color);
    state = 1;
    color = COLOR_RED;
    break;
  case 1:
    clearScreen(COLOR_BLUE);
    // fillRectangle(0,0,width,width,COLOR_BLUE);
    fillRectangle(screenWidth-width,0,width,width,color);
    state = 2;
    color = COLOR_RED;
    break;
  case 2:
    clearScreen(COLOR_BLUE);
    // fillRectangle(screenWidth-width,0,width,width,COLOR_BLUE);
    fillRectangle(screenWidth-30,screenHeight-30,width,width,color);
    state = 3;
    color = COLOR_RED;
    break;
  case 3:
    clearScreen(COLOR_BLUE);
    
    // fillRectangle(screenWidth-30,screenHeight-30,width,width,COLOR_BLUE);
    fillRectangle(0,screenHeight-30,width,width,color);
    state = 0;
    color = COLOR_RED;
    break;
  default:
    state = 0;
    break;
    }
      

  
}
