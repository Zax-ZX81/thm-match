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
#define MAGICK_ARGS " -resize '64x64' -background 'gray(50%)' -gravity center -extent 64x64 -depth 6 -compress None -set filename:out '%f_%G' 's/%[filename:out].rgb'"

int main (int argc, char *argv [])

{
struct file_name_return filename_separation;

FILE *IMGFILE;

char img_name [FILENAME_LENGTH] = NULL_STRING;
char cmd_line [FILENAME_LENGTH] = NULL_STRING;
unsigned char nine_byte_chunk [9];
unsigned char *thm_buffer;
unsigned char base_sixfour [65] = BASE_SIXTYFOUR;

int lp = 0;
int wid = 1;
int rerr, f_len, pos;


strcpy (img_name, argv [FILE_ARG - 1]);
rerr = snprintf (cmd_line, FILENAME_LENGTH, "%s%s%s", MAGICK_COMMAND, img_name, MAGICK_ARGS);
printf ("%s\n", cmd_line);
IMGFILE = popen (cmd_line, "r");
}
