/* * * * * * * * * * * * * *
 *                         *
 *       tprint 0.20       *
 *                         *
 *       2025-10-08        *
 *                         *
 *       Zax               *
 *                         *
 * * * * * * * * * * * * * */

#include <stdio.h>
#include "TMLib.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
//#include <sys/types.h>
#include <sys/stat.h>

#define PROG_VERSION "0.20"

int main (int argc, char *argv [])

{
struct rgb_accumulator rgb_return;
struct maxmin_return limits_return;
struct colgry_accumulator quad_accum [4] = {{0}};
struct file_name_return filename_separation;

char img_name [FILENAME_LENGTH] = NULL_STRING;
char new_name [FILENAME_LENGTH] = NULL_STRING;
unsigned char nine_byte_chunk [9];
unsigned char *thm_buffer;
unsigned char base_sixfour [65] = BASE_SIXTYFOUR;
char hue_print [5] = NULL_STRING;
char gry_print [5] = NULL_STRING;

int lp = 0;
int qlp, llp, hlp, olp, rerr, f_len, pos, nn_len;

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
fseek (rgb_thumbnail, 0L, SEEK_END);
f_len = ftell (rgb_thumbnail);
rewind (rgb_thumbnail);
thm_buffer = (unsigned char *) calloc (1, f_len + 1);
rerr = fread (thm_buffer, 1, f_len, rgb_thumbnail);

for (olp = 0; olp < 4; olp += 2)
	{
	for (hlp = 1; hlp < 33; hlp++)
		{
		for (llp = 0; llp < 2; llp++)
			{
			for (qlp = 1; qlp < 9; qlp++)
				{
				for (pos = 0; pos <10 ; pos++)
					{
					nine_byte_chunk [pos] = thm_buffer [lp];
					lp++;
					}
				lp = lp - 1;
				rgb_return = get_nine_six (nine_byte_chunk);
				quad_accum[olp + llp].red_val = quad_accum[olp + llp].red_val + rgb_return.red_val;
				quad_accum[olp + llp].grn_val = quad_accum[olp + llp].grn_val + rgb_return.grn_val;
				quad_accum[olp + llp].blu_val = quad_accum[olp + llp].blu_val + rgb_return.blu_val;
				quad_accum[olp + llp].gry_val = quad_accum[olp + llp].gry_val + (rgb_return.red_val + rgb_return.grn_val + rgb_return.blu_val) / 3;
				base_sixfour [0] = '0';
//printf ("%c%c%c ", base_sixfour [(int) rgb_return.red_val], base_sixfour [(int) rgb_return.grn_val], base_sixfour [(int) rgb_return.blu_val]);
//printf ("\n%f  %f  %f\n", rgb_return.red_val, rgb_return.grn_val, rgb_return.blu_val);
				}	// end qlp
			}	// end llp
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
//printf ("%s\tQuad: %d\tMax: %5.2f\tMin: %5.2f\tMaxChan: %d\n", img_name, olp, limits_return.max_val, limits_return.min_val, limits_return.channel);
	red_dec = quad_accum[olp].red_val / DECIMAL_DIVIDER;
	grn_dec = quad_accum[olp].grn_val / DECIMAL_DIVIDER;
	blu_dec = quad_accum[olp].blu_val / DECIMAL_DIVIDER;
//printf ("R: %f\tG: %f\tB: %f\n", red_dec, grn_dec, blu_dec);
	if (limits_return.channel == RED_CHAN )
		{
		if (limits_return.max_val - limits_return.min_val == 0)
			{
			if (limits_return.min_val == 0)
				{
				limits_return.max_val = limits_return.max_val + 0.00001;
				}
				else
				{
				limits_return.min_val = limits_return.min_val - 0.00001;
				}
			}
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
	base_sixfour [0] = '0';
	hue_print [olp] = base_sixfour [(int) hue_value];
	base_sixfour [0] = '0';
	gry_print[olp] = base_sixfour [(int) quad_accum[olp].gry_val];
//printf ("%f\n", quad_accum[olp].gry_val);
	}
base_sixfour [0] = '0';

mag_n = (powf ((float) (filename_separation.width * filename_separation.height), EXPONENT) / DIVIDER) - SUBTRACTOR;
printf ("%s\t%s\t%c\t%s\n", gry_print, hue_print, base_sixfour [(int) mag_n], filename_separation.name);
nn_len = snprintf (new_name, FILENAME_LENGTH, "%s_%s%s%c%s", filename_separation.name, gry_print, hue_print, base_sixfour [(int) mag_n], FILE_EXTN);
//printf ("%s\t%s\n", img_name, new_name);
//rename (img_name, new_name);
}
