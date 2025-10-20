# Makefile for thm-match

CC= gcc
CFLAGS= -I. -Wunused-variable

all: bsf_test3 sbp_test2 tprint tfind f_test fns_test1 popen_test1 buf_test

f_test:	f_test.c TMLib.c
	$(CC) -o bin/f_test f_test.c TMLib.c $(CFLAGS)

tprint:		tprint.c TMLib.c
	$(CC) -o bin/tprint tprint.c TMLib.c -lm $(CFLAGS)

tfind:		tfind.c TMLib.c
	$(CC) -o bin/tfind tfind.c TMLib.c -lm $(CFLAGS)

fns_test1:	fns_test1.c TMLib.c
	$(CC) -o bin/fns_test1 fns_test1.c TMLib.c $(CFLAGS)

popen_test1:	popen_test1.c TMLib.c
	$(CC) -o bin/popen_test1 popen_test1.c TMLib.c $(CFLAGS)

buf_test:	buf_test.c TMLib.c
	$(CC) -o bin/buf_test buf_test.c TMLib.c $(CFLAGS)

#tbuild:	tbuild.c TMLib.c
#	$(CC) -o bin/tbuild tbuild.c TMLib.c $(CFLAGS)

bsf_test3:	bsf_test3.c TMLib.c
	$(CC) -o bin/bsf -lm bsf_test3.c TMLib.c $(CFLAGS)

sbp_test2:	sbp_test2.c TMLib.c
	$(CC) -o bin/sbp sbp_test2.c TMLib.c $(CFLAGS)

# install for Linux only
#install:
#	cp -fv bin/tbuild bin/tdup bin/tcheck thm-match.1 ~/bin/
