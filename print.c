/* * * * * * * * * * * * * *
 *                         *
 *       print.c 0.35      *
 *                         *
 *       2025-10-01        *
 *                         *
 *       Zax               *
 *                         *
 * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TMLib.h"


struct maxmin_return find_limits (float red_value, float grn_value, float blu_value)

{
struct maxmin_return ans;

if ((red_value >= grn_value) && (red_value >= blu_value))
	{
	ans.channel = RED_CHAN;
	ans.max_val = red_value;
	}
	else
	{
	if ((grn_value >= red_value) && (grn_value >= blu_value))
		{
		ans.channel = GRN_CHAN;
		ans.max_val = grn_value;
		}
		else
		{
		ans.channel = BLU_CHAN;
		ans.max_val = blu_value;
		}
	}
if ((red_value <= grn_value) && (red_value <= blu_value))
	{
	ans.min_val = red_value;
	}
if ((grn_value <= red_value) && (grn_value <= blu_value))
	{
	ans.min_val = grn_value;
	}
if ((blu_value <= red_value) && (blu_value <= grn_value))
	{
	ans.min_val = blu_value;
	}
return ans;
}


struct dimension_return separate_magnitude (char *mag_string)

{
struct dimension_return mag_return;

char mag_sum [MAG_SUM_LEN] = NULL_STRING;

int pos, wid, hig;
int digit = 1;

strcpy (mag_sum, mag_string);
pos = strlen (mag_sum) - 1;
//printf ("%s\t%d\t=%c=\n", mag_sum, pos, mag_sum [pos]);

while (pos && mag_sum[pos] != 'x')
	{
//	printf ("%d\t%c\n", pos, mag_sum[pos]);
	switch (digit)
		{
		case 1:
			wid = mag_sum[pos--] - 48;
			break;
		case 2:
			wid = wid + (mag_sum[pos--] - 48) * 10;
			break;
		case 3:
			wid = wid + (mag_sum[pos--] - 48) * 100;
			break;
		case 4:
			wid = wid + (mag_sum[pos--] - 48) * 1000;
			break;
		case 5:
			wid = wid + (mag_sum[pos--] - 48) * 10000;
			break;
		}
	digit++;
	}
digit = 1;
pos--;
while (pos >= 0)
	{
//	printf ("%d\t%c\n", pos, mag_sum[pos]);
	switch (digit)
		{
		case 1:
			hig = mag_sum[pos--] - 48;
			break;
		case 2:
			hig = hig + (mag_sum[pos--] - 48) * 10;
			break;
		case 3:
			hig = hig + (mag_sum[pos--] - 48) * 100;
			break;
		case 4:
			hig = hig + (mag_sum[pos--] - 48) * 1000;
			break;
		case 5:
			hig = hig + (mag_sum[pos--] - 48) * 10000;
			break;
		}
	digit++;
	}
//printf ("=%s=\t%d\n", mag_sum, pos);
mag_return.width = wid;
mag_return.height = hig;

return mag_return;
}
