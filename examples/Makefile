CFLAGS=-Wall -Wextra -Werror -pedantic
CLIBS=-L.  -I. -lm -lncurses
CC=clang

all: PiCalcb DvD Controls

PiCalcb: PiCalc.c
	$(CC) $(CFLAGS) PiCalc.c -o PiCalc $(CLIBS)
DvD: DvD.c
	$(CC) $(CFLAGS) DvD.c -o DvD $(CLIBS)
Controls: Controls.c
	$(CC) $(CFLAGS) Controls.c -o Controls $(CLIBS)

clean:
	rm PiCalc DvD Controls
