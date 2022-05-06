# makfile configuration
CPU             	= msp430g2553
CFLAGS          	= -mmcu=${CPU} -Os -I../h
LDFLAGS		= -L../lib -L/opt/ti/msp430_gcc/include/ 

#switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-gcc -mmcu=${CPU} -c

all:proj3.elf

#additional rules for files
proj3.elf: ${COMMON_OBJECTS} wdt_handler.o switches_c.o buzzer.o gameMain.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer -lLcd 

load: proj3.elf
	msp430loader.sh $^

clean:
	rm -f *.o *.elf
