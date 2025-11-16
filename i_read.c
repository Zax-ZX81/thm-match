#include <stdio.h>
#include <math.h>
#include "TMLib.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

struct rgb_return get_pixel (unsigned char *i_buff, int anch, int width, int y_max, int x_max);

int main (int argc, char *argv [])
{
int width, height, comp;
int rlp, olp, scl_div, scl_mod, pix_cnt, max_side, y_scl_rmd, x_scl_rmd;
int pix_ofs = 0, ir_pos = 0, y_inc = 0, x_inc = 0;
long racc = 0, gacc = 0, bacc = 0;
float scl_div_fp;
char img_name [FILENAME_LENGTH] = NULL_STRING;
unsigned char *img_raw, *i_buff;
char aspect = 0;
struct rgb_return pix_return;

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
printf ("W=%d, H=%d, C=%d, A=%d, PC=%d, SD=%d, SM=%d, Sf=%5.3f\n", width, height, comp, aspect, pix_cnt, scl_div, scl_mod, scl_div_fp);
i_buff = (unsigned char *) calloc (pix_cnt, 3);
if (aspect != SQUARE)
	{
printf ("Padding %d\n", pix_cnt * 3);
	for (olp = 0;olp < (pix_cnt * 3);olp++)
		{
		i_buff [olp] = 127;
		}
	}
switch (aspect)
	{
	case PORTRAIT:
		pix_ofs = (height - width) / 2;
		printf ("Portrait, %d, %d\n", pix_ofs, max_side - pix_ofs);
		for (olp = 0;olp < height;olp++)
			{
			for (rlp = pix_ofs * 3;rlp < (max_side - pix_ofs) * 3;rlp++)
				{
				i_buff [(olp * height * 3) + rlp] = img_raw [ir_pos++];
				}
			}
		break;
	case LANDSC:
		pix_ofs = (width - height) / 2;
		printf ("Landscape, %d\n", pix_ofs);
		for (olp = (width * pix_ofs * 3);olp < width * width * 3 - (width * pix_ofs * 3);olp++)
			{
//printf ("W=%d, H=%d, PO=%d, L=%d\n", width, height, pix_ofs, olp);
			i_buff [olp] = img_raw [ir_pos++];
			}
		break;
	case SQUARE:
		printf ("Square\n");
		for (olp = 0;olp < (pix_cnt * 3);olp++)
			{
			i_buff [olp] = img_raw [ir_pos++];
			}
		break;
	}
printf ("W=%d, H=%d, C=%d, A=%d\n", width, height, comp, aspect);

if (!scl_mod)
	{
	printf ("Clean divide\n");
	for (olp = 0; olp < 64; olp++)
		{
		for (rlp = 0; rlp < 64; rlp++)
			{
			pix_return = get_pixel (i_buff, (olp * (max_side * 3) * scl_div) + (rlp * scl_div * 3), max_side, scl_div, scl_div);
			racc = racc + pix_return.red_val;
			gacc = gacc + pix_return.grn_val;
			bacc = bacc + pix_return.blu_val;
			fputc (pix_return.red_val / pow (scl_div, 2), out_thumbnail);
			fputc (pix_return.grn_val / pow (scl_div, 2), out_thumbnail);
			fputc (pix_return.blu_val / pow (scl_div, 2), out_thumbnail);
//printf ("OL=%2d\tRL=%2d\tR=%2d, G=%2d, R=%2d\n", olp, rlp, pix_return.red_val / 1296, pix_return.grn_val / 1296, pix_return.blu_val / 1296);
			}
		}
	}
	else
	{
	printf ("Remainder\n");
	for (olp = 0; olp < 64; olp++)
		{
		if ((float) (scl_div_fp * olp) > (float) (scl_div * olp + 0.5 + y_inc))
			{
printf ("y\n");
			y_inc++;
			y_scl_rmd--;
			}
		for (rlp = 0; rlp < 64; rlp++)
			{
			if (((float) scl_div_fp * rlp) > (float) (scl_div * rlp + 0.5 + x_inc))
				{
printf ("x\n");
				x_inc++;
				x_scl_rmd--;
				}
			pix_return = get_pixel (i_buff, (olp * (max_side * 3) * scl_div) + (rlp * scl_div * 3), max_side, scl_div + y_inc, scl_div + x_inc);
printf ("Of=%5.3f, Oi=%5.3f, Rf=%5.3f, Ri=%5.3f\n", (float) (scl_div_fp * olp), \
						(float) (scl_div * olp + 0.5 + y_inc), \
						(float) (scl_div_fp * rlp), \
						(float) (scl_div * rlp + 0.5 + x_inc));
printf ("O=%2d, R=%2d, A=%7d, M=%5d, Y=%3d, X=%3d\n", olp, rlp, (olp * (max_side * 3) * scl_div) + (rlp * scl_div * 3), max_side, scl_div + y_inc, scl_div + x_inc);
			racc = racc + pix_return.red_val;
			gacc = gacc + pix_return.grn_val;
			bacc = bacc + pix_return.blu_val;
			fputc (pix_return.red_val / pow (scl_div, 2), out_thumbnail);
			fputc (pix_return.grn_val / pow (scl_div, 2), out_thumbnail);
			fputc (pix_return.blu_val / pow (scl_div, 2), out_thumbnail);
//printf ("O=%5d\tR=%2d, G=%2d, R=%2d\n", olp, pix_return.red_val / 1296, pix_return.grn_val / 1296, pix_return.blu_val / 1296);
			}
		}
	}

printf ("R=%2ld, G=%2ld, R=%2ld\tWM=%d\n", racc, gacc, bacc, scl_mod);

fclose (out_thumbnail);
return 0;
}

struct rgb_return get_pixel (unsigned char *i_buff, int anch, int width, int y_max, int x_max)
{
int yl, xl, px, rc = 0, gc = 0, bc = 0;
struct rgb_return pix_return;

for (yl = 0; yl < y_max; yl++)
	{
	for (xl = 0; xl < x_max; xl++)
		{
		px = anch + (yl * (width * 3)) + (xl * 3);
		rc = rc + i_buff [px];
		gc = gc + i_buff [px + 1];
		bc = bc + i_buff [px + 2];
		//printf ("%2x%2x%2x ", rp, gp, bp);
//		printf ("Y=%2d   X=%2d   %2x %2x %2x\n", yl, xl, rc, gc, bc);
		}
	}
pix_return.red_val = rc;
pix_return.grn_val = gc;
pix_return.blu_val = bc;

return (pix_return);
}
