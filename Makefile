# Makefile for thm-match

CC= gcc
CFLAGS= -I. -Wunused-variable

all: bsf_test3 sbp_test2 tprint tfind tcheck f_test fns_test1 popen_test1 buf_test tdiff

tdiff:		tdiff.c TMLib.c
	$(CC) -o bin/tdiff tdiff.c TMLib.c -lm $(CFLAGS)

tprint:		tprint.c TMLib.c thumbprint.c
	$(CC) -o bin/tprint tprint.c TMLib.c thumbprint.c -lm $(CFLAGS)

tfind:		tfind.c TMLib.c thumbprint.c
	$(CC) -o bin/tfind tfind.c TMLib.c thumbprint.c -lm $(CFLAGS)

tcheck:		tcheck.c TMLib.c
	$(CC) -o bin/tcheck tcheck.c TMLib.c -lm $(CFLAGS)

f_test:		f_test.c TMLib.c
	$(CC) -o bin/f_test f_test.c TMLib.c $(CFLAGS)

fns_test1:	fns_test1.c TMLib.c
	$(CC) -o bin/fns_test1 fns_test1.c TMLib.c $(CFLAGS)

popen_test1:	popen_test1.c TMLib.c
	$(CC) -o bin/popen_test1 popen_test1.c TMLib.c $(CFLAGS)

buf_test:	buf_test.c TMLib.c
	$(CC) -o bin/buf_test buf_test.c TMLib.c $(CFLAGS)

bsf_test3:	bsf_test3.c TMLib.c
	$(CC) -o bin/bsf -lm bsf_test3.c TMLib.c $(CFLAGS)

sbp_test2:	sbp_test2.c TMLib.c
	$(CC) -o bin/sbp sbp_test2.c TMLib.c $(CFLAGS)


# install for Linux only
#install:
#	cp -fv bin/tbuild bin/tdup bin/tcheck thm-match.1 ~/bin/
