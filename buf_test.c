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
char nine_byte_string [9];
char *thm_buffer;
char base_sixfour [65] = BASE_SIXTYFOUR;

int lp = 0;
int wid = 1;
int rerr, f_len, pos;


pos = 0;
while (pos < 64)
	{
	putchar (base_sixfour [pos++]);
	}
printf ("\n");

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
thm_buffer = (char *) malloc ((f_len + 1));
printf ("Length=%d\n", f_len);
rerr = fread (thm_buffer, 1, f_len, rgb_thumbnail);
pos = 0;
while (pos < 64)
	{
	putchar (base_sixfour [pos++]);
	}
printf ("\n");

while (lp < 9217)
	{
	for (pos = 0; pos <10 ; pos++)
		{
		nine_byte_string [pos] = thm_buffer [lp++];
		}
	rgb_return = get_nine_six (nine_byte_string);
//printf ("%2d%c %2d%c %2d%c  ", (int) rgb_return.red_val, base_sixfour [(int) rgb_return.red_val], \
//	(int) rgb_return.grn_val, base_sixfour [(int) rgb_return.grn_val], \
//	(int) rgb_return.blu_val, base_sixfour [(int) rgb_return.blu_val]);
printf ("%c%c%c ", base_sixfour [(int) rgb_return.red_val], base_sixfour [(int) rgb_return.grn_val], base_sixfour [(int) rgb_return.blu_val]);
//printf ("%6.3f  %6.3f  %6.3f\n", rgb_return.red_val, rgb_return.grn_val, rgb_return.blu_val);
	if (++wid > 18)
		{
		wid = 1;
		printf ("\n");
pos = 0;
while (pos < 64)
	{
	putchar (base_sixfour [pos++]);
	}
		}
	}	// end olp

fclose (rgb_thumbnail);
printf ("\n%s\n", BASE_SIXTYFOUR);
pos = 0;
while (pos < 64)
	{
	putchar (base_sixfour [pos++]);
	}
printf ("\n");
//printf ("%s ...\n", img_rename);
}

/*

FILE *fptr;
char *msg;
long length;
size_t read_s = 0;
fptr = fopen("example_test.xml", "rb");
fseek(fptr, 0L, SEEK_END);
length = ftell(fptr);
rewind(fptr);
msg = (char*)malloc((length+1));
read_s = fread(msg, 1, length, fptr);
*(mip_msg+ read_s) = 0;
if (fptr) fclose(fptr);


*/
