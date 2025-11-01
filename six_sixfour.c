/* * * * * * * * * * * * * * *
 *                           *
 *    six_sixfour.c 0.35     *
 *                           *
 *    2025-10-01             *
 *                           *
 *    Zax                    *
 *                           *
 * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TMLib.h"


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


int sixfour_to_dec (char sixfour)  // Convert base sixty-four to decimal
{
int dec_ret;

//printf ("S=%c\tA=%d\n", sixfour, sixfour);
if ((int) sixfour > 96)
	{
	dec_ret = (int) sixfour - 60;
	}
	else
	{
	if ((int) sixfour > 64)
		{
		dec_ret = (int) sixfour - 55;
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
