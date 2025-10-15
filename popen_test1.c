/* * * * * * * * * * * * * *
 *                         *
 *       popen test        *
 *                         *
 *       2025-10-15        *
 *                         *
 *       Zax               *
 *                         *
 * * * * * * * * * * * * * */

#include <stdio.h>
#include "TMLib.h"
#include <stdlib.h>
//#include <math.h>
#include <string.h>
//#include <sys/types.h>
#include <sys/stat.h>

#define PROG_VERSION "0.10"
#define MAGICK_COMMAND "magick "
#define MAGICK_ARGS " -resize '64x64' -background 'gray(50%)' -gravity center -extent 64x64 -depth 6 -compress None rgb:-"

int main (int argc, char *argv [])

{
struct file_name_return filename_separation;

FILE *IMGFILE;
FILE *FILE_OUT;
char img_name [FILENAME_LENGTH] = NULL_STRING;
char cmd_line [FILENAME_LENGTH] = NULL_STRING;
unsigned char in_byte [2];
unsigned char nine_byte_chunk [9];
unsigned char *thm_buffer;
unsigned char base_sixfour [65] = BASE_SIXTYFOUR;

int wid = 1;
int rerr, f_len, pos;


strcpy (img_name, argv [FILE_ARG - 1]);
rerr = snprintf (cmd_line, FILENAME_LENGTH, "%s%s%s", MAGICK_COMMAND, img_name, MAGICK_ARGS);
printf ("%s\n", cmd_line);
IMGFILE = popen (cmd_line, "r");
FILE_OUT = fopen ("popentest.rgb", "wb");
for (pos = 1; pos < THUMBNAIL_BYTES + 1; pos++)
	{
	rerr = fread (in_byte, 1, 1, IMGFILE);
//	printf ("%d ", in_byte [0]);
	fputc (in_byte [0], FILE_OUT);
	}
fclose (IMGFILE);
fclose (FILE_OUT);
}
