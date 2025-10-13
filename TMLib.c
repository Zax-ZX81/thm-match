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


struct rgb_accumulator get_nine_six (unsigned char *nine_byte_string)  // Extract 12 6 bit values from 9 bytes
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

int pos, wid, hig;
int digit = 1;

pos = strlen (thm_filename) - 1;
if (!(pos > 4 && thm_filename [pos] == 'b' && thm_filename [pos -1] == 'g' && thm_filename [pos - 2] == 'r' && thm_filename [pos - 3] == '.'))
	{
	file_return.name [1] = '\0';
	return file_return;
	}
pos = pos - 4;

while (pos && thm_filename[pos] != 'x')
	{
//	printf ("%d\t%c\n", pos, thm_filename[pos]);
	switch (digit)
		{
		case 1:
			wid = thm_filename[pos--] - 48;
			break;
		case 2:
			wid = wid + (thm_filename[pos--] - 48) * 10;
			break;
		case 3:
			wid = wid + (thm_filename[pos--] - 48) * 100;
			break;
		case 4:
			wid = wid + (thm_filename[pos--] - 48) * 1000;
			break;
		case 5:
			wid = wid + (thm_filename[pos--] - 48) * 10000;
			break;
		}
	digit++;
	}
digit = 1;
pos--;
while (pos && thm_filename[pos] != '_')
	{
//	printf ("%d\t%c\n", pos, thm_filename[pos]);
	switch (digit)
		{
		case 1:
			hig = thm_filename[pos--] - 48;
			break;
		case 2:
			hig = hig + (thm_filename[pos--] - 48) * 10;
			break;
		case 3:
			hig = hig + (thm_filename[pos--] - 48) * 100;
			break;
		case 4:
			hig = hig + (thm_filename[pos--] - 48) * 1000;
			break;
		case 5:
			hig = hig + (thm_filename[pos--] - 48) * 10000;
			break;
		}
	digit++;
	}
pos--;
thm_filename [pos + 1] = '\0';
strcpy (file_return.name, thm_filename);
file_return.width = wid;
file_return.height = hig;

//printf ("%s\tW=%d\tH=%d\n", file_return.name, file_return.width, file_return.height);
return file_return;
}
