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
#include <math.h>
#include <string.h>
//#include <sys/types.h>
#include <sys/stat.h>

#define PROG_VERSION "0.10"

int main (int argc, char *argv [])

{
struct rgb_accumulator rgb_return;
struct maxmin_return limits_return;
struct colgry_accumulator quad_accum [4] = {{0}};
struct file_name_return filename_separation;

char img_name [FILENAME_LENGTH] = NULL_STRING;
char nine_byte_string [9];
char base_sixfour [65] = BASE_SIXTYFOUR;
char hue_print [5] = NULL_STRING;
char gry_print [5] = NULL_STRING;

int qlp, llp, hlp, olp, rerr;

float hue_value, red_dec, grn_dec, blu_dec, mag_n;

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

for (olp = 0; olp < 4; olp += 2)
	{
	for (hlp = 1; hlp < 33; hlp++)
		{
		for (llp = 0; llp < 2; llp++)
			{
			for (qlp = 1; qlp < 9; qlp++)
				{
				rerr = fread (nine_byte_string, 9, 1, rgb_thumbnail);
				rgb_return = get_nine_six (nine_byte_string);
				quad_accum[olp + llp].red_val = quad_accum[olp + llp].red_val + rgb_return.red_val;
				quad_accum[olp + llp].grn_val = quad_accum[olp + llp].grn_val + rgb_return.grn_val;
				quad_accum[olp + llp].blu_val = quad_accum[olp + llp].blu_val + rgb_return.blu_val;
				quad_accum[olp + llp].gry_val = quad_accum[olp + llp].gry_val + (rgb_return.red_val + rgb_return.grn_val + rgb_return.blu_val) / 3;
//printf ("\n%f  %f  %f\n", rgb_return.red_val, rgb_return.grn_val, rgb_return.blu_val);
//printf ("\n%f  %f  %f__%f\t%d\n", quad_accum[olp + llp].red_val, quad_accum[olp + llp].grn_val, quad_accum[olp + llp].blu_val, quad_accum[olp + llp].gry_val, rerr);
				}	// end qlp
			}	// xend llp
		}	// end hlp
	}	// end olp

fclose (rgb_thumbnail);

for (olp = 0; olp < 4; olp++)
	{
	quad_accum[olp].red_val = quad_accum[olp].red_val / QUADRANT_DIVIDER;
	quad_accum[olp].grn_val = quad_accum[olp].grn_val / QUADRANT_DIVIDER;
	quad_accum[olp].blu_val = quad_accum[olp].blu_val / QUADRANT_DIVIDER;
	quad_accum[olp].gry_val = quad_accum[olp].gry_val / QUADRANT_DIVIDER;
	}

for (olp = 0; olp < 4; olp++)
	{
	limits_return = find_limits (quad_accum[olp].red_val, quad_accum[olp].grn_val, quad_accum[olp].blu_val);
	limits_return.max_val = limits_return.max_val / DECIMAL_DIVIDER;
	limits_return.min_val = limits_return.min_val / DECIMAL_DIVIDER;
//	printf ("%s\tQuad: %d\tMax: %5.2f\t", img_name, olp, limits_return.max_val);
//	printf ("Min: %5.2f\t", limits_return.min_val);
//	printf ("MaxChan: %d\t", limits_return.channel);
	red_dec = quad_accum[olp].red_val / DECIMAL_DIVIDER;
	grn_dec = quad_accum[olp].grn_val / DECIMAL_DIVIDER;
	blu_dec = quad_accum[olp].blu_val / DECIMAL_DIVIDER;
//	printf ("R: %f\tG: %f\tB: %f\t", red_dec, grn_dec, blu_dec);
	if (limits_return.channel == RED_CHAN )
		{
		hue_value = (grn_dec - blu_dec) / (limits_return.max_val - limits_return.min_val);
		if (hue_value < 0)
			{
			hue_value = hue_value + 6;
			}
		hue_value = hue_value * SIXBIT_MULTIPLIER;
		}
	if (limits_return.channel == GRN_CHAN )
		{
		hue_value = (2 + ((blu_dec - red_dec) / (limits_return.max_val - limits_return.min_val))) * SIXBIT_MULTIPLIER;
		}
	if (limits_return.channel == BLU_CHAN )
		{
		hue_value = (4 + ((red_dec - grn_dec) / (limits_return.max_val - limits_return.min_val))) * SIXBIT_MULTIPLIER;
		}
//	printf ("\tHue %d: %5.2f\t%c\n", olp, hue_value, base_sixfour [(int) hue_value]);
	hue_print[olp] = base_sixfour [(int) hue_value];
	gry_print[olp] = base_sixfour [(int) quad_accum[olp].gry_val];
	}
//compose_filename (img_name, img_rename, gry_print, hue_print);
//( ( pix^0.408163265306) /28.8 ) - 0.83
//#define EXPONENT 0.408163265306
//#define DIVIDER 28.8
//#define SUBTRACTOR 0.83
mag_n = (powf ((float) (filename_separation.width * filename_separation.height), EXPONENT) / DIVIDER) - SUBTRACTOR;
printf ("%s_%s%s%c%s\n", filename_separation.name, gry_print, hue_print, base_sixfour [(int) mag_n], FILE_EXTN);

//printf ("%s ...\n", img_rename);
//rename (img_name, img_rename);
}
