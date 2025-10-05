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
struct colgry_accumulator lq_accum;
struct colgry_accumulator rq_accum;

char nine_byte_string [9];
char sixtyfour_bit [65] = SIXTYFOUR_BIT;

int qlp, olp;

float gry_value;

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
		lq_accum.red_val = lq_accum.red_val + rgb_return.red_val;
		lq_accum.grn_val = lq_accum.grn_val + rgb_return.grn_val;
		lq_accum.blu_val = lq_accum.blu_val + rgb_return.blu_val;
		lq_accum.gry_val = lq_accum.gry_val + gry_value;
		printf ("%c %c %c %c  ", sixtyfour_bit [(int) rgb_return.red_val], sixtyfour_bit [(int) rgb_return.grn_val], sixtyfour_bit [(int) rgb_return.blu_val], sixtyfour_bit [(int) gry_value]);
		}

	printf ("%7.2f %7.2f %7.2f %7.2f    ", lq_accum.red_val, lq_accum.grn_val, lq_accum.blu_val, lq_accum.gry_val);
//	printf ("%8.3f %8.3f %8.3f %8.3f    ", lq_accum.red_val, lq_accum.grn_val, lq_accum.blu_val, lq_accum.gry_val);

	for (qlp = 1; qlp < 9; qlp++)
		{
		fread (nine_byte_string, 9, 1, rgb_thumbnail);
		rgb_return = get_nine_six (nine_byte_string);
		gry_value = (int) (rgb_return.red_val + rgb_return.grn_val + rgb_return.blu_val) / 3;
		rq_accum.red_val = rq_accum.red_val + rgb_return.red_val;
		rq_accum.grn_val = rq_accum.grn_val + rgb_return.grn_val;
		rq_accum.blu_val = rq_accum.blu_val + rgb_return.blu_val;
		rq_accum.gry_val = rq_accum.gry_val + gry_value;
		printf ("%c %c %c %c  ", sixtyfour_bit [(int) rgb_return.red_val], sixtyfour_bit [(int) rgb_return.grn_val], sixtyfour_bit [(int) rgb_return.blu_val], sixtyfour_bit [(int) gry_value]);
		}

	printf ("%7.2f %7.2f %7.2f %7.2f\t%d\n", rq_accum.red_val, rq_accum.grn_val, rq_accum.blu_val, rq_accum.gry_val, olp);
//	printf ("%8.3f %8.3f %8.3f %8.3f\n", rq_accum.red_val, rq_accum.grn_val, rq_accum.blu_val, rq_accum.gry_val);
	}

	printf ("%7.2f %7.2f %7.2f %7.2f    ", (lq_accum.red_val / 256), (lq_accum.grn_val / 256), (lq_accum.blu_val / 256), (lq_accum.gry_val / 256));
	printf ("%7.2f %7.2f %7.2f %7.2f\n", (rq_accum.red_val / 256), (rq_accum.grn_val / 256), (rq_accum.blu_val / 256), (rq_accum.gry_val / 256));
	printf ("%c %c %c %c    ", sixtyfour_bit [(int) (lq_accum.red_val / 256)], sixtyfour_bit [(int) (lq_accum.grn_val / 256)], sixtyfour_bit [(int) (lq_accum.blu_val / 256)], sixtyfour_bit [(int) (lq_accum.gry_val / 256)]);
	printf ("%c %c %c %c\n", sixtyfour_bit [(int) (rq_accum.red_val / 256)], sixtyfour_bit [(int) (rq_accum.grn_val / 256)], sixtyfour_bit [(int) (rq_accum.blu_val / 256)], sixtyfour_bit [(int) (rq_accum.gry_val / 256)]);

fclose (rgb_thumbnail);
}

//SIXTYFOUR_BIT
