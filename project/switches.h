// WARNING: LCD DISPLAY USES P1.0.  Do not touch!!! 

#define LED BIT6		/* note that bit zero req'd for display */

#define SW1 1
#define SW2 2
#define SW3 4
#define SW4 8

#define SWITCHES 15
extern char switch_update_interrupt_sense();
extern void switch_interrupt_handler();
extern void switch_init();
//extern void wdt_c_handler();
extern int switches;  
extern char p2val;
extern int redrawScreen;
extern int secondsW;
