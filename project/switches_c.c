//write stuff
#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "switches.h"

int redrawScreen = 1;
int secondsW = 0;
char
switch_update_interrupt_sense()
{
  char p2val = P2IN;
  /* update switch interrupt to detect changes from current buttons */
  P2IES |= (p2val & SWITCHES);/* if switch up, sense down */
  P2IES &= (p2val | ~SWITCHES);/* if switch down, sense up */
  return p2val;
}

void
switch_init()/* setup switch */
{
  P2REN |= SWITCHES;/* enables resistors for switches */
  P2IE |= SWITCHES;/* enable interrupts from switches */
  P2OUT |= SWITCHES;/* pull-ups for switches */
  P2DIR &= ~SWITCHES;/* set switches' bits for input */
  switch_update_interrupt_sense();
}

int switches = 0;
int current_pos = 0;
void
switch_interrupt_handler()
{
  char p2val = switch_update_interrupt_sense();
  switches = ~p2val & SWITCHES;
  if (switches & SWITCHES) { /* a switch is depresssed */
    redrawScreen = 1;
    for (char swNum = 0; swNum < 4; swNum++) { /* respond to lowest button pressed */
      int swFlag = 1 << swNum;
      if (switches & swFlag) {
	break;
      }
    }
  }
}

void wdt_c_handler()
{
  static int sec2Count = 0;
  static int sec1Count = 0;
  sec2Count +=1;
  if (sec2Count >= 125) {/* 2/sec */
    sec2Count = 0;
    redrawScreen = 1;
  }
  sec1Count +=1; 
  if (sec1Count>= 250) {/* 1/sec */
    sec1Count = 0;
    secondsW++;
  }
}

/* Switch on S2 */
void
__interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {      /* did a button cause this interrupt? */
    P2IFG &= ~SWITCHES;      /* clear pending sw interrupts */
    switch_interrupt_handler();/* single handler for all switches */
  }
}
 
