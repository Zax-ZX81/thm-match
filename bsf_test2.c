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
struct colgry_accumulator ulq_accum;
struct colgry_accumulator urq_accum;
struct colgry_accumulator llq_accum;
struct colgry_accumulator lrq_accum;
struct colgry_accumulator ulq_output;
struct colgry_accumulator urq_output;
struct colgry_accumulator llq_output;
struct colgry_accumulator lrq_output;
struct maxmin_return max_return;
struct maxmin_return min_return;

char ulq_max, urq_max, llq_max, lrq_max, ulq_min, urq_min, llq_min, lrq_min;
char nine_byte_string [9];
char sixtyfour_bit [65] = SIXTYFOUR_BIT;

int qlp, olp;

float hue_value, gry_value;

FILE* rgb_thumbnail = fopen(argv [FILE_ARG - 1], "rb");

if (rgb_thumbnail == NULL)
	{
	perror("Error opening file");
	return 1;
	}

for (olp = 1; olp < 33; olp++)
	{
	for (qlp = 1; qlp < 9; qlp++)
		{
		fread (nine_byte_string, 9, 1, rgb_thumbnail);
		rgb_return = get_nine_six (nine_byte_string);
		gry_value = (int) (rgb_return.red_val + rgb_return.grn_val + rgb_return.blu_val) / 3;
		ulq_accum.red_val = ulq_accum.red_val + rgb_return.red_val;
		ulq_accum.grn_val = ulq_accum.grn_val + rgb_return.grn_val;
		ulq_accum.blu_val = ulq_accum.blu_val + rgb_return.blu_val;
		ulq_accum.gry_val = ulq_accum.gry_val + gry_value;
//		printf ("%c%c ", sixtyfour_bit [(int) gry_value], sixtyfour_bit [(int) gry_value]);
//		printf ("%c %c %c %c  ", sixtyfour_bit [(int) rgb_return.red_val], sixtyfour_bit [(int) rgb_return.grn_val], sixtyfour_bit [(int) rgb_return.blu_val], sixtyfour_bit [(int) gry_value]);
		}
//	printf (" ");

	for (qlp = 1; qlp < 9; qlp++)
		{
		fread (nine_byte_string, 9, 1, rgb_thumbnail);
		rgb_return = get_nine_six (nine_byte_string);
		gry_value = (int) (rgb_return.red_val + rgb_return.grn_val + rgb_return.blu_val) / 3;
		urq_accum.red_val = urq_accum.red_val + rgb_return.red_val;
		urq_accum.grn_val = urq_accum.grn_val + rgb_return.grn_val;
		urq_accum.blu_val = urq_accum.blu_val + rgb_return.blu_val;
		urq_accum.gry_val = urq_accum.gry_val + gry_value;
//		printf ("%c%c ", sixtyfour_bit [(int) gry_value], sixtyfour_bit [(int) gry_value]);
//		printf ("%c %c %c %c  ", sixtyfour_bit [(int) rgb_return.red_val], sixtyfour_bit [(int) rgb_return.grn_val], sixtyfour_bit [(int) rgb_return.blu_val], sixtyfour_bit [(int) gry_value]);
		}
//	printf ("\n");
	}

//printf ("\n");

for (olp = 1; olp < 33; olp++)
	{
	for (qlp = 1; qlp < 9; qlp++)
		{
		fread (nine_byte_string, 9, 1, rgb_thumbnail);
		rgb_return = get_nine_six (nine_byte_string);
		gry_value = (int) (rgb_return.red_val + rgb_return.grn_val + rgb_return.blu_val) / 3;
		llq_accum.red_val = llq_accum.red_val + rgb_return.red_val;
		llq_accum.grn_val = llq_accum.grn_val + rgb_return.grn_val;
		llq_accum.blu_val = llq_accum.blu_val + rgb_return.blu_val;
		llq_accum.gry_val = llq_accum.gry_val + gry_value;
//		printf ("%c%c ", sixtyfour_bit [(int) gry_value], sixtyfour_bit [(int) gry_value]);
//		printf ("%c %c %c %c  ", sixtyfour_bit [(int) rgb_return.red_val], sixtyfour_bit [(int) rgb_return.grn_val], sixtyfour_bit [(int) rgb_return.blu_val], sixtyfour_bit [(int) gry_value]);
		}
//	printf (" ");

	for (qlp = 1; qlp < 9; qlp++)
		{
		fread (nine_byte_string, 9, 1, rgb_thumbnail);
		rgb_return = get_nine_six (nine_byte_string);
		gry_value = (int) (rgb_return.red_val + rgb_return.grn_val + rgb_return.blu_val) / 3;
		lrq_accum.red_val = lrq_accum.red_val + rgb_return.red_val;
		lrq_accum.grn_val = lrq_accum.grn_val + rgb_return.grn_val;
		lrq_accum.blu_val = lrq_accum.blu_val + rgb_return.blu_val;
		lrq_accum.gry_val = lrq_accum.gry_val + gry_value;
//		printf ("%c%c ", sixtyfour_bit [(int) gry_value], sixtyfour_bit [(int) gry_value]);
//		printf ("%c %c %c %c  ", sixtyfour_bit [(int) rgb_return.red_val], sixtyfour_bit [(int) rgb_return.grn_val], sixtyfour_bit [(int) rgb_return.blu_val], sixtyfour_bit [(int) gry_value]);
		}
//	printf ("\n");

	}
ulq_output.red_val = ulq_accum.red_val / 256;
ulq_output.grn_val = ulq_accum.grn_val / 256;
ulq_output.blu_val = ulq_accum.blu_val / 256;
ulq_output.gry_val = ulq_accum.gry_val / 256;

urq_output.red_val = urq_accum.red_val / 256;
urq_output.grn_val = urq_accum.grn_val / 256;
urq_output.blu_val = urq_accum.blu_val / 256;
urq_output.gry_val = urq_accum.gry_val / 256;

llq_output.red_val = llq_accum.red_val / 256;
llq_output.grn_val = llq_accum.grn_val / 256;
llq_output.blu_val = llq_accum.blu_val / 256;
llq_output.gry_val = llq_accum.gry_val / 256;

lrq_output.red_val = lrq_accum.red_val / 256;
lrq_output.grn_val = lrq_accum.grn_val / 256;
lrq_output.blu_val = lrq_accum.blu_val / 256;
lrq_output.gry_val = lrq_accum.gry_val / 256;

printf ("%c%c%c%c ", sixtyfour_bit [(int) ulq_output.red_val], sixtyfour_bit [(int) ulq_output.grn_val], sixtyfour_bit [(int) ulq_output.blu_val], sixtyfour_bit [(int) ulq_output.gry_val]);
printf ("%c%c%c%c\n", sixtyfour_bit [(int) urq_output.red_val], sixtyfour_bit [(int) urq_output.grn_val], sixtyfour_bit [(int) urq_output.blu_val], sixtyfour_bit [(int) urq_output.gry_val]);

printf ("%c%c%c%c ", sixtyfour_bit [(int) llq_output.red_val], sixtyfour_bit [(int) llq_output.grn_val], sixtyfour_bit [(int) llq_output.blu_val], sixtyfour_bit [(int) llq_output.gry_val]);
printf ("%c%c%c%c\n", sixtyfour_bit [(int) lrq_output.red_val], sixtyfour_bit [(int) lrq_output.grn_val], sixtyfour_bit [(int) lrq_output.blu_val], sixtyfour_bit [(int) lrq_output.gry_val]);

max_return = find_max (ulq_output.red_val, ulq_output.grn_val, ulq_output.blu_val);
min_return = find_min (ulq_output.red_val, ulq_output.grn_val, ulq_output.blu_val);
if (max_return.channel == RED_CHAN )
	{
	hue_value = (ulq_output.grn_val - ulq_output.blu_val) / (min_return.channel - max_return.channel) * 10.5;
	}
if (max_return.channel == GRN_CHAN )
	{
	hue_value = 2 + (ulq_output.grn_val - ulq_output.blu_val) / (min_return.channel - max_return.channel) * 10.5;
	}
if (max_return.channel == BLU_CHAN )
	{
	hue_value = 4 + (ulq_output.grn_val - ulq_output.blu_val) / (min_return.channel - max_return.channel) * 10.5;
	}
printf ("Max ULQ: %d\t", ulq_max);
printf ("Min ULQ: %d\n", ulq_min);

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
