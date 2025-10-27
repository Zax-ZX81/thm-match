/* * * * * * * * * * * * * *
 *                         *
 *       tdiff             *
 *                         *
 *       2025-10-22        *
 *                         *
 *       Zax               *
 *                         *
 * * * * * * * * * * * * * */

#include <stdio.h>
#include "TMLib.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PROG_NAME "TDiff"
#define PROG_VERSION "0.30"

struct tprint_flags tpflags [1] = {0};
struct thumbprint_histogram tprint_hist [1] = {0};

int main (int argc, char *argv [])

{
char rgb_return_a [13] = "";
char rgb_return_b [13] = "";

char img_name_a [FILENAME_LENGTH] = NULL_STRING;
char img_name_b [FILENAME_LENGTH] = NULL_STRING;
char switch_chr;
unsigned char nine_byte_chunk_a [9];
unsigned char nine_byte_chunk_b [9];
unsigned char *thm_buffer_a;
unsigned char *thm_buffer_b;
unsigned char base_sixfour [65] = BASE_SIXTYFOUR;

int pos, r_idx, rerr_a, rerr_b, olp, dist, arg_no, switch_pos;
int ch_a, ch_b, ch_r, ch_g, pix_idx;
int err = FALSE;


/*struct thumbprint_histogram
        {
        unsigned int histogram [64];
        float hscale;
        float grey_mean;
        float grey_tot;
        float vari_tot;
        float cont_mult;
        float std_dev;
        float grey_val [4096];
        };
*/


//unsigned int *histogram = (unsigned int *) calloc (64, sizeof (unsigned int));

//float hscale, grey_mean, grey_tot, variance_tot;
//float *grey_val = (float *) calloc (4096, sizeof (float));
//float *cont_mult = (float *) calloc (1, sizeof (float));
//float *std_dev = (float *) calloc (1, sizeof (float));

FILE *IMGFILE_A;
FILE *IMGFILE_B;

// Arguments section
for (arg_no = 1; arg_no < argc; arg_no++)		// loop through arguments
	{
	if ((int) argv [arg_no] [0] == '-')
		{
		for (switch_pos = 1; switch_pos < strlen (argv[arg_no]); switch_pos++)
			{
			switch_chr = (int) argv [arg_no] [switch_pos];
			switch (switch_chr)
				{
				case 't':
					tpflags->tprt = SW_ON;
					break;
				case 'v':
					tpflags->verbose = SW_ON;
					break;
				case 'V':
					printf ("%s version %s\n", PROG_NAME, PROG_VERSION);
					exit (0);
				default:
					printf ("%s# TDiff [pstvV] <image A> <image B>%s\n", TEXT_YELLOW, TEXT_RESET);
					exit (0);
				}	// END switch
			}	// END for switch_pos
		}	// END if int argv
		else
		{
		if (strcmp (img_name_a, "") == 0)
			{
			strncpy (img_name_a, argv [arg_no], FILENAME_LENGTH);
			}
			else
			{
			if (strcmp (img_name_b, "") == 0)
				{
				strncpy (img_name_b, argv [arg_no], FILENAME_LENGTH);
				}
			}
		}	// END else if int argv
	}	// END for arg_no

//strncpy (img_name_a, argv [1], FILENAME_LENGTH);
//strncpy (img_name_b, argv [2], FILENAME_LENGTH);
printf ("%s, %s\n\n",img_name_a, img_name_b);
/*for (olp = 0; olp < 64; olp++)
	{
	printf ("%c=%4d.  ", base_sixfour [olp], tprint_hist->histogram [olp]);
	}
printf ("\n");*/

thm_buffer_a = (unsigned char *) calloc (1, THUMBNAIL_BYTES + 1);
thm_buffer_b = (unsigned char *) calloc (1, THUMBNAIL_BYTES + 1);
//printf ("1\n");
//printf ("2\n");
IMGFILE_A = fopen (img_name_a, "r");
IMGFILE_B = fopen (img_name_b, "r");
if (IMGFILE_A == NULL)
	{
	error_mess ("Error opening file: ", img_name_a);
	err = TRUE;
	}
if (IMGFILE_B == NULL)
	{
	error_mess ("Error opening file: ", img_name_b);
	err = TRUE;
	}
if (err == TRUE)
	{
	return 1;
	}
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
		tprint_hist->histogram [dist] = tprint_hist->histogram [dist] + 1;
		}
	for (r_idx = 0; r_idx < 12 ; r_idx += 3)
		{
		ch_r = sixfour_to_dec (rgb_return_a [r_idx]);
		ch_g = sixfour_to_dec (rgb_return_a [r_idx + 1]);
		ch_b = sixfour_to_dec (rgb_return_a [r_idx + 2]);
		tprint_hist->grey_val [pix_idx] = (ch_r + ch_g + ch_b) / 3;
		tprint_hist->grey_tot = tprint_hist->grey_tot + tprint_hist->grey_val [pix_idx];
//printf ("LP=%4d\tP=%4d\tG=%8.3f\tT=%8.3f\n", olp, pix_idx, tprint_hist->grey_val [pix_idx], tprint_hist->grey_tot);
		pix_idx++;
		}
	}
tprint_hist->grey_mean = tprint_hist->grey_tot / 4096.0;
//exit (0);

for (olp = 0; olp < 4096; olp++)
	{
	tprint_hist->vari_tot = tprint_hist->vari_tot + pow (tprint_hist->grey_val [olp] - tprint_hist->grey_mean, 2);
//printf ("G=%8.3f\tD=%8.3f\tS=%8.3f\tT=%8.3f\n", tprint_hist->grey_val [olp], tprint_hist->grey_val [olp] - tprint_hist->grey_mean, pow (tprint_hist->grey_val [olp] - tprint_hist->grey_mean, 2), tprint_hist->vari_tot);
	}
tprint_hist->std_dev = sqrt (tprint_hist->vari_tot / 4096.0);
tprint_hist->cont_mult = 3.0 - tprint_hist->std_dev;
printf ("GM=%8.3f\tSTDEV=%8.3f\tCM=%8.3f\n", tprint_hist->grey_mean, tprint_hist->std_dev, tprint_hist->cont_mult);
/*printf ("\nUnscaled\n");
for (olp = 0; olp < 8; olp++)
	{
	for (pos = 0; pos < 8; pos++)
		{
		if (tprint_hist->histogram [(olp * 8) + pos] == 0.0)
			{
			printf ("      ");
			}
			else
			{
			printf ("%5d ", tprint_hist->histogram [(olp * 8) + pos]);
			}
		}
	printf ("|\n");
	}
printf ("\n\n");

printf ("Percentage\n");
for (olp = 0; olp < 8; olp++)
	{
	for (pos = 0; pos < 8; pos++)
		{
		tprint_hist->hscale = 0.00813802083334 * tprint_hist->histogram [(olp * 8) + pos];
		if (tprint_hist->hscale == 0.0)
			{
			printf ("   ");
			}
			else
			{
			printf ("%3d", (int) tprint_hist->hscale);
			}
		}
	printf ("|\n");
	}*/
printf ("\n\n");

printf ("Per64age\n");
for (olp = 0; olp < 8; olp++)
	{
	for (pos = 0; pos < 8; pos++)
		{
		tprint_hist->hscale = 0.005126953125 * tprint_hist->histogram [(olp * 8) + pos];
		if (tprint_hist->std_dev < 2.0 && tprint_hist->hscale == 0)
			{
			printf (". ");
			}
			else
			{
			if (tprint_hist->hscale != 0.0)
				{
				printf ("%c ", base_sixfour [(int) tprint_hist->hscale]);
				}
				else
				{
				printf ("  ");
				}
			}
		}
	printf ("|\n");
	}

free (thm_buffer_a);
free (thm_buffer_b);
}
