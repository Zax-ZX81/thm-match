/* * * * * * * * * * * * * *
 *                         *
 *       TMLib.c 0.10      *
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


void exit_error (char *message_a, char *message_b)	// Print two strings as error messages in orange
{
fprintf (stderr, "%s%s%s%s\n", TEXT_ORANGE, message_a, message_b, TEXT_RESET);
exit (1);
}

void error_mess (char *message_a, char *message_b)	// Print two strings as error messages in orange
{
fprintf (stderr, "%s%s%s%s\n", TEXT_ORANGE, message_a, message_b, TEXT_RESET);
}


struct rgb_accumulator get_nine_six (unsigned char *nine_byte_string)  // Extract 3 6 bit values from 9 bytes
{
struct four_six_bit_pixels fsbp;
struct rgb_accumulator rgb_return;

unsigned char tmpa, tmpb;

tmpa = nine_byte_string [0];
fsbp.red_a = (tmpa >> 2);

tmpa = nine_byte_string [0];
tmpb = (tmpa << 6);
fsbp.grn_a = ((tmpb >> 2) & ~15) | 0;
tmpa = nine_byte_string [1];
tmpb = (tmpa >> 4);
fsbp.grn_a = fsbp.grn_a + tmpb;

tmpa = nine_byte_string [1];
tmpb = (tmpa << 4);
fsbp.blu_a = (tmpb >> 2);
tmpa = nine_byte_string [2];
tmpb = (tmpa >> 6);
fsbp.blu_a = fsbp.blu_a + tmpb;

tmpa = nine_byte_string [2];
tmpb = (tmpa << 2);
fsbp.red_b = (tmpb >> 2);

tmpa = nine_byte_string [3];
fsbp.grn_b = (tmpa >> 2);

tmpa = nine_byte_string [3];
tmpb = (tmpa << 6);
fsbp.blu_b = ((tmpb >> 2) & ~15) | 0;
tmpa = nine_byte_string [4];
tmpb = (tmpa >> 4);
fsbp.blu_b = fsbp.blu_b + tmpb;

tmpa = nine_byte_string [4];
tmpb = (tmpa << 4);
fsbp.red_c = (tmpb >> 2);
tmpa = nine_byte_string [5];
tmpb = (tmpa >> 6);
fsbp.red_c = fsbp.red_c + tmpb;

tmpa = nine_byte_string [5];
tmpb = (tmpa << 2);
fsbp.grn_c = (tmpb >> 2);

tmpa = nine_byte_string [6];
fsbp.blu_c = (tmpa >> 2);

tmpa = nine_byte_string [6];
tmpb = (tmpa << 6);
fsbp.red_d = ((tmpb >> 2) & ~15) | 0;
tmpa = nine_byte_string [7];
tmpb = (tmpa >> 4);
fsbp.red_d = fsbp.red_d + tmpb;

tmpa = nine_byte_string [7];
tmpb = (tmpa << 4);
fsbp.grn_d = (tmpb >> 2);
tmpa = nine_byte_string [8];
tmpb = (tmpa >> 6);
fsbp.grn_d = fsbp.grn_d + tmpb;

tmpa = nine_byte_string [8];
tmpb = (tmpa << 2);
fsbp.blu_d = (tmpb >> 2);

//printf ("\tr%2d g%2d b%2d\tr%2d g%2d b%2d\tr%2d g%2d b%2d\tr%2d g%2d b%2d\n", fsbp.red_a, fsbp.grn_a, fsbp.blu_a, fsbp.red_b, fsbp.grn_b, fsbp.blu_b, fsbp.red_c, fsbp.grn_c, fsbp.blu_c, fsbp.red_d, fsbp.grn_d, fsbp.blu_d);

rgb_return.red_val = (float) (fsbp.red_a + fsbp.red_b + fsbp.red_c + fsbp.red_d) / 4;
rgb_return.grn_val = (float) (fsbp.grn_a + fsbp.grn_b + fsbp.grn_c + fsbp.grn_d) / 4;
rgb_return.blu_val = (float) (fsbp.blu_a + fsbp.blu_b + fsbp.blu_c + fsbp.blu_d) / 4;

return rgb_return;
}


char *twelve_six_bit (unsigned char *nine_byte_string)  // Extract 12 6 bit values from 9 bytes
{
struct four_six_bit_pixels fsbp;

char *rgb_return = malloc (13);
unsigned char base_sixfour [65] = BASE_SIXTYFOUR;
unsigned char tmpa, tmpb;

tmpa = nine_byte_string [0];
fsbp.red_a = (tmpa >> 2);

tmpa = nine_byte_string [0];
tmpb = (tmpa << 6);
fsbp.grn_a = ((tmpb >> 2) & ~15) | 0;
tmpa = nine_byte_string [1];
tmpb = (tmpa >> 4);
fsbp.grn_a = fsbp.grn_a + tmpb;

tmpa = nine_byte_string [1];
tmpb = (tmpa << 4);
fsbp.blu_a = (tmpb >> 2);
tmpa = nine_byte_string [2];
tmpb = (tmpa >> 6);
fsbp.blu_a = fsbp.blu_a + tmpb;

tmpa = nine_byte_string [2];
tmpb = (tmpa << 2);
fsbp.red_b = (tmpb >> 2);

tmpa = nine_byte_string [3];
fsbp.grn_b = (tmpa >> 2);

tmpa = nine_byte_string [3];
tmpb = (tmpa << 6);
fsbp.blu_b = ((tmpb >> 2) & ~15) | 0;
tmpa = nine_byte_string [4];
tmpb = (tmpa >> 4);
fsbp.blu_b = fsbp.blu_b + tmpb;

tmpa = nine_byte_string [4];
tmpb = (tmpa << 4);
fsbp.red_c = (tmpb >> 2);
tmpa = nine_byte_string [5];
tmpb = (tmpa >> 6);
fsbp.red_c = fsbp.red_c + tmpb;

tmpa = nine_byte_string [5];
tmpb = (tmpa << 2);
fsbp.grn_c = (tmpb >> 2);

tmpa = nine_byte_string [6];
fsbp.blu_c = (tmpa >> 2);

tmpa = nine_byte_string [6];
tmpb = (tmpa << 6);
fsbp.red_d = ((tmpb >> 2) & ~15) | 0;
tmpa = nine_byte_string [7];
tmpb = (tmpa >> 4);
fsbp.red_d = fsbp.red_d + tmpb;

tmpa = nine_byte_string [7];
tmpb = (tmpa << 4);
fsbp.grn_d = (tmpb >> 2);
tmpa = nine_byte_string [8];
tmpb = (tmpa >> 6);
fsbp.grn_d = fsbp.grn_d + tmpb;

tmpa = nine_byte_string [8];
tmpb = (tmpa << 2);
fsbp.blu_d = (tmpb >> 2);

//printf ("\tr%2d g%2d b%2d\tr%2d g%2d b%2d\tr%2d g%2d b%2d\tr%2d g%2d b%2d\n", fsbp.red_a, fsbp.grn_a, fsbp.blu_a, fsbp.red_b, fsbp.grn_b, fsbp.blu_b, fsbp.red_c, fsbp.grn_c, fsbp.blu_c, fsbp.red_d, fsbp.grn_d, fsbp.blu_d);

rgb_return [0] = base_sixfour [(int) fsbp.red_a];
rgb_return [1] = base_sixfour [(int) fsbp.grn_a];
rgb_return [2] = base_sixfour [(int) fsbp.blu_a];
rgb_return [3] = base_sixfour [(int) fsbp.red_b];
rgb_return [4] = base_sixfour [(int) fsbp.grn_b];
rgb_return [5] = base_sixfour [(int) fsbp.blu_b];
rgb_return [6] = base_sixfour [(int) fsbp.red_c];
rgb_return [7] = base_sixfour [(int) fsbp.grn_c];
rgb_return [8] = base_sixfour [(int) fsbp.blu_c];
rgb_return [9] = base_sixfour [(int) fsbp.red_d];
rgb_return [10] = base_sixfour [(int) fsbp.grn_d];
rgb_return [11] = base_sixfour [(int) fsbp.blu_d];
rgb_return [12] = '\0';
//printf ("%s", rgb_return);
return rgb_return;
}


struct maxmin_return find_limits (float red_value, float grn_value, float blu_value)

{
struct maxmin_return ans;

if ((red_value >= grn_value) && (red_value >= blu_value))
	{
	ans.channel = RED_CHAN;
	ans.max_val = red_value;
	}
	else
	{
	if ((grn_value >= red_value) && (grn_value >= blu_value))
		{
		ans.channel = GRN_CHAN;
		ans.max_val = grn_value;
		}
		else
		{
		ans.channel = BLU_CHAN;
		ans.max_val = blu_value;
		}
	}
if ((red_value <= grn_value) && (red_value <= blu_value))
	{
	ans.min_val = red_value;
	}
if ((grn_value <= red_value) && (grn_value <= blu_value))
	{
	ans.min_val = grn_value;
	}
if ((blu_value <= red_value) && (blu_value <= grn_value))
	{
	ans.min_val = blu_value;
	}
return ans;
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


struct dimension_return separate_magnitude (char *mag_string)

{
struct dimension_return mag_return;

char mag_sum [FILENAME_LENGTH] = NULL_STRING;

int pos, wid, hig;
int digit = 1;

strcpy (mag_sum, mag_string);
pos = strlen (mag_sum) - 1;
//printf ("%s\t%d\t=%c=\n", mag_sum, pos, mag_sum [pos]);

while (pos && mag_sum[pos] != 'x')
	{
//	printf ("%d\t%c\n", pos, mag_sum[pos]);
	switch (digit)
		{
		case 1:
			wid = mag_sum[pos--] - 48;
			break;
		case 2:
			wid = wid + (mag_sum[pos--] - 48) * 10;
			break;
		case 3:
			wid = wid + (mag_sum[pos--] - 48) * 100;
			break;
		case 4:
			wid = wid + (mag_sum[pos--] - 48) * 1000;
			break;
		case 5:
			wid = wid + (mag_sum[pos--] - 48) * 10000;
			break;
		}
	digit++;
	}
digit = 1;
pos--;
while (pos >= 0)
	{
//	printf ("%d\t%c\n", pos, mag_sum[pos]);
	switch (digit)
		{
		case 1:
			hig = mag_sum[pos--] - 48;
			break;
		case 2:
			hig = hig + (mag_sum[pos--] - 48) * 10;
			break;
		case 3:
			hig = hig + (mag_sum[pos--] - 48) * 100;
			break;
		case 4:
			hig = hig + (mag_sum[pos--] - 48) * 1000;
			break;
		case 5:
			hig = hig + (mag_sum[pos--] - 48) * 10000;
			break;
		}
	digit++;
	}
//printf ("=%s=\t%d\n", mag_sum, pos);
mag_return.width = wid;
mag_return.height = hig;

return mag_return;


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
	gpx_ext [ext_pos++] = ' ';
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
char quote_marks [] = {'"', '\0'};

strcpy (out_string, quote_marks);
strcat (out_string, filepath);
strcat (out_string, quote_marks);

return out_string;
}

int sixfour_to_dec (char sixfour)  // Convert base sixty-four to decimal
{
int dec_ret;

//printf ("S=%c\tA=%d\t", sixfour, sixfour);
if ((int) sixfour > 96)
	{
	dec_ret = (int) sixfour - 60;
	}
	else
	{
	if ((int) sixfour > 64)
		{
		dec_ret = (int) sixfour - 54;
		}
		else
		{
		if ((int) sixfour > 47)
			{
			dec_ret = (int) sixfour - 48;
			}
			else
			{
			if ((int) sixfour == '-')
				{
				dec_ret = 62;
				}
				else
				{
				if ((int) sixfour == '+')
					{
					dec_ret = 63;
					}
				}
			}
		}
	}
//printf ("D=%3d\n", dec_ret);

return dec_ret;
}
