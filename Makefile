# Makefile for thm-match

CC= gcc
CFLAGS= -I. -Wunused-variable

all: bsf_test3 sbp_test2

#tbuild:	tbuild.c TMLib.c
#	$(CC) -o bin/tbuild tbuild.c TMLib.c $(CFLAGS)

#tbuild:	tbuild.c TMLib.c
#	$(CC) -o bin/tbuild tbuild.c TMLib.c $(CFLAGS)

bsf_test3:	bsf_test3.c TMLib.c
	$(CC) -o bin/bsf bsf_test3.c TMLib.c $(CFLAGS)

sbp_test2:	sbp_test2.c TMLib.c
	$(CC) -o bin/sbp sbp_test2.c TMLib.c $(CFLAGS)

# install for Linux only
#install:
#	cp -fv bin/tbuild bin/tdup bin/tcheck thm-match.1 ~/bin/
