#include <stdio.h>
#include <math.h>
#include "TMLib.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

char *get_pixel (unsigned char *data, int anch, int width);

int main (void)
{
int width, height, comp;
int xl, yl, rlp, rp, gp, bp, olp, anch, rc = 0, gc = 0, bc = 0, px = 0;
long racc = 0, gacc = 0, bacc = 0;
unsigned char *data = stbi_load("img/PJH.png", &width, &height, &comp, 0);

FILE *out_thumbnail;
out_thumbnail = fopen ("img/P.rgb", WRITE_BINARY);
for (olp = 0; olp < 64; olp++)
	{
	for (rlp = 0; rlp < 64; rlp++)
		{
		get_pixel ((olp * (width * 3) * 18) + (rlp * (width / 64) * 3));
/*		for (yl = 0; yl < width / 63; yl++)
			{
			for (xl = 0; xl < width / 63; xl++)
				{
				anch = (olp * (width * 3) * 18) + (rlp * (width / 64) * 3) + (yl * (width * 3)) + (xl * 3);
				rp = data [anch];
				gp = data [anch + 1];
				bp = data [anch + 2];
				rc = rc + rp;
				gc = gc + gp;
				bc = bc + bp;
				//printf ("%2x%2x%2x ", rp, gp, bp);
				printf ("O=%4d   R=%2d   Y=%2d   X=%2d   A=%5d   %2x %2x %2x   P=%4d\n", olp, rlp, yl, xl, anch, rp, gp, bp, px);
				}
			//printf ("\n");
			}*/
		racc = racc + rc;
		gacc = gacc + gc;
		bacc = bacc + bc;
		px++;
		fputc (rc / 324, out_thumbnail);
		fputc (gc / 324, out_thumbnail);
		fputc (bc / 324, out_thumbnail);
		printf ("O=%5d\tR=%2d, G=%2d, R=%2d\n", olp, rc / 1296, gc / 1296, bc / 1296);
		rc = 0;
		gc = 0;
		bc = 0;
//		printf ("\n");
//	printf ("R=%d, G=%d, R=%d\tO=%d, A=%d\n", rc / 1296, gc / 1296, bc / 1296, olp, anch);
		}
/*	for (yl = 0; yl < width % 63; yl++)
		{
		for (xl = 0; xl < width % 63; xl++)
			{
			anch = (olp * (width * 3) * 18) + (width * 3) + (yl * (width * 3)) + (xl * 3);
			rp = data [anch];
			gp = data [anch + 1];
			bp = data [anch + 2];
			rc = rc + rp;
			gc = gc + gp;
			bc = bc + bp;
			//printf ("%2x%2x%2x ", rp, gp, bp);
			printf ("O=%4d   R=%2d   Y=%2d   X=%2d   A=%5d   %2x %2x %2x   P=%4d   M=%d\n", olp, rlp, yl, xl, anch, rp, gp, bp, px, width % 63);
			}
		//printf ("\n");
		}
	racc = racc + rc;
	gacc = gacc + gc;
	bacc = bacc + bc;
	px++;
	fputc (rc / 1681, out_thumbnail);
	fputc (gc / 1681, out_thumbnail);
	fputc (bc / 1681, out_thumbnail);
	printf ("O=%5d\tR=%2d, G=%2d, R=%2d\n", olp, rc / 6724, gc / 6724, bc / 6724);
	rc = 0;
	gc = 0;
	bc = 0;
//	printf ("\n");
//	printf ("R=%d, G=%d, R=%d\tO=%d, A=%d\n", rc / 1296, gc / 1296, bc / 1296, olp, anch);*/
	}
printf ("R=%2ld, G=%2ld, R=%2ld\n", racc / 5308416, gacc / 5308416, bacc / 5308416);

fclose (out_thumbnail);
return 0;
}

char *get_pixel (unsigned char *data, int anch, int width)
{
int yl, xl, rp, gp, bp, rc, gc, bc, px;

for (yl = 0; yl < width / 63; yl++)
	{
	for (xl = 0; xl < width / 63; xl++)
		{
		px = anch + (yl * (width * 3)) + (xl * 3);
		rp = data [px];
		gp = data [px + 1];
		bp = data [px + 2];
		rc = rc + rp;
		gc = gc + gp;
		bc = bc + bp;
		//printf ("%2x%2x%2x ", rp, gp, bp);
		printf ("Y=%2d   X=%2d   %2x %2x %2x\n", yl, xl, rp, gp, bp);
		}
	//printf ("\n");
	}
}
