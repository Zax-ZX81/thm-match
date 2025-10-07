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
struct colgry_accumulator quad_accum [4] = {{0}};
struct maxmin_return limit_return;

char nine_byte_string [9];
char sixtyfour_bit [65] = SIXTYFOUR_BIT;

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
//				printf ("%c.%c.%c ", sixtyfour_bit [(int) rgb_return.red_val], sixtyfour_bit [(int) rgb_return.grn_val], sixtyfour_bit [(int) rgb_return.blu_val]);
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

for (olp = 0; olp < 4; olp++)
	{
	quad_accum[olp].red_val = quad_accum[olp].red_val / 256;
	quad_accum[olp].grn_val = quad_accum[olp].grn_val / 256;
	quad_accum[olp].blu_val = quad_accum[olp].blu_val / 256;
	quad_accum[olp].gry_val = quad_accum[olp].gry_val / 256;
	}

for (olp = 0; olp < 4; olp++)
	{
	limit_return = find_max (quad_accum[olp].red_val, quad_accum[olp].grn_val, quad_accum[olp].blu_val);
	limit_return.max_val = limit_return.max_val / 64;
	limit_return.min_val = limit_return.min_val / 64;
	printf ("%s\tQuadrant %d\tMax: %5.2f\t", argv [FILE_ARG - 1], olp, limit_return.max_val);
	printf ("Min: %5.2f\t", limit_return.min_val);
	printf ("MaxChan: %d\t", limit_return.channel);
	red_eb = quad_accum[olp].red_val / 64;
	grn_eb = quad_accum[olp].grn_val / 64;
	blu_eb = quad_accum[olp].blu_val / 64;
	printf ("R: %f\tG: %f\tB: %f\t", red_eb, grn_eb, blu_eb);
	if (limit_return.channel == RED_CHAN )
		{
		hue_value = (grn_eb - blu_eb) / (limit_return.max_val - limit_return.min_val);
		if (hue_value < 0)
			{
			hue_value = hue_value + 6;
			}
//		printf ("R%f  %f  %fR\t", (float) fabs (grn_eb - blu_eb), (limit_return.max_val - limit_return.min_val), fmodf ((float) fabs (grn_eb - blu_eb) / (limit_return.max_val - limit_return.min_val), 6));
		}
	if (limit_return.channel == GRN_CHAN )
		{
		hue_value = 2 + ((blu_eb - red_eb) / (limit_return.max_val - limit_return.min_val));
		}
	if (limit_return.channel == BLU_CHAN )
		{
		hue_value = 4 + ((red_eb - grn_eb) / (limit_return.max_val - limit_return.min_val));
		}

//	printf ("%c%c%c%c ", sixtyfour_bit [(int) quad_accum[0].red_val], sixtyfour_bit [(int) quad_accum[0].grn_val], sixtyfour_bit [(int) quad_accum[0].blu_val], sixtyfour_bit [(int) quad_accum[0].gry_val]);
//	printf ("%c%c%c%c\n", sixtyfour_bit [(int) quad_accum[1].red_val], sixtyfour_bit [(int) quad_accum[1].grn_val], sixtyfour_bit [(int) quad_accum[1].blu_val], sixtyfour_bit [(int) quad_accum[1].gry_val]);

//	printf ("%c%c%c%c ", sixtyfour_bit [(int) quad_accum[2].red_val], sixtyfour_bit [(int) quad_accum[2].grn_val], sixtyfour_bit [(int) quad_accum[2].blu_val], sixtyfour_bit [(int) quad_accum[2].gry_val]);
//	printf ("%c%c%c%c\n", sixtyfour_bit [(int) quad_accum[3].red_val], sixtyfour_bit [(int) quad_accum[3].grn_val], sixtyfour_bit [(int) quad_accum[3].blu_val], sixtyfour_bit [(int) quad_accum[3].gry_val]);

	printf ("\tHue %d: %5.2f\t%c\n", olp, hue_value, sixtyfour_bit [(int) hue_value]);

	}

/*


float fmodf(float x, float y);



if (ulq_output.red_val > ulq_output.grn_val || ulq_output.red_val > ulq_output.blu_val)
	{
	(ulq_output.grn_val - ulq_output.blu_val) 
	}


r>(g|r)=MOD((g-b)/(MIN-MAX),6)*10.5
g>(r|b)=(2+((b-r)/(MIN-MAX)))*10.5
r>(r|g)=(4+((r-g)/(MIN-MAX)))*10.5

*/
fclose (rgb_thumbnail);
}

//SIXTYFOUR_BIT
