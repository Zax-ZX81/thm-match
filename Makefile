# Makefile for thm-match

CC= gcc
CFLAGS= -I. -Wunused-variable

all: smatch sfind sconvert scheck sdup supdate

tbuild:	tbuild.c TMLib.c
	$(CC) -o bin/tbuild tbuild.c TMLib.c $(CFLAGS)

scheck: scheck.c SMLib.c
	$(CC) -o bin/scheck scheck.c SMLib.c $(CFLAGS)

sdup: sdup.c SMLib.c
	$(CC) -o bin/sdup sdup.c SMLib.c $(CFLAGS)

# install for Linux only
install:
	cp -fv bin/tbuild bin/tdup bin/tcheck thm-match.1 ~/bin/
