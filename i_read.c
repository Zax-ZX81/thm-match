#include <stdio.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//const size_t NUM_PIXELS_TO_PRINT = 192;

int main (void)
{
int width, height, comp;
int xl, yl, cl, rp, gp, bp;
long rc = 0, gc = 0, bc = 0;
unsigned char *data = stbi_load("img/PJH.png", &width, &height, &comp, 0);

for (yl = 0; yl < 18; yl++)
	{
	for (xl = 0; xl < 18; xl++)
		{
		rp = data [(yl * (width * 3)) + (xl * 3)];
		gp = data [(yl * (width * 3)) + (xl * 3) + 1];
		bp = data [(yl * (width * 3)) + (xl * 3) + 2];
		rc = rc + rp;
		gc = gc + gp;
		bc = bc + bp;
		printf ("%2x%2x%2x ", rp, gp, bp);
		}
	printf ("\n");
	}
printf ("R=%ld, G=%ld, R=%ld\n", rc / 1296, gc / 1296, bc / 1296);
/*if (data)
	{
	printf ("height = %d, width = %d, comp = %d (channels), rows = %d, columns = %d\n", height, width, comp, height / 64, width / 64);
	for (size_t i = 0; i < NUM_PIXELS_TO_PRINT * comp; i++)
		{
		if ((i + 1) % 4 != 0)
			{
			px++;
			printf ("%02x%s", data [i], px % 3 ? "" : " ");
			if (px % 96 == 0)
				{
				printf ("_%d\n", px);
				}
			}
		}
	}*/
return 0;
}
