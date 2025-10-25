#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/stat.h>
#include "TMLib.h"

struct tprint_database thumbprint (char *img_name)

{
extern struct tprint_flags tpflags;

struct rgb_accumulator rgb_return;
struct maxmin_return limits_return;
struct colgry_accumulator quad_accum [4] = {{0}};
struct dimension_return mag_separation;
struct stat sb;
struct tprint_database tprint_return;

char out_name [FILENAME_LENGTH] = NULL_STRING;
char cmd_line [FILENAME_LENGTH] = NULL_STRING;
char file_name [FILENAME_LENGTH] = NULL_STRING;
char file_path [FILENAME_LENGTH] = NULL_STRING;
char out_path [FILENAME_LENGTH] = NULL_STRING;
unsigned char nine_byte_chunk [9];
unsigned char *thm_buffer;
unsigned char base_sixfour [65] = BASE_SIXTYFOUR;
char mag_string [12];
char hue_print [5] = NULL_STRING;
char gry_print [5] = NULL_STRING;

int lp = 0;
int qlp, llp, hlp, olp, rd_err, pos, n_chrs;
int wr_items = 0;

float hue_value, red_dec, grn_dec, blu_dec, mag_n;

FILE *IMGFILE;
FILE *rgb_thumbnail;
FILE *out_thumbnail;

n_chrs = snprintf (cmd_line, FILENAME_LENGTH, "%s%s%s", MAGICK_COMMAND, enquote (img_name), RGB_ARGS);
//printf ("CL=%s=W=%d=\n", cmd_line, wr_items);
rgb_thumbnail = popen (cmd_line, "r");
thm_buffer = (unsigned char *) calloc (1, THUMBNAIL_BYTES + 1);
rd_err = fread (thm_buffer, 1, THUMBNAIL_BYTES, rgb_thumbnail);

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
//				base_sixfour [0] = '0';
//printf ("%1.1f %1.1f %1.1f O%d H%d L%d Q%d  ", rgb_return.red_val, rgb_return.grn_val, rgb_return.blu_val, olp, hlp, llp, qlp);
				}	// end qlp
//printf ("\n");
			}	// end llp
		}	// end hlp
	}	// end olp

if (tpflags.tprt == SW_ON)
	{
	if (strrchr (img_name, 47))
		{
		strcpy (file_name, strrchr (img_name, 47) + 1);
		strncpy (file_path, img_name, strrchr (img_name, 47) - img_name + 1);
		n_chrs = snprintf (out_path, FILENAME_LENGTH, "%s%s", file_path, "s/");
		n_chrs = snprintf (out_name, FILENAME_LENGTH, "%s%s%s", out_path, file_name, FILE_EXTN);
//printf ("Sub: I=%s=\tN=%s=\tP=%s=\n", img_name, out_name, out_path);
		}
		else
		{
		strcpy (out_path, "s/");
		strcat (out_name, img_name);
		n_chrs = snprintf (out_name, FILENAME_LENGTH, "%s%s%s%s", file_path, "s/", img_name, FILE_EXTN);
//printf ("NoSub: I=%s=\tN=%s=\n", img_name, out_name);
		}
//printf ("FILE_PATH=%s\tFILE_NAME=%s\n", file_path, file_name);
	if (stat (out_path, &sb) == -1)
		{
		mkdir (out_path, 0700);
		}
	out_thumbnail = fopen (out_name, "wb");
//printf ("Writing =%s=\n", out_name);
	wr_items = fwrite (thm_buffer, THUMBNAIL_BYTES, 1, out_thumbnail);
	fclose (out_thumbnail);
	}

fclose (rgb_thumbnail);
free (thm_buffer);

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

n_chrs = snprintf (cmd_line, FILENAME_LENGTH, "%s%s%s", MAGICK_COMMAND,  MAG_ARGS, img_name);
IMGFILE = popen (cmd_line, "r");
rd_err = (long) fgets (mag_string, 12, IMGFILE);
fclose (IMGFILE);
mag_separation = separate_magnitude (mag_string);
mag_n = (powf ((float) (mag_separation.width * mag_separation.height), EXPONENT) / DIVIDER) - SUBTRACTOR;
if (mag_n < 0)
	{
	mag_n = 0;
	}
strcpy (tprint_return.gry_print, gry_print);
strcpy (tprint_return.hue_print, hue_print);
tprint_return.magnitude [0] = base_sixfour [(int) mag_n];
strcpy (tprint_return.filepath, img_name);
if (wr_items)
	{
	printf ("%s*%s ", TEXT_RED, TEXT_RESET);
	}

return (tprint_return);
//n_chrs = snprintf (new_name, FILENAME_LENGTH, "%s_%s%s%c%s", img_name, gry_print, hue_print, base_sixfour [(int) mag_n], FILE_EXTN);
//printf ("%s\t%s\n", img_name, new_name);
}
