#include <stdio.h>
#include <math.h>
#include "TMLib.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

struct rgb_return get_pixel (unsigned char *data, int anch, int width, int y_max, int x_max);

int main (void)
{
int width, height, comp;
int rlp, olp, wid_mod;
long racc = 0, gacc = 0, bacc = 0;
unsigned char *data = stbi_load("img/PJH.png", &width, &height, &comp, 0);
struct rgb_return pix_return;

FILE *out_thumbnail;
out_thumbnail = fopen ("img/P.rgb", WRITE_BINARY);
wid_mod = width % 64;
for (olp = 0; olp < 64; olp++)
	{
	for (rlp = 0; rlp < 63; rlp++)
		{
		pix_return = get_pixel (data, (olp * (width * 3) * (width / 64)) + (rlp * (width / 64) * 3), width, width / 64, width / 64);
		racc = racc + pix_return.red_val;
		gacc = gacc + pix_return.grn_val;
		bacc = bacc + pix_return.blu_val;
		fputc (pix_return.red_val / (int) pow (width / 64, 2), out_thumbnail);
		fputc (pix_return.grn_val / (int) pow (width / 64, 2), out_thumbnail);
		fputc (pix_return.blu_val / (int) pow (width / 64, 2), out_thumbnail);
//printf ("O=%5d\tR=%2d, G=%2d, R=%2d\n", olp, pix_return.red_val / 1296, pix_return.grn_val / 1296, pix_return.blu_val / 1296);
		}
	pix_return = get_pixel (data, (olp * (width * 3) * (width / 64)) + (63 * (width / 64) * 3), width, width / 64, width / 64 + wid_mod);
	racc = racc + pix_return.red_val;
	gacc = gacc + pix_return.grn_val;
	bacc = bacc + pix_return.blu_val;
	fputc (pix_return.red_val / (width / 64 * (width / 64 + wid_mod)), out_thumbnail);
	fputc (pix_return.grn_val / (width / 64 * (width / 64 + wid_mod)), out_thumbnail);
	fputc (pix_return.blu_val / (width / 64 * (width / 64 + wid_mod)), out_thumbnail);
	}
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
