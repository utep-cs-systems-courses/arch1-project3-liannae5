#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "switches.h"
#include "buzzer.h"

void update_shape();
void paradiddle1();
void paradiddle2();
void paradiddle3();
void paradiddle4();
void start_screen();
void choose_paradiddle();
void drawChar8x12(u_char rcol, u_char rrow, char c,  u_int fgColorBGR, u_int bgColorBGR);
void drawString8x12(u_char col, u_char row, char *string, u_int fgColorBGR, u_int bgColorBGR);

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
static unsigned int size = screenWidth/3;
//square colors for kick, right, left
static unsigned int kickC = COLOR_RED;
static unsigned int rightC = COLOR_GREEN;
static unsigned int leftC = COLOR_YELLOW;

//buzzer sounds for kick, right, left
static short kickS = 700;
static short rightS = 1000;
static short leftS = 1300;

//holds which switch was last pressed
static unsigned char sw1pressed = 0;
static unsigned char sw2pressed = 0;
static unsigned char sw3pressed = 0;
static unsigned char sw4pressed = 0;
static unsigned char nopressed =  1;
    
void update_shape()
{
 
  if(secondsW >= 10 || nopressed){
    sw1pressed = 0;
    sw2pressed = 0;
    sw3pressed = 0;
    sw4pressed = 0;
    nopressed = 1;
    start_screen();
  }
  if (switches & SW1){
    clearScreen(COLOR_BLUE);
    sw1pressed = 1;
    sw2pressed = 0;
    sw3pressed = 0;
    sw4pressed = 0;
    nopressed = 0;
    secondsW = 0;
    
  }
  if (switches & SW2){
    clearScreen(COLOR_BLUE);
    sw1pressed = 0;
    sw2pressed = 1;
    sw3pressed = 0;
    sw4pressed = 0;
    nopressed = 0;
    secondsW = 0;
    
  }
  if (switches & SW3){
    clearScreen(COLOR_BLUE);
    sw1pressed = 0;
    sw2pressed = 0;
    sw3pressed = 1;
    sw4pressed = 0;
    nopressed = 0;
    secondsW = 0;
    
  }
  if (switches & SW4){
    clearScreen(COLOR_BLUE);
    sw1pressed = 0;
    sw2pressed = 0;
    sw3pressed = 0;
    sw4pressed = 1;
    nopressed = 0;
    secondsW = 0;
  }
  choose_paradiddle();
}

void choose_paradiddle(){
  if(sw1pressed){
    paradiddle1();
  }
  else if(sw2pressed){
    paradiddle2();
  }
  else if(sw3pressed){
    paradiddle3();
  }
  else if(sw4pressed){
    paradiddle4();
  }

}
void paradiddle1(){
  static unsigned char state = 0;
  switch(state){
  case 0://kick
    drawString8x12(screenWidth/2,screenHeight/4,"K",COLOR_BLACK,COLOR_BLUE);
    fillRectangle(0,screenHeight/3,screenWidth,size,COLOR_BLUE);
    fillRectangle(screenWidth/3,screenHeight/3,size,size,kickC);
    buzzer_set_period(kickS);
    state = 1;
    break;
  case 1://right
    fillRectangle(screenWidth/3,screenHeight/3,size,size,COLOR_BLUE);
    fillRectangle((screenWidth/3)*2,screenHeight/3,size,size,rightC);
    buzzer_set_period(rightS);
    state = 2;
    break;
  case 2://left
    fillRectangle((screenWidth/3)*2,screenHeight/3,size,size,COLOR_BLUE);
    fillRectangle(0,screenHeight/3,size,size,leftC);
    buzzer_set_period(leftS);
    state = 0;
    break;
  }
}
void paradiddle2(){
  static int test = 0;
  test++;
  if (test >100){
    test = 0;
  }
}
void paradiddle3(){
  static int test = 0;
  test++;
  if (test >100){
    test = 0;
  }
}

void paradiddle4(){
  static int test = 0;
  test++;
  if (test >100){
    test = 0;
  }
}
void start_screen(){
  turn_buzzer_off();
  clearScreen(COLOR_BLACK);
  // redrawScreen = 0;
}
    
    
void drawChar8x12(u_char rcol, u_char rrow, char c,  u_int fgColorBGR, u_int bgColorBGR) 
{
  u_char col = 0;
  u_char row = 0;
  u_char bit = 0x01;
  u_char oc = c - 0x20;

  lcd_setArea(rcol, rrow, rcol + 7, rrow + 12); /* relative to requested col/row */
  while (row < 13) {
    while (col < 8) {
      u_int colorBGR = (font_8x12[oc][col] & bit) ? fgColorBGR : bgColorBGR;
      lcd_writeColor(colorBGR);
      col++;
    }
    col = 0;
    bit <<= 1;
    row++;
  }
}
void drawString8x12(u_char col, u_char row, char *string, u_int fgColorBGR, u_int bgColorBGR)
{
  u_char cols = col;
  while (*string) {
    drawChar8x12(cols, row, *string++, fgColorBGR, bgColorBGR);
    cols += 9;
  }
}

