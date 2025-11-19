#include <stdio.h>
#include <math.h>
#include "TMLib.h"
#include "six_sixfour.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

struct rgb_return get_pixel (struct rgb_return *in_img_buff, int anch, int width, int y_max, int x_max);

int main (int argc, char *argv [])
{
int width, height, comp;
int rlp, olp, scl_div, scl_mod, pix_cnt, max_side, y_scl_rmd, x_scl_rmd, pos;
int pix_ofs = 0, ir_pos = 0, y_inc = 0, x_inc = 0, y_acc = 0, x_acc = 0, y_tot = 0, x_tot = 0, out_vals = 0;
long racc = 0, gacc = 0, bacc = 0;
float scl_div_fp;
char img_name [FILENAME_LENGTH] = NULL_STRING;
unsigned char *img_raw, *out_img_buff;
unsigned char twelve_vals [13] = NULL_STRING;
unsigned char nine_byte_chunk [9];
char aspect = 0;
struct rgb_return pix_return;
struct rgb_return *in_img_buff;

strncpy (img_name, argv [1], FILENAME_LENGTH);
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
out_img_buff = (unsigned char *) malloc (sizeof (unsigned char) * (THUMBNAIL_PIXELS * 3));
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
			out_img_buff [out_vals++] = pix_return.red_val / (scl_div * scl_div);
			out_img_buff [out_vals++] = pix_return.grn_val / (scl_div * scl_div);
			out_img_buff [out_vals++] = pix_return.blu_val / (scl_div * scl_div);
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
		if ((float) (scl_div_fp * olp) > (float) (scl_div * olp + 0.5 + y_acc) && olp != 63)
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
			out_img_buff [out_vals++] = pix_return.red_val / ((scl_div + y_inc) * (scl_div + x_inc));
			out_img_buff [out_vals++] = pix_return.grn_val / ((scl_div + y_inc) * (scl_div + x_inc));
			out_img_buff [out_vals++] = pix_return.blu_val / ((scl_div + y_inc) * (scl_div + x_inc));
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
printf ("OP=%d\n", out_vals);

for (olp = 0;olp < out_vals; olp += 12)
	{
	for (rlp = 0;rlp < 12;rlp++)
		{
		twelve_vals [rlp] = out_img_buff [olp + rlp];
//printf ("O=%d R=%d T=%d\n", olp, rlp, twelve_vals [rlp]);
		}
	strcpy (nine_byte_chunk, twelveight_to_nine_six (twelve_vals));
	for (pos = 0;pos < 9;pos++)
		{
		fputc (nine_byte_chunk [pos], out_thumbnail);
//printf ("p=%d c=%d   ", pos, nine_byte_chunk [pos]);
		}
//printf ("\n");
	}

fclose (out_thumbnail);
return 0;
}

struct rgb_return get_pixel (struct rgb_return *in_img_buff, int anch, int width, int y_max, int x_max)
{
int yl, xl, px, rc = 0, gc = 0, bc = 0;
struct rgb_return pix_return;

for (yl = 0; yl < y_max; yl++)
	{
	for (xl = 0; xl < x_max; xl++)
		{
		px = anch + (yl * width) + xl;
		rc = rc + in_img_buff [px].red_val;
		gc = gc + in_img_buff [px].grn_val;
		bc = bc + in_img_buff [px].blu_val;
		//printf ("%2x%2x%2x ", rp, gp, bp);
//		printf ("Y=%2d   X=%2d   %2x %2x %2x\n", yl, xl, rc, gc, bc);
		}
	}
//printf ("YM=%d, XM=%d, PixCnt=%d\n", y_max, x_max, y_max * x_max);
pix_return.red_val = rc;
pix_return.grn_val = gc;
pix_return.blu_val = bc;

return (pix_return);
}
