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
#include "TMLib.h"
#include <sys/types.h>
#include <sys/stat.h>

#define PROG_VERSION "0.10"

int main (int argc, char *argv [])

{
struct rgb_accumulator rgb_return;
struct colgry_accumulator quad_accum [4] = {{0}};
struct maxmin_return max_return;
struct maxmin_return min_return;

char nine_byte_string [9];
char sixtyfour_bit [65] = SIXTYFOUR_BIT;

int qlp, llp, hlp, olp;

float hue_value;

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
				printf ("%c.%c.%c ", sixtyfour_bit [(int) rgb_return.red_val], sixtyfour_bit [(int) rgb_return.grn_val], sixtyfour_bit [(int) rgb_return.blu_val]);
				quad_accum[olp + llp].red_val = quad_accum[olp + llp].red_val + rgb_return.red_val;
				quad_accum[olp + llp].grn_val = quad_accum[olp + llp].grn_val + rgb_return.grn_val;
				quad_accum[olp + llp].blu_val = quad_accum[olp + llp].blu_val + rgb_return.blu_val;
				quad_accum[olp + llp].gry_val = quad_accum[olp + llp].gry_val + (rgb_return.red_val + rgb_return.grn_val + rgb_return.blu_val) / 3;
				}	// end qlp
			printf ("  ");
			}	// xend llp
		printf ("\n");
		}	// end hlp
	printf ("\n");
	}	// end olp
for (olp = 0; olp < 4; olp++)
	{
	quad_accum[olp].red_val = quad_accum[olp].red_val / 256;
	quad_accum[olp].grn_val = quad_accum[olp].grn_val / 256;
	quad_accum[olp].blu_val = quad_accum[olp].blu_val / 256;
	quad_accum[olp].gry_val = quad_accum[olp].gry_val / 256;
	}

printf ("%c%c%c%c ", sixtyfour_bit [(int) quad_accum[0].red_val], sixtyfour_bit [(int) quad_accum[0].grn_val], sixtyfour_bit [(int) quad_accum[0].blu_val], sixtyfour_bit [(int) quad_accum[0].gry_val]);
printf ("%c%c%c%c\n", sixtyfour_bit [(int) quad_accum[1].red_val], sixtyfour_bit [(int) quad_accum[1].grn_val], sixtyfour_bit [(int) quad_accum[1].blu_val], sixtyfour_bit [(int) quad_accum[1].gry_val]);

printf ("%c%c%c%c ", sixtyfour_bit [(int) quad_accum[2].red_val], sixtyfour_bit [(int) quad_accum[2].grn_val], sixtyfour_bit [(int) quad_accum[2].blu_val], sixtyfour_bit [(int) quad_accum[2].gry_val]);
printf ("%c%c%c%c\n", sixtyfour_bit [(int) quad_accum[3].red_val], sixtyfour_bit [(int) quad_accum[3].grn_val], sixtyfour_bit [(int) quad_accum[3].blu_val], sixtyfour_bit [(int) quad_accum[3].gry_val]);

max_return = find_max (quad_accum[0].red_val, quad_accum[0].grn_val, quad_accum[0].blu_val);
min_return = find_min (quad_accum[0].red_val, quad_accum[0].grn_val, quad_accum[0].blu_val);
if (max_return.channel == RED_CHAN )
	{
	hue_value = (quad_accum[0].grn_val - quad_accum[0].blu_val) / (min_return.channel - max_return.channel) * 10.5;
	}
if (max_return.channel == GRN_CHAN )
	{
	hue_value = 2 + (quad_accum[0].grn_val - quad_accum[0].blu_val) / (min_return.channel - max_return.channel) * 10.5;
	}
if (max_return.channel == BLU_CHAN )
	{
	hue_value = 4 + (quad_accum[0].grn_val - quad_accum[0].blu_val) / (min_return.channel - max_return.channel) * 10.5;
	}
printf ("Max ULQ: %5.2f\t", max_return.value);
printf ("Min ULQ: %5.2f\n", min_return.value);

/*if (ulq_output.red_val > ulq_output.grn_val || ulq_output.red_val > ulq_output.blu_val)
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
