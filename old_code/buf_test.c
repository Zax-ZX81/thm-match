/* * * * * * * * * * * * * *
 *                         *
 *       buffer test       *
 *                         *
 *       2025-10-13        *
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

int main (int argc, char *argv [])

{
struct rgb_accumulator rgb_return;
struct file_name_return filename_separation;

char img_name [FILENAME_LENGTH] = NULL_STRING;
unsigned char nine_byte_chunk [9];
unsigned char *thm_buffer;
unsigned char base_sixfour [65] = BASE_SIXTYFOUR;

int lp = 0;
int wid = 1;
int rerr, f_len, pos;


strcpy (img_name, argv [FILE_ARG - 1]);
FILE* rgb_thumbnail = fopen(img_name, "rb");
if (rgb_thumbnail == NULL)
	{
	perror("Error opening file");
	return 1;
	}
filename_separation = separate_filename (img_name);
if (filename_separation.name == "")
	{
	perror("Invalid file");
	return 1;
	}

fseek (rgb_thumbnail, 0L, SEEK_END);
f_len = ftell (rgb_thumbnail);
rewind (rgb_thumbnail);
thm_buffer = (unsigned char *) calloc (1, f_len + 1);
rerr = fread (thm_buffer, 1, f_len, rgb_thumbnail);

while (lp < 9216)
	{
	for (pos = 0; pos <10 ; pos++)
		{
		nine_byte_chunk [pos] = thm_buffer [lp];
		lp++;
		}
	lp = lp - 1;
//	base_sixfour [0] = '0';
	rgb_return = get_nine_six (nine_byte_chunk);
//printf ("=%2d=%c =%2d=%c =%2d=%c  ", (int) rgb_return.red_val, base_sixfour [(int) rgb_return.red_val], \
//	(int) rgb_return.grn_val, base_sixfour [(int) rgb_return.grn_val], \
//	(int) rgb_return.blu_val, base_sixfour [(int) rgb_return.blu_val]);
	base_sixfour [0] = '0';
	putchar (base_sixfour [(int) rgb_return.red_val]);
	base_sixfour [0] = '0';
	putchar (base_sixfour [(int) rgb_return.grn_val]);
	base_sixfour [0] = '0';
	putchar (base_sixfour [(int) rgb_return.blu_val]);
	base_sixfour [0] = '0';
	putchar (' ');
	if (wid++ > 15)
		{
		wid = 1;
		putchar ('\n');
		}
	}	// end olp

fclose (rgb_thumbnail);
//printf ("\n%d-%d =%c=\n", 0, base_sixfour [0], base_sixfour [0]);
//printf ("%s ...\n", img_rename);
}
