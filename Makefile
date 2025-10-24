# Makefile for thm-match

CC= gcc
CFLAGS= -I. -Wunused-variable

all: tprint tfind tcheck tdiff

tdiff:		tdiff.c TMLib.c
	$(CC) -o bin/tdiff tdiff.c TMLib.c -lm $(CFLAGS)

tprint:		tprint.c TMLib.c
	$(CC) -o bin/tprint tprint.c TMLib.c -lm $(CFLAGS)

tfind:		tfind.c TMLib.c
	$(CC) -o bin/tfind tfind.c TMLib.c -lm $(CFLAGS)

tcheck:		tcheck.c TMLib.c
	$(CC) -o bin/tcheck tcheck.c TMLib.c -lm $(CFLAGS)

# install for Linux only
#install:
#	cp -fv bin/tbuild bin/tdup bin/tcheck thm-match.1 ~/bin/
