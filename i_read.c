#include <stdio.h>
#include <math.h>
#include "TMLib.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

struct rgb_return get_pixel (unsigned char *data, int anch, int width, int y_max, int x_max);

int main (int argc, char *argv [])
{
int width, height, comp;
int rlp, olp, wid_div, wid_mod;
long racc = 0, gacc = 0, bacc = 0;
char img_name [FILENAME_LENGTH] = NULL_STRING;
unsigned char *data;
struct rgb_return pix_return;

strncpy (img_name, argv [1], FILENAME_LENGTH);
data = stbi_load (img_name, &width, &height, &comp, 0);
FILE *out_thumbnail;
out_thumbnail = fopen ("p.rgb", WRITE_BINARY);
wid_div = width / 64;
wid_mod = width % 64;
/*###############################################*/
if (!wid_mod)
	{
	printf ("Clean divide\n");
	for (olp = 0; olp < 64; olp++)
		{
		for (rlp = 0; rlp < 64; rlp++)
			{
			pix_return = get_pixel (data, (olp * (width * 3) * wid_div) + (rlp * wid_div * 3), width, wid_div, wid_div);
			racc = racc + pix_return.red_val;
			gacc = gacc + pix_return.grn_val;
			bacc = bacc + pix_return.blu_val;
			fputc (pix_return.red_val / pow (wid_div, 2), out_thumbnail);
			fputc (pix_return.grn_val / pow (wid_div, 2), out_thumbnail);
			fputc (pix_return.blu_val / pow (wid_div, 2), out_thumbnail);
//printf ("O=%5d\tR=%2d, G=%2d, R=%2d\n", olp, pix_return.red_val / 1296, pix_return.grn_val / 1296, pix_return.blu_val / 1296);
			}
		}
	}
	else
	{
/*###############################################*/
/*###############################################*/
	printf ("Remainder\n");
	for (olp = 0; olp < 63; olp++)
		{
		for (rlp = 0; rlp < 63; rlp++)
			{
			pix_return = get_pixel (data, (olp * (width * 3) * wid_div) + (rlp * wid_div * 3), width, wid_div, wid_div);
			racc = racc + pix_return.red_val;
			gacc = gacc + pix_return.grn_val;
			bacc = bacc + pix_return.blu_val;
			fputc (pix_return.red_val / pow (wid_div, 2), out_thumbnail);
			fputc (pix_return.grn_val / pow (wid_div, 2), out_thumbnail);
			fputc (pix_return.blu_val / pow (wid_div, 2), out_thumbnail);
//printf ("O=%5d\tR=%2d, G=%2d, R=%2d\n", olp, pix_return.red_val / 1296, pix_return.grn_val / 1296, pix_return.blu_val / 1296);
			}
		pix_return = get_pixel (data, (olp * (width * 3) * wid_div) + (63 * wid_div * 3), width, wid_div, wid_div + wid_mod);
		racc = racc + pix_return.red_val;
		gacc = gacc + pix_return.grn_val;
		bacc = bacc + pix_return.blu_val;
		fputc (pix_return.red_val / (wid_div * (wid_div + wid_mod)), out_thumbnail);
		fputc (pix_return.grn_val / (wid_div * (wid_div + wid_mod)), out_thumbnail);
		fputc (pix_return.blu_val / (wid_div * (wid_div + wid_mod)), out_thumbnail);
		}
	for (rlp = 0; rlp < 63; rlp++)
		{
		pix_return = get_pixel (data, (63 * (width * 3) * wid_div) + (rlp * wid_div * 3), width, wid_div + wid_mod, wid_div);
		racc = racc + pix_return.red_val;
		gacc = gacc + pix_return.grn_val;
		bacc = bacc + pix_return.blu_val;
		fputc (pix_return.red_val / (wid_div * (wid_div + wid_mod)), out_thumbnail);
		fputc (pix_return.grn_val / (wid_div * (wid_div + wid_mod)), out_thumbnail);
		fputc (pix_return.blu_val / (wid_div * (wid_div + wid_mod)), out_thumbnail);
//printf ("O=%5d\tR=%2d, G=%2d, R=%2d\n", olp, pix_return.red_val / 1296, pix_return.grn_val / 1296, pix_return.blu_val / 1296);
		}
	pix_return = get_pixel (data, (63 * (width * 3) * wid_div) + (63 * wid_div * 3), width, wid_div + wid_mod, wid_div + wid_mod);
	printf ("R=%d, G=%d, B=%d\n", pix_return.red_val, pix_return.grn_val, pix_return.blu_val);
	fputc (pix_return.red_val / ((wid_div + wid_mod) * (wid_div + wid_mod)), out_thumbnail);
	fputc (pix_return.grn_val / ((wid_div + wid_mod) * (wid_div + wid_mod)), out_thumbnail);
	fputc (pix_return.blu_val / ((wid_div + wid_mod) * (wid_div + wid_mod)), out_thumbnail);
	}
/*###############################################*/

printf ("R=%2ld, G=%2ld, R=%2ld\tWM=%d\n", racc, gacc, bacc, wid_mod);

fclose (out_thumbnail);
return 0;
}

struct rgb_return get_pixel (unsigned char *data, int anch, int width, int y_max, int x_max)
{
int yl, xl, px, rc = 0, gc = 0, bc = 0;
struct rgb_return pix_return;

for (yl = 0; yl < y_max; yl++)
	{
	for (xl = 0; xl < x_max; xl++)
		{
		px = anch + (yl * (width * 3)) + (xl * 3);
		rc = rc + data [px];
		gc = gc + data [px + 1];
		bc = bc + data [px + 2];
		//printf ("%2x%2x%2x ", rp, gp, bp);
//		printf ("Y=%2d   X=%2d   %2x %2x %2x\n", yl, xl, rc, gc, bc);
		}
	}
pix_return.red_val = rc;
pix_return.grn_val = gc;
pix_return.blu_val = bc;

return (pix_return);
}
