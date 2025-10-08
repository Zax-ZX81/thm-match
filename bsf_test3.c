/* * * * * * * * * * * * * *
 *                         *
 *    base six four test   *
 *                         *
 *       2025-10-06        *
 *                         *
 *       Zax               *
 *                         *
 * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "TMLib.h"
#include <sys/types.h>
#include <sys/stat.h>

#define PROG_VERSION "0.10"

int main (int argc, char *argv [])

{
struct rgb_accumulator rgb_return;
struct maxmin_return limits_return;
struct colgry_accumulator quad_accum [4] = {{0}};
struct image_print quad_print [4] = {{0}};

char nine_byte_string [9];
char base_sixfour [65] = BASE_SIXTYFOUR;

int qlp, llp, hlp, olp;

float hue_value, red_eb, grn_eb, blu_eb;

FILE* rgb_thumbnail = fopen(argv [FILE_ARG - 1], "rb");

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
//				printf ("%c.%c.%c ", base_sixfour [(int) rgb_return.red_val], base_sixfour [(int) rgb_return.grn_val], base_sixfour [(int) rgb_return.blu_val]);
				quad_accum[olp + llp].red_val = quad_accum[olp + llp].red_val + rgb_return.red_val;
				quad_accum[olp + llp].grn_val = quad_accum[olp + llp].grn_val + rgb_return.grn_val;
				quad_accum[olp + llp].blu_val = quad_accum[olp + llp].blu_val + rgb_return.blu_val;
				quad_accum[olp + llp].gry_val = quad_accum[olp + llp].gry_val + (rgb_return.red_val + rgb_return.grn_val + rgb_return.blu_val) / 3;
				}	// end qlp
//			printf ("  ");
			}	// xend llp
//		printf ("\n");
		}	// end hlp
//	printf ("\n");
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
	printf ("%s\tQuadrant %d\tMax: %5.2f\t", argv [FILE_ARG - 1], olp, limits_return.max_val);
	printf ("Min: %5.2f\t", limits_return.min_val);
	printf ("MaxChan: %d\t", limits_return.channel);
	red_eb = quad_accum[olp].red_val / 64;
	grn_eb = quad_accum[olp].grn_val / 64;
	blu_eb = quad_accum[olp].blu_val / 64;
	printf ("R: %f\tG: %f\tB: %f\t", red_eb, grn_eb, blu_eb);
	if (limits_return.channel == RED_CHAN )
		{
		hue_value = (grn_eb - blu_eb) / (limits_return.max_val - limits_return.min_val);
		if (hue_value < 0)
			{
			hue_value = hue_value + 6;
			}
		hue_value = hue_value * 10.5;
//		printf ("R%f  %f  %fR\t", (float) fabs (grn_eb - blu_eb), (limits_return.max_val - limits_return.min_val), fmodf ((float) fabs (grn_eb - blu_eb) / (limits_return.max_val - limits_return.min_val), 6));
		}
	if (limits_return.channel == GRN_CHAN )
		{
		hue_value = (2 + ((blu_eb - red_eb) / (limits_return.max_val - limits_return.min_val))) * 10.5;
		}
	if (limits_return.channel == BLU_CHAN )
		{
		hue_value = (4 + ((red_eb - grn_eb) / (limits_return.max_val - limits_return.min_val))) * 10.5;
		}
	printf ("\tHue %d: %5.2f\t%c\n", olp, hue_value, base_sixfour [(int) hue_value]);
	quad_print[olp].hue = base_sixfour [(int) hue_value];
	quad_print[olp].gry = base_sixfour [(int) quad_accum[olp].gry_val];
	}

for (olp = 0; olp < 4; olp++)
	{
	printf ("%c", quad_print[olp].gry);
	}
printf ("\n");
for (olp = 0; olp < 4; olp++)
	{
	printf ("%c", quad_print[olp].hue);
	}
printf ("\n");
}
