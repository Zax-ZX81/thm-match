/* * * * * * * * * * * * * *
 *                         *
 *       cmp_test1         *
 *                         *
 *       2025-10-08        *
 *                         *
 *       Zax               *
 *                         *
 * * * * * * * * * * * * * */

#include <stdio.h>
#include "TMLib.h"
#include <stdlib.h>
#include <string.h>

#define PROG_NAME "Cmp_Test"
#define PROG_VERSION "0.30"

int main (int argc, char *argv [])

{
char rgb_return_a [13] = "";
char rgb_return_b [13] = "";

char img_name_a [FILENAME_LENGTH] = NULL_STRING;
char img_name_b [FILENAME_LENGTH] = NULL_STRING;
unsigned char nine_byte_chunk_a [9];
unsigned char nine_byte_chunk_b [9];
unsigned char *thm_buffer_a;
unsigned char *thm_buffer_b;
unsigned char base_sixfour [65] = BASE_SIXTYFOUR;
unsigned char hc;

int lp = 0;
int pos, r_idx, rerr_a, rerr_b;
int r_pos = 0;
int ch_a, ch_b, olp, dist;
unsigned int *histogram = (unsigned int *) calloc (64, sizeof (unsigned int));

float hscale;

FILE *IMGFILE_A;
FILE *IMGFILE_B;


strncpy (img_name_a, argv [1], FILENAME_LENGTH);
strncpy (img_name_b, argv [2], FILENAME_LENGTH);
printf ("%s, %s\n",img_name_a, img_name_b);
/*for (olp = 0; olp < 64; olp++)
	{
	printf ("%c=%4d.  ", base_sixfour [olp], histogram [olp]);
	}
printf ("\n");*/

thm_buffer_a = (unsigned char *) calloc (1, THUMBNAIL_BYTES + 1);
thm_buffer_b = (unsigned char *) calloc (1, THUMBNAIL_BYTES + 1);
//printf ("1\n");
//printf ("2\n");
IMGFILE_A = fopen (img_name_a, "r");
IMGFILE_B = fopen (img_name_b, "r");
//printf ("3\n");
rerr_a = fread (thm_buffer_a, 1, THUMBNAIL_BYTES, IMGFILE_A);
rerr_b = fread (thm_buffer_b, 1, THUMBNAIL_BYTES, IMGFILE_B);
//printf ("4\n");
fclose (IMGFILE_A);
fclose (IMGFILE_B);

for (olp = 0; olp < 9216; olp += 9)
	{
	for (pos = 0; pos < 9 ; pos++)
		{
		nine_byte_chunk_a [pos] = thm_buffer_a [olp + pos];
		nine_byte_chunk_b [pos] = thm_buffer_b [olp + pos];
//printf ("I=%4d.\tA=%4d.\tB=%4d.\n", olp + pos, thm_buffer_a [olp + pos], thm_buffer_b [olp + pos]);
		}
	strcpy (rgb_return_a, twelve_six_bit (nine_byte_chunk_a));
	strcpy (rgb_return_b, twelve_six_bit (nine_byte_chunk_b));
//printf ("A=%s\tB=%s\n", rgb_return_a, rgb_return_b);
	for (r_idx = 0; r_idx < 12 ; r_idx++)
		{
		ch_a = sixfour_to_dec (rgb_return_a [r_idx]);
		ch_b = sixfour_to_dec (rgb_return_b [r_idx]);
		dist = abs (ch_a - ch_b);
//printf ("LP=%4d\tSa=%c\tSb=%c\t%4d\n", olp, rgb_return_a [r_idx], rgb_return_b [r_idx], dist);
		histogram [dist] = histogram [dist] + 1;
		}
	}

/*for (olp = 0; olp < 64; olp++)
	{
//	if (histogram [olp] > 0)
//		{
		hscale = 0.005126953125 * histogram [olp];
		hc = base_sixfour [(int) hscale];
		base_sixfour [0] = '0';
		printf ("S=%c=D=%d=\tHS=%c=HD=%f=\n", base_sixfour [olp], olp, hc, hscale);
//		printf ("%c=%c ", base_sixfour [olp], base_sixfour [(int) hscale]);
//		}
	}
printf ("\n");

for (olp = 0; olp < 64; olp++)
	{
//	if (histogram [olp] > 0)
//		{
		hscale = 0.00813802083334 * histogram [olp];
		printf ("%02d ", (int) hscale);
//		printf ("%c=%c ", base_sixfour [olp], base_sixfour [(int) hscale]);
//		}
	}
printf ("\n");
*/
for (olp = 0; olp < 8; olp++)
	{
	for (pos = 0; pos < 8; pos++)
		{
		hscale = 0.005126953125 * histogram [(olp * 8) + pos];
		if (hscale == 0.0)
			{
			printf ("  ");
			}
			else
			{
	                printf ("%c ", base_sixfour [(int) hscale]);
			}
		}
	printf ("|\n");
	}

free (thm_buffer_a);
free (thm_buffer_b);
free (histogram);
}
