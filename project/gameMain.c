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
extern unsigned short rand = 0;
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
//square size
static unsigned int size = screenWidth/3;

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
  //if it's been at least 10 seconds without any interaction go to start screen
  if(secondsW >= 10 || nopressed){
    sw1pressed = 0;
    sw2pressed = 0;
    sw3pressed = 0;
    sw4pressed = 0;
    nopressed = 1;
    start_screen();
  }
  //when switch 1 is pressed
  if (switches & SW1){
    clearScreen(COLOR_BLUE);
    sw1pressed = 1;
    sw2pressed = 0;
    sw3pressed = 0;
    sw4pressed = 0;
    nopressed = 0;
    secondsW = 0;
    
  }
  //when switch 2 is pressed
  if (switches & SW2){
    clearScreen(COLOR_BLUE);
    sw1pressed = 0;
    sw2pressed = 1;
    sw3pressed = 0;
    sw4pressed = 0;
    nopressed = 0;
    secondsW = 0;
    
  }
  //when switch 3 is pressed
  if (switches & SW3){
    clearScreen(COLOR_BLUE);
    sw1pressed = 0;
    sw2pressed = 0;
    sw3pressed = 1;
    sw4pressed = 0;
    nopressed = 0;
    secondsW = 0;
    
  }
  //when switch 4 is pressed
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
//choose paradiddle state machine that corresponds to the switch pressed
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
//paradiddle: KRL
void paradiddle1(){
  static unsigned char state = 0;
  switch(state){
  case 0://kick
    drawString5x7(screenWidth/2,screenHeight/4,"K",COLOR_BLACK,COLOR_BLUE);//display "k" for kick
    fillRectangle(0,screenHeight/3,screenWidth,size,COLOR_BLUE);//clear previous square
    fillRectangle(screenWidth/3,screenHeight/3,size,size,color_picker(secondsW));//display square in middle to represent kick
    buzzer_set_period(kickS); //play kick buzzer frequency
    rand++;//add 1 to random
    state = 1;
    break;
  case 1://right
    drawString5x7(screenWidth/2,screenHeight/4,"R",COLOR_BLACK,COLOR_BLUE);//"r" for right
    fillRectangle(screenWidth/3,screenHeight/3,size,size,COLOR_BLUE);//clear previous square
    fillRectangle((screenWidth/3)*2,screenHeight/3,size,size,color_picker(secondsW));//display square on right side of screen
    buzzer_set_period(rightS);//play right buzzer frequency
    state = 2;
    break;
  case 2://left
    drawString5x7(screenWidth/2,screenHeight/4,"L",COLOR_BLACK,COLOR_BLUE);//"L" for left
    fillRectangle((screenWidth/3)*2,screenHeight/3,size,size,COLOR_BLUE);//clear previous square
    fillRectangle(0,screenHeight/3,size,size,color_picker(secondsW));//display square on left side of screen
    buzzer_set_period(leftS);// play left buzzer frequency
    state = 0;
    break;
  }
}
//paradiddle:RLRR
void paradiddle2(){
  static unsigned char state = 0;
  switch(state){
  case 0://right
    drawString5x7(screenWidth/2,screenHeight/4,"R",COLOR_BLACK,COLOR_BLUE);
    fillRectangle(0,screenHeight/3,size,size,COLOR_BLUE);
    fillRectangle((screenWidth/3)*2,screenHeight/3,size,size,color_picker(secondsW));
    buzzer_set_period(rightS);
    state = 1;
    break;
  case 1://quiet
    drawString5x7(screenWidth/2,screenHeight/4," ",COLOR_BLUE,COLOR_BLUE);//clear string
    fillRectangle(0,screenHeight/3,size,size,COLOR_BLUE);//clear screen
    turn_buzzer_off();//shut off buzzer
    state = 2;
    break;
  case 2://left
    drawString5x7(screenWidth/2,screenHeight/4,"L",COLOR_BLACK,COLOR_BLUE);
    fillRectangle((screenWidth/3)*2,screenHeight/3,size,size,color_picker(secondsW));
    fillRectangle(0,screenHeight/3,size,size,leftC);
    buzzer_set_period(leftS);
    state = 3;
    rand +=2;
    break;
  case 3://quiet
    drawString5x7(screenWidth/2,screenHeight/4," ",COLOR_BLUE,COLOR_BLUE);
    fillRectangle(0,screenHeight/3,size,size,COLOR_BLUE);
    turn_buzzer_off();
    state = 4;
    break;
  case 4://right
    drawString5x7(screenWidth/2,screenHeight/4,"R",COLOR_BLACK,COLOR_BLUE);
    fillRectangle(0,screenHeight/3,size,size,COLOR_BLUE);
    fillRectangle((screenWidth/3)*2,screenHeight/3,size,size,color_picker(secondsW));
    buzzer_set_period(rightS);
    state = 5;
    break;
  case 5://quiet
    drawString5x7(screenWidth/2,screenHeight/4," ",COLOR_BLUE,COLOR_BLUE);
    fillRectangle(0,screenHeight/3,size,size,COLOR_BLUE);
    turn_buzzer_off();
    rand ^=49;
    state = 6;
    break;
  case 6://right
    drawString5x7(screenWidth/2,screenHeight/4,"R",COLOR_BLACK,COLOR_BLUE);
    fillRectangle(0,screenHeight/3,size,size,COLOR_BLUE);
    fillRectangle((screenWidth/3)*2,screenHeight/3,size,size,color_picker(secondsW));
    buzzer_set_period(rightS);
    state = 7;
    break;
  case 7://quiet
    drawString5x7(screenWidth/2,screenHeight/4," ",COLOR_BLUE,COLOR_BLUE);
    fillRectangle(0,screenHeight/3,size,size,COLOR_BLUE);
    turn_buzzer_off();
    state = 0;
    break;
  
  }
}
//single snare roll aka "RLRL" paradiddle
void paradiddle3(){
  static unsigned char state = 0;
  switch(state){
  case 0://right
    drawString5x7(screenWidth/2,screenHeight/4,"R",COLOR_BLACK,COLOR_BLUE);
    fillRectangle(0,screenHeight/3,size,size,COLOR_BLUE);
    fillRectangle((screenWidth/3)*2,screenHeight/3,size,size,color_picker(secondsW));
    buzzer_set_period(rightS);
    state = 1;
    break;
  case 1://left
    drawString5x7(screenWidth/2,screenHeight/4,"L",COLOR_BLACK,COLOR_BLUE);
    fillRectangle((screenWidth/3)*2,screenHeight/3,size,size,COLOR_BLUE);
    fillRectangle(0,screenHeight/3,size,size,color_picker(secondsW));
    buzzer_set_period(leftS);
    rand +=7;
    state = 0;
    break;
  }
}

void paradiddle4(){
  static unsigned char state = 0;
  switch(state){
  case 0://left
    drawString5x7(screenWidth/2,screenHeight/4,"L",COLOR_BLACK,COLOR_BLUE);
    fillRectangle(0,screenHeight/3,size,size,COLOR_BLUE);
    fillRectangle(0,screenHeight/3,size,size,color_picker(secondsW));
    buzzer_set_period(leftS);
    state = 1;
    break;
  case 1://kick
    drawString5x7(screenWidth/2,screenHeight/4,"K",COLOR_BLACK,COLOR_BLUE);
    fillRectangle(0,screenHeight/3,screenWidth,size,COLOR_BLUE);
    fillRectangle(screenWidth/3,screenHeight/3,size,size,color_picker(secondsW));
    buzzer_set_period(kickS);
    state = 2;
    rand +=12;
    break;
  case 2://right
    drawString5x7(screenWidth/2,screenHeight/4,"R",COLOR_BLACK,COLOR_BLUE);
    fillRectangle(screenWidth/3,screenHeight/3,size,size,COLOR_BLUE);
    fillRectangle((screenWidth/3)*2,screenHeight/3,size,size,color_picker(secondsW));
    buzzer_set_period(rightS);
    state = 3;
    break;
  case 3://left
    drawString5x7(screenWidth/2,screenHeight/4,"L",COLOR_BLACK,COLOR_BLUE);
    fillRectangle((screenWidth/3)*2,screenHeight/3,size,size,COLOR_BLUE);
    fillRectangle(0,screenHeight/3,size,size,color_picker(secondsW));
    buzzer_set_period(leftS);
    state = 4;
    break;
  case 4://quiet
    drawString5x7(screenWidth/2,screenHeight/4," ",COLOR_BLUE,COLOR_BLUE);
    fillRectangle(0,screenHeight/3,size,size,COLOR_BLUE);
    turn_buzzer_off();
    state = 0;
    break;
  }
}
//displays start screen
void start_screen(){
  turn_buzzer_off();//turn buzzer off
  rand = 0;//reset rand
  clearScreen(COLOR_BLACK);//set screen to black
  drawString5x7(0,screenHeight/2,"Choose a paradiddle",COLOR_PINK,COLOR_BLACK);//display instruction
}

//final step in randomizing rand
extern short random(){
  rand ^=67;//xor it
  rand +=COLOR_PINK;//add pink int to it
  return rand;//return
}
  
  
