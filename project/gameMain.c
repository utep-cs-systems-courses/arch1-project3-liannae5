#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "switches.h"
#include "buzzer.h"

void update_shape();
int BG_COLOR = COLOR_BLUE;
void main()
{
  P1DIR |= LED;/**< Green led on when CPU on */
  P1OUT |= LED;
  configureClocks();
  lcd_init();
  buzzer_init();
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
static unsigned char row = screenHeight / 2, col = screenWidth / 2;
static char blue = 31, green = 0, red = 31;
static unsigned char step = 0;
static char state = 0;
static char width= 10;
static int color = COLOR_RED;
    
void update_shape()
{
  /*  static unsigned char row = screenHeight / 2, col = screenWidth / 2;
  static char blue = 31, green = 0, red = 31;
  static unsigned char step = 0;
  static char state = 0;
  static char width= 10;
  static int color = COLOR_RED;*/
  if (switches & SW4) return;
  down_arrow();
  // nf_intro();
  if(switches & SW3){
    square_line();
  }
}

void down_arrow(){
  static char state = 0;
    switch(state){
  case 0:
    // clearScreen(COLOR_BLUE);
    fillRectangle(col-width,screenHeight/3-width,width,width,COLOR_BLUE);
    fillRectangle(0,0,width,width,COLOR_RED);
    state = 1;
    break;
  case 1:
    // clearScreen(COLOR_BLUE);
    fillRectangle(0,0,width,width,COLOR_BLUE);
    fillRectangle(screenWidth-width,0,width,width,COLOR_RED);
    state = 2;
    break;
  case 2:
    // clearScreen(COLOR_BLUE);
    fillRectangle(screenWidth-width,0,width,width,COLOR_BLUE);
    fillRectangle(col-width,screenHeight/3-width,width,width,COLOR_RED);
    state = 0;
    break;
    }
}
 void square_line(){
   
   fillRectangle(0,row,width+(secondsW%screenWidth),width,COLOR_BLUE);
   fillRectangle(secondsW%screenWidth,row,width,width,COLOR_GREEN);
 }
void nf_intro(){
  static char nfi_state = 0;
  static int f_note = 698;
  static int e_note = 659;
  static int d_note = 587;
  static int a_note = 440;
  static int d_flat_note = 554;
  static int b_flat_note = 466;
  switch(nfi_state){
    case 0:
      buzzer_set_period(f_note);
      nfi_state = 1;
      break;
    case 1:
      buzzer_set_period(e_note);
      nfi_state = 2;
      break;
    case 2:
      buzzer_set_period(d_note);
      nfi_state = 3;
      break;
    case 3:
      buzzer_set_period(d_flat_note);
      nfi_state = 4;
      break;
    case 4:
      buzzer_set_period(d_note);
      nfi_state = 5;
      break;
    case 5:
      buzzer_set_period(d_flat_note);
      nfi_state = 6;
      break;
    case 6:
      buzzer_set_period(a_note);
      nfi_state = 7;
      break;
    case 7:
      buzzer_set_period(b_flat_note);
      nfi_state = 0;
      break;
  }
  
}  

