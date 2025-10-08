/* * * * * * * * * * * * * *
 *                         *
 *       tprint 0.10       *
 *                         *
 *       2025-10-08        *
 *                         *
 *       Zax               *
 *                         *
 * * * * * * * * * * * * * */

#include <stdio.h>
#include "TMLib.h"
//#include <stdlib.h>
#include <string.h>
//#include <sys/types.h>
#include <sys/stat.h>

#define PROG_VERSION "0.10"

int main (int argc, char *argv [])

{
struct rgb_accumulator rgb_return;
struct maxmin_return limits_return;
struct colgry_accumulator quad_accum [4] = {{0}};

char img_name [256] = "";
char img_rename [256] = "";
char nine_byte_string [9];
char sixtyfour_bit [65] = SIXTYFOUR_BIT;
char hue_print [5] = "";
char gry_print [5] = "";

int qlp, llp, hlp, olp;

float hue_value, red_dec, grn_dec, blu_dec;

strcpy (img_name, argv [FILE_ARG - 1]);
FILE* rgb_thumbnail = fopen(img_name, "rb");

if (rgb_thumbnail == NULL)
	{
	perror("Error opening file");
	return 1;
	}

for (olp = 0; olp < 4; olp += 2)
	{
	for (hlp = 1; hlp < 33; hlp++)
		{
		for (llp = 0; llp < 2; llp++)
			{
			for (qlp = 1; qlp < 9; qlp++)
				{
				fread (nine_byte_string, 9, 1, rgb_thumbnail);
				rgb_return = get_nine_six (nine_byte_string);
				quad_accum[olp + llp].red_val = quad_accum[olp + llp].red_val + rgb_return.red_val;
				quad_accum[olp + llp].grn_val = quad_accum[olp + llp].grn_val + rgb_return.grn_val;
				quad_accum[olp + llp].blu_val = quad_accum[olp + llp].blu_val + rgb_return.blu_val;
				quad_accum[olp + llp].gry_val = quad_accum[olp + llp].gry_val + (rgb_return.red_val + rgb_return.grn_val + rgb_return.blu_val) / 3;
				}	// end qlp
			}	// xend llp
		}	// end hlp
	}	// end olp

fclose (rgb_thumbnail);

for (olp = 0; olp < 4; olp++)
	{
	quad_accum[olp].red_val = quad_accum[olp].red_val / 256;
	quad_accum[olp].grn_val = quad_accum[olp].grn_val / 256;
	quad_accum[olp].blu_val = quad_accum[olp].blu_val / 256;
	quad_accum[olp].gry_val = quad_accum[olp].gry_val / 256;
	}

for (olp = 0; olp < 4; olp++)
	{
	limits_return = find_limits (quad_accum[olp].red_val, quad_accum[olp].grn_val, quad_accum[olp].blu_val);
	limits_return.max_val = limits_return.max_val / 64;
	limits_return.min_val = limits_return.min_val / 64;
//	printf ("%s\tQuadrant %d\tMax: %5.2f\t", argv [FILE_ARG - 1], olp, limits_return.max_val);
//	printf ("Min: %5.2f\t", limits_return.min_val);
//	printf ("MaxChan: %d\t", limits_return.channel);
	red_dec = quad_accum[olp].red_val / 64;
	grn_dec = quad_accum[olp].grn_val / 64;
	blu_dec = quad_accum[olp].blu_val / 64;
//	printf ("R: %f\tG: %f\tB: %f\t", red_dec, grn_dec, blu_dec);
	if (limits_return.channel == RED_CHAN )
		{
		hue_value = (grn_dec - blu_dec) / (limits_return.max_val - limits_return.min_val);
		if (hue_value < 0)
			{
			hue_value = hue_value + 6;
			}
		hue_value = hue_value * 10.5;
		}
	if (limits_return.channel == GRN_CHAN )
		{
		hue_value = (2 + ((blu_dec - red_dec) / (limits_return.max_val - limits_return.min_val))) * 10.5;
		}
	if (limits_return.channel == BLU_CHAN )
		{
		hue_value = (4 + ((red_dec - grn_dec) / (limits_return.max_val - limits_return.min_val))) * 10.5;
		}
//	printf ("\tHue %d: %5.2f\t%c\n", olp, hue_value, sixtyfour_bit [(int) hue_value]);
	hue_print[olp] = sixtyfour_bit [(int) hue_value];
	gry_print[olp] = sixtyfour_bit [(int) quad_accum[olp].gry_val];
	}
compose_filename (img_name, img_rename, gry_print, hue_print);

printf ("%s\n", img_rename);
rename (img_name, img_rename);
}
