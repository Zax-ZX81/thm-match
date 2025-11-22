#include <stdio.h>
#include <math.h>
#include "TMLib.h"
#include "six_sixfour.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main (int argc, char *argv [])
{
int width, height, comp;
int rlp, olp, scl_div, scl_mod, pix_cnt, max_side, y_scl_rmd, x_scl_rmd, pos, mag_n, arg_no, switch_pos;
int lp = 0, qlp, llp, hlp;
int pix_ofs = 0, ir_pos = 0, y_inc = 0, x_inc = 0, y_acc = 0, x_acc = 0, y_tot = 0, x_tot = 0, ebo_vals = 0, thmb_vals = 0;
long racc = 0, gacc = 0, bacc = 0, wr_items = 0;
float scl_div_fp, red_dec, grn_dec, blu_dec, hue_value;

char img_name [FILENAME_LENGTH] = NULL_STRING;
unsigned char *img_raw;				// Image input buffer via stbl_load
unsigned char *eightbit_thmb_buff;		// 8 bit per channel thumbnail buffer
unsigned char *thmb_buff;			// 6 bit per channel thumbnail buffer
unsigned char twelve_vals [13] = NULL_STRING;
unsigned char nine_byte_chunk [9];
unsigned char base_sixfour [65] = BASE_SIXTYFOUR;
char aspect = 0;
char hue_print [5] = NULL_STRING;
char gry_print [5] = NULL_STRING;
char switch_chr;

struct rgb_return pix_return;			// Thumbnail pixel info
struct rgb_return *in_img_buff;			// Square padded 8 bit per channel input buffer
struct colgry_accumulator quad_accum [4] = {{0}};
struct maxmin_return limits_return;
struct rgb_accumulator rgb_return;

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
//					tpflags->tprt = SW_ON;
					break;
				case 'v':
//					tpflags->verbose = SW_ON;
					break;
				case 'V':
//					printf ("%s version %s\n", PROG_NAME, PROG_VERSION);
					exit (0);
				default:
					printf ("%s# i_read [] <image file>%s\n", TEXT_YELLOW, TEXT_RESET);
					exit (0);
				}	// END switch
			}	// END for switch_pos
		}	// END if int argv
		else
		{
		if (strcmp (img_name, NULL_STRING) == 0)
			{
			strncpy (img_name, argv [arg_no], FILENAME_LENGTH);
			}
		}	// END else if int argv
	}	// END for arg_no

//strncpy (img_name, argv [1], FILENAME_LENGTH);
img_raw = stbi_load (img_name, &width, &height, &comp, 0);
FILE *out_thumbnail;
out_thumbnail = fopen ("p.rgb", WRITE_BINARY);
if (width < height)
	{
	aspect = PORTRAIT;
	max_side = height;
	}
	else
	{
	if (width > height)
		{
		aspect = LANDSC;
		}
		else
		{
		aspect = SQUARE;
		}
	max_side = width;
	}
pix_cnt = max_side * max_side;
scl_div = max_side / 64;
scl_mod = max_side % 64;
scl_div_fp = (float) max_side / 64;
y_scl_rmd = scl_mod;
x_scl_rmd = scl_mod;
//printf ("W=%d, H=%d, C=%d, A=%d, PC=%d, SD=%d, SM=%d, Sf=%5.3f\n", width, height, comp, aspect, pix_cnt, scl_div, scl_mod, scl_div_fp);

in_img_buff = (struct rgb_return *) malloc (sizeof (struct rgb_return) * pix_cnt);
eightbit_thmb_buff = (unsigned char *) malloc (sizeof (unsigned char) * (THUMBNAIL_PIXELS * 3));
thmb_buff = (unsigned char *) calloc (1, THUMBNAIL_BYTES + 1);

if (aspect != SQUARE)
	{
printf ("Padding %d\n", pix_cnt);
	for (olp = 0;olp < pix_cnt;olp++)
		{
		in_img_buff [olp].red_val = 127;
		in_img_buff [olp].grn_val = 127;
		in_img_buff [olp].blu_val = 127;
		}
	}
switch (aspect)
	{
	case PORTRAIT:
		pix_ofs = (height - width) / 2;
		printf ("Portrait, %d, %d\n", pix_ofs, max_side - pix_ofs);
		for (olp = 0;olp < height;olp++)
			{
			for (rlp = pix_ofs;rlp < max_side - pix_ofs;rlp++)
				{
				in_img_buff [(olp * height) + rlp].red_val = img_raw [ir_pos++];
				in_img_buff [(olp * height) + rlp].grn_val = img_raw [ir_pos++];
				in_img_buff [(olp * height) + rlp].blu_val = img_raw [ir_pos++];
				}
			}
		break;
	case LANDSC:
		pix_ofs = (width - height) / 2;
		printf ("Landscape, %d\n", pix_ofs);
		for (olp = width * pix_ofs;olp < ((width * width) - (width * pix_ofs));olp++)
			{
//printf ("W=%d, H=%d, PO=%d, L=%d\n", width, height, pix_ofs, olp);
			in_img_buff [olp].red_val = img_raw [ir_pos++];
			in_img_buff [olp].grn_val = img_raw [ir_pos++];
			in_img_buff [olp].blu_val = img_raw [ir_pos++];
			}
		break;
	case SQUARE:
		printf ("Square\n");
		for (olp = 0;olp < pix_cnt;olp++)
			{
//printf (".");
			in_img_buff [olp].red_val = img_raw [ir_pos++];
			in_img_buff [olp].grn_val = img_raw [ir_pos++];
			in_img_buff [olp].blu_val = img_raw [ir_pos++];
			}
		break;
	}
//printf ("\nW=%d, H=%d, C=%d, A=%d\n", width, height, comp, aspect);

if (!scl_mod)
	{
	printf ("Clean divide\n");
	for (olp = 0; olp < 64; olp++)
		{
		for (rlp = 0; rlp < 64; rlp++)
			{
			pix_return = get_pixel (in_img_buff, (olp * max_side * scl_div) + (rlp * scl_div), max_side, scl_div, scl_div);
			racc = racc + pix_return.red_val;
			gacc = gacc + pix_return.grn_val;
			bacc = bacc + pix_return.blu_val;
			eightbit_thmb_buff [ebo_vals++] = pix_return.red_val / (scl_div * scl_div);
			eightbit_thmb_buff [ebo_vals++] = pix_return.grn_val / (scl_div * scl_div);
			eightbit_thmb_buff [ebo_vals++] = pix_return.blu_val / (scl_div * scl_div);
//			fputc (pix_return.red_val, out_thumbnail);
//printf ("O=%2d, R=%2d, A=%7d, M=%5d\t", olp, rlp, (olp * max_side * scl_div) + (rlp * scl_div), max_side);
//printf ("OL=%2d\tRL=%2d\tR=%2d, G=%2d, R=%2d\n", olp, rlp, pix_return.red_val / 1296, pix_return.grn_val / 1296, pix_return.blu_val / 1296);
			}
		}
	}
	else
	{
	printf ("Remainder\n");
	for (olp = 0; olp < 64; olp++)
		{
		if ((float) (scl_div_fp * olp) > (float) (scl_div * olp + 0.5 + y_acc) && olp != 63)		// kludgy workaround, better test needed
			{
//printf ("\t\t\t\t\t\tYYY\n");
			y_inc++;
			y_acc++;
			}
		for (rlp = 0; rlp < 64; rlp++)
			{
			if ((float) (scl_div_fp * rlp) > (float) (scl_div * rlp + 0.5 + x_acc))
				{
//printf ("\t\t\t\t\t\tX\n");
				x_inc++;
				x_acc++;
				}
			pix_return = get_pixel (in_img_buff, (olp * max_side * scl_div) + (max_side * y_acc) + rlp * scl_div + x_acc, max_side, scl_div + y_inc, scl_div + x_inc);
//			pix_return = get_pixel (in_img_buff, olp * max_side * scl_div + rlp * scl_div + x_acc, max_side, scl_div_fp + y_inc, scl_div_fp + x_inc);
//			pix_return = get_pixel (in_img_buff,(float) (olp * max_side * scl_div_fp) + (float) (rlp * scl_div_fp), max_side, scl_div_fp + y_acc, scl_div_fp + x_acc);
			racc = racc + (pix_return.red_val / ((scl_div + y_inc) * (scl_div + x_inc)));
			gacc = gacc + (pix_return.grn_val / ((scl_div + y_inc) * (scl_div + x_inc)));
			bacc = bacc + (pix_return.blu_val / ((scl_div + y_inc) * (scl_div + x_inc)));
/*printf ("O=%2d, R=%2d, A=%7d, M=%4d, Y=%2d, X=%2d, YA=%7d, XA=%7d\tR=%ld\n", olp, rlp, olp * max_side * scl_div + \
								rlp * scl_div, max_side, \
								scl_div + y_inc, scl_div + x_inc, y_acc, x_acc, racc);*/
			eightbit_thmb_buff [ebo_vals++] = pix_return.red_val / ((scl_div + y_inc) * (scl_div + x_inc));
			eightbit_thmb_buff [ebo_vals++] = pix_return.grn_val / ((scl_div + y_inc) * (scl_div + x_inc));
			eightbit_thmb_buff [ebo_vals++] = pix_return.blu_val / ((scl_div + y_inc) * (scl_div + x_inc));
//			fputc (pix_return.red_val / ((scl_div + y_inc) * (scl_div + x_inc)), out_thumbnail);
//printf ("O=%5d\tR=%2d, G=%2d, R=%2d\n", olp, pix_return.red_val / 1296, pix_return.grn_val / 1296, pix_return.blu_val / 1296);
			x_tot = x_tot + scl_div + x_inc;
			x_inc = 0;
			}
		y_tot = y_tot + scl_div + y_inc;
		y_inc = 0;
//printf ("XTOT=%d\n", x_tot);
		x_acc= 0;
		x_tot= 0;
		}
	}
//printf ("YTOT=%d\n", y_tot);

printf ("R=%ld, G=%ld, B=%ld\tSM=%d\n", racc, gacc, bacc, scl_mod);
printf ("OP=%d\n", ebo_vals);

for (olp = 0;olp < ebo_vals; olp += 12)
	{
	for (rlp = 0;rlp < 12;rlp++)
		{
		twelve_vals [rlp] = eightbit_thmb_buff [olp + rlp];
//printf ("O=%d R=%d T=%d\n", olp, rlp, twelve_vals [rlp]);
		}
	strcpy (nine_byte_chunk, twelveight_to_nine_six (twelve_vals));
	for (pos = 0;pos < 9;pos++)
		{
		thmb_buff [thmb_vals++] = nine_byte_chunk [pos];
//		fputc (nine_byte_chunk [pos], out_thumbnail);	// thmb_vals
//printf ("p=%d c=%d   ", pos, nine_byte_chunk [pos]);
		}
//printf ("\n");
	}
wr_items = fwrite (thmb_buff, THUMBNAIL_BYTES, 1, out_thumbnail);

fclose (out_thumbnail);


for (olp = 0; olp < 4; olp += 2)
	{
	for (hlp = 1; hlp < 33; hlp++)
		{
		for (llp = 0; llp < 2; llp++)
			{
			for (qlp = 1; qlp < 9; qlp++)
				{
				for (pos = 0; pos < 9 ; pos++)
					{
//printf ("O=%d H=%d L=%d, Q=%d, P=%d\n", olp, hlp, llp, qlp, pos);
					nine_byte_chunk [pos] = thmb_buff [lp];
					lp++;
					}
				lp = lp - 1;
				rgb_return = get_nine_six (nine_byte_chunk);
				quad_accum[olp + llp].red_val = quad_accum[olp + llp].red_val + rgb_return.red_val;
				quad_accum[olp + llp].grn_val = quad_accum[olp + llp].grn_val + rgb_return.grn_val;
				quad_accum[olp + llp].blu_val = quad_accum[olp + llp].blu_val + rgb_return.blu_val;
				quad_accum[olp + llp].gry_val = quad_accum[olp + llp].gry_val + (rgb_return.red_val + rgb_return.grn_val + rgb_return.blu_val) / 3;
				base_sixfour [0] = '0';
				}	// end qlp
			}	// end llp
		}	// end hlp
	}	// end olp



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
	red_dec = quad_accum[olp].red_val / DECIMAL_DIVIDER;
	grn_dec = quad_accum[olp].grn_val / DECIMAL_DIVIDER;
	blu_dec = quad_accum[olp].blu_val / DECIMAL_DIVIDER;
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
	base_sixfour [0] = '0';
	hue_print [olp] = base_sixfour [(int) hue_value];
	base_sixfour [0] = '0';
	gry_print [olp] = base_sixfour [(int) quad_accum [olp].gry_val];
	}
mag_n = (powf ((float) (width * height), EXPONENT) / DIVIDER) - SUBTRACTOR;
if (mag_n < 0)
	{
	mag_n = 0;
	}

printf ("%s %s %c\n", gry_print, hue_print, base_sixfour [mag_n]);

return 0;
}



/* * * * * * * * * * * * * * *
 *                           *
 *     thumbprint.c 0.35     *
 *                           *
 *     2025-10-18            *
 *                           *
 *     Zax                   *
 *                           *
 * * * * * * * * * * * * * * 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/stat.h>
#include "TMLib.h"
#include "print.h"
#include "six_sixfour.h"

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
char cmd_line [FILELINE_LENGTH] = NULL_STRING;
char file_name [FILENAME_LENGTH] = NULL_STRING;
char file_path [FILELINE_LENGTH] = NULL_STRING;
char out_path [FILELINE_LENGTH] = NULL_STRING;
unsigned char nine_byte_chunk [9];
unsigned char *thmb_buff;
unsigned char base_sixfour [65] = BASE_SIXTYFOUR;
char mag_string [MAG_SUM_LEN];
char hue_print [5] = NULL_STRING;
char gry_print [5] = NULL_STRING;

int lp = 0;
int qlp, llp, hlp, olp, rd_err, pos, n_chrs;
int wr_items = 0;

float hue_value, red_dec, grn_dec, blu_dec, mag_n;

//thmb_buff = (unsigned char *) calloc (1, THUMBNAIL_BYTES + 1);
//rd_err = fread (thmb_buff, 1, THUMBNAIL_BYTES, rgb_thumbnail);

for (olp = 0; olp < 4; olp += 2)
	{
	for (hlp = 1; hlp < 33; hlp++)
		{
		for (llp = 0; llp < 2; llp++)
			{
			for (qlp = 1; qlp < 9; qlp++)
				{
				for (pos = 0; pos < 9 ; pos++)
					{
					nine_byte_chunk [pos] = thmb_buff [lp];
					lp++;
					}
				lp = lp - 1;
				rgb_return = get_nine_six (nine_byte_chunk);
				quad_accum[olp + llp].red_val = quad_accum[olp + llp].red_val + rgb_return.red_val;
				quad_accum[olp + llp].grn_val = quad_accum[olp + llp].grn_val + rgb_return.grn_val;
				quad_accum[olp + llp].blu_val = quad_accum[olp + llp].blu_val + rgb_return.blu_val;
				quad_accum[olp + llp].gry_val = quad_accum[olp + llp].gry_val + (rgb_return.red_val + rgb_return.grn_val + rgb_return.blu_val) / 3;
				base_sixfour [0] = '0';
				}	// end qlp
			}	// end llp
		}	// end hlp
	}	// end olp

if (tpflags.tprt == SW_ON)
	{
	if (strrchr (img_name, 47))
		{
		strcpy (file_name, strrchr (img_name, 47) + 1);
		strncpy (file_path, img_name, strrchr (img_name, 47) - img_name + 1);
		n_chrs = snprintf (out_path, FILELINE_LENGTH, "%s%s", file_path, "s/");
		n_chrs = snprintf (out_name, FILENAME_LENGTH, "%s%s%s", out_path, file_name, FILE_EXTN);
		}
		else
		{
		strcpy (out_path, "s/");
		strcat (out_name, img_name);
		n_chrs = snprintf (out_name, FILENAME_LENGTH, "%s%s%s%s", file_path, "s/", img_name, FILE_EXTN);
		}
	if (stat (out_path, &sb) == -1)
		{
		mkdir (out_path, 0700);
		}
	out_thumbnail = fopen (out_name, WRITE_BINARY);
	wr_items = fwrite (thmb_buff, THUMBNAIL_BYTES, 1, out_thumbnail);
	fclose (out_thumbnail);
	}

fclose (rgb_thumbnail);
free (thmb_buff);

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
	red_dec = quad_accum[olp].red_val / DECIMAL_DIVIDER;
	grn_dec = quad_accum[olp].grn_val / DECIMAL_DIVIDER;
	blu_dec = quad_accum[olp].blu_val / DECIMAL_DIVIDER;
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
	base_sixfour [0] = '0';
	hue_print [olp] = base_sixfour [(int) hue_value];
	base_sixfour [0] = '0';
	gry_print[olp] = base_sixfour [(int) quad_accum[olp].gry_val];
	}
base_sixfour [0] = '0';

mag_n = (powf ((float) (width * height), EXPONENT) / DIVIDER) - SUBTRACTOR;
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
}
*/
