/* * * * * * * * * * * * * *
 *                         *
 *       TMLib.c 0.40      *
 *                         *
 *       2025-10-01        *
 *                         *
 *       Zax               *
 *                         *
 * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TMLib.h"


void error_message (char *message_a, char *message_b)	// Print two strings as error messages in orange

{
fprintf (stderr, "%s%s%s%s\n", TEXT_ORANGE, message_a, message_b, TEXT_RESET);
}


struct file_name_return separate_filename (char *thm_filename)

{
struct file_name_return file_return;

char old_filename [FILENAME_LENGTH] = NULL_STRING;

int pos, wid, hig;
int digit = 1;

strcpy (old_filename, thm_filename);
pos = strlen (old_filename) - 1;
if (!(pos > 4 && old_filename [pos] == 'b' && old_filename [pos -1] == 'g' && old_filename [pos - 2] == 'r' && old_filename [pos - 3] == '.'))
	{
	file_return.name [1] = '\0';
	return file_return;
	}
pos = pos - 4;

while (pos && old_filename[pos] != 'x')
	{
//	printf ("%d\t%c\n", pos, old_filename[pos]);
	switch (digit)
		{
		case 1:
			wid = old_filename[pos--] - 48;
			break;
		case 2:
			wid = wid + (old_filename[pos--] - 48) * 10;
			break;
		case 3:
			wid = wid + (old_filename[pos--] - 48) * 100;
			break;
		case 4:
			wid = wid + (old_filename[pos--] - 48) * 1000;
			break;
		case 5:
			wid = wid + (old_filename[pos--] - 48) * 10000;
			break;
		}
	digit++;
	}
digit = 1;
pos--;
while (pos && old_filename[pos] != '_')
	{
//	printf ("%d\t%c\n", pos, old_filename[pos]);
	switch (digit)
		{
		case 1:
			hig = old_filename[pos--] - 48;
			break;
		case 2:
			hig = hig + (old_filename[pos--] - 48) * 10;
			break;
		case 3:
			hig = hig + (old_filename[pos--] - 48) * 100;
			break;
		case 4:
			hig = hig + (old_filename[pos--] - 48) * 1000;
			break;
		case 5:
			hig = hig + (old_filename[pos--] - 48) * 10000;
			break;
		}
	digit++;
	}
old_filename [pos] = '\0';
//printf ("=%s=\t%d\n", old_filename, pos);
strncpy (file_return.name, old_filename, pos);
file_return.name [pos] = '\0';
file_return.width = wid;
file_return.height = hig;

//printf ("O=%s=\tP=%d=\tN=%s=\tW=%d=\tH=%d=\n", old_filename, pos, file_return.name, file_return.width, file_return.height);
return file_return;
}


char *get_gpx_ext (char *filepath)

{
char *gpx_ext = malloc (FILENAME_LENGTH);
char rev_ext [6] = NULL_STRING;
//char gpx_ext [6] = NULL_STRING;
char fp_string [FILENAME_LENGTH] = NULL_STRING;

int fp_pos;
int rev_pos = 0;
int ext_pos = 0;

strcpy (fp_string, filepath);
fp_pos = strlen (fp_string) - 1;

while (fp_pos && fp_string [fp_pos] != '.' && rev_pos < 6)
	{
//printf ("1 F=%d\tS=%c\n", fp_pos, fp_string [fp_pos]);
	rev_ext [rev_pos] = fp_string [fp_pos];
	fp_pos--;
	rev_pos++;
	}
rev_ext [rev_pos--] = '\0';
while (rev_pos + 1 && fp_pos > 1)
	{
//printf ("2 F=%d\tR=%d\tE%d\n", fp_pos, rev_pos, ext_pos);
	gpx_ext [ext_pos] = rev_ext [rev_pos];
	rev_pos--;
	ext_pos++;
	}
if (ext_pos > 0)
	{
	gpx_ext [ext_pos++] = ' ';		// Put space after extension to prevent false positives
	gpx_ext [ext_pos] = '\0';
	}
/*	else
	{
	gpx_ext = "XXXX";
	}*/
//printf ("3 E=%s\tP=%d\n", gpx_ext, ext_pos);
return (gpx_ext);
}


char *enquote (char *filepath)  // Encapsulate a string in double quotes
{
char *out_string = malloc (FILENAME_LENGTH);
char quote_marks [] = {'\'', '\0'};

strcpy (out_string, quote_marks);
strcat (out_string, filepath);
strcat (out_string, quote_marks);

return out_string;
}


char tpdb_verify (char *fileline)
{
int char_index = 0;
char *char_pos;

do
	{
	char_pos = strchr (BASE_SIXTYFOUR, fileline [char_index]);
	if (char_pos == 0)
		{
		return (0);
		}
	} while (++char_index < 4);
if (fileline [char_index++] != TAB_CHAR)
	{
	return (0);
	}
do
	{
	char_pos = strchr (BASE_SIXTYFOUR, fileline [char_index]);
	if (char_pos == 0)
		{
		return (0);
		}
	} while (++char_index < 9);
if (fileline [char_index++] != TAB_CHAR)
	{
	return (0);
	}
char_pos = strchr (BASE_SIXTYFOUR, fileline [char_index]);
if (char_pos == 0)
	{
	return (0);
	}

return (1);
}

struct rgb_return get_pixel (struct rgb_return *in_img_buff, int anch, int width, int y_max, int x_max)
{
int yl, xl, px, rc = 0, gc = 0, bc = 0;
struct rgb_return pix_return;

for (yl = 0; yl < y_max; yl++)
	{
	for (xl = 0; xl < x_max; xl++)
		{
		px = anch + (yl * width) + xl;
		rc = rc + in_img_buff [px].red_val;
		gc = gc + in_img_buff [px].grn_val;
		bc = bc + in_img_buff [px].blu_val;
		//printf ("%2x%2x%2x ", rp, gp, bp);
//		printf ("Y=%2d   X=%2d   %2x %2x %2x\n", yl, xl, rc, gc, bc);
		}
	}
//printf ("YM=%d, XM=%d, PixCnt=%d\n", y_max, x_max, y_max * x_max);
pix_return.red_val = rc;
pix_return.grn_val = gc;
pix_return.blu_val = bc;

return (pix_return);
}
