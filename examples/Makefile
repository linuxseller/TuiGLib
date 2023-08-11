CFLAGS=-Wall -Wextra -Werror -pedantic
CLIBS=-L. -L.. -I. -I.. -lm -lncurses
CC=clang

all: PiCalcb DvDb
PiCalcb: PiCalc.c
	$(CC) $(CFLAGS) PiCalc.c -o PiCalc $(CLIBS)
DvDb: DvD.c
	$(CC) $(CFLAGS) DvD.c -o DvD $(CLIBS)

clean:
	rm PiCalc DvD
