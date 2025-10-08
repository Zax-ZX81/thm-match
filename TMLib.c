/* * * * * * * * * * * * * *
 *                         *
 *       TMLib.c 0.10      *
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


void exit_error (char *message_a, char *message_b)	// Print two strings as error messages in orange
{
fprintf (stderr, "%s%s%s%s\n", TEXT_ORANGE, message_a, message_b, TEXT_RESET);
exit (1);
}


struct rgb_accumulator get_nine_six (char *nine_byte_string)  // Extract 12 6 bit values from 9 bytes
{
struct four_six_bit_pixels fsbp;
struct rgb_accumulator rgb_return;

unsigned char tmpa, tmpb;

tmpa = nine_byte_string [0];
fsbp.red_a = (tmpa >> 2);

tmpa = nine_byte_string [0];
tmpb = (tmpa << 6);
fsbp.grn_a = ((tmpb >> 2) & ~15) | 0;
tmpa = nine_byte_string [1];
tmpb = (tmpa >> 4);
fsbp.grn_a = fsbp.grn_a + tmpb;

tmpa = nine_byte_string [1];
tmpb = (tmpa << 4);
fsbp.blu_a = (tmpb >> 2);
tmpa = nine_byte_string [2];
tmpb = (tmpa >> 6);
fsbp.blu_a = fsbp.blu_a + tmpb;

tmpa = nine_byte_string [2];
tmpb = (tmpa << 2);
fsbp.red_b = (tmpb >> 2);

tmpa = nine_byte_string [3];
fsbp.grn_b = (tmpa >> 2);

tmpa = nine_byte_string [3];
tmpb = (tmpa << 6);
fsbp.blu_b = ((tmpb >> 2) & ~15) | 0;
tmpa = nine_byte_string [4];
tmpb = (tmpa >> 4);
fsbp.blu_b = fsbp.blu_b + tmpb;

tmpa = nine_byte_string [4];
tmpb = (tmpa << 4);
fsbp.red_c = (tmpb >> 2);
tmpa = nine_byte_string [5];
tmpb = (tmpa >> 6);
fsbp.red_c = fsbp.red_c + tmpb;

tmpa = nine_byte_string [5];
tmpb = (tmpa << 2);
fsbp.grn_c = (tmpb >> 2);

tmpa = nine_byte_string [6];
fsbp.blu_c = (tmpa >> 2);

tmpa = nine_byte_string [6];
tmpb = (tmpa << 6);
fsbp.red_d = ((tmpb >> 2) & ~15) | 0;
tmpa = nine_byte_string [7];
tmpb = (tmpa >> 4);
fsbp.red_d = fsbp.red_d + tmpb;

tmpa = nine_byte_string [7];
tmpb = (tmpa << 4);
fsbp.grn_d = (tmpb >> 2);
tmpa = nine_byte_string [8];
tmpb = (tmpa >> 6);
fsbp.grn_d = fsbp.grn_d + tmpb;

tmpa = nine_byte_string [8];
tmpb = (tmpa << 2);
fsbp.blu_d = (tmpb >> 2);

//printf ("\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", fsbp.red_a, fsbp.grn_a, fsbp.blu_a, fsbp.red_b, fsbp.grn_b, fsbp.blu_b, fsbp.red_c, fsbp.grn_c, fsbp.blu_c, fsbp.red_d, fsbp.grn_d, fsbp.blu_d);

rgb_return.red_val = (float) (fsbp.red_a + fsbp.red_b + fsbp.red_c + fsbp.red_d) / 4;
rgb_return.grn_val = (float) (fsbp.grn_a + fsbp.grn_b + fsbp.grn_c + fsbp.grn_d) / 4;
rgb_return.blu_val = (float) (fsbp.blu_a + fsbp.blu_b + fsbp.blu_c + fsbp.blu_d) / 4;


return rgb_return;
}

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

char *compose_filename (char *img_name, char *img_rename, char *gry_print, char *hue_print)
{
int fn_ep;
char *fn_end;
char mag_sum [16];
char f_name [FILENAME_LENGTH];
char outname [FILENAME_LENGTH];

printf ("1%s\n", img_name);
fn_end = strrchr (img_name, '_');
printf ("2%s\n", img_name);
fn_ep = (int) (fn_end - img_name);
strncpy (f_name, img_name, fn_ep);
printf ("3%s\n", img_name);
f_name [(int) (fn_end - img_name)] = '\0';
printf ("4%s\t%s\n", f_name, img_name);
strncat (img_rename, img_name, strlen (img_name) - 4);
printf ("5%s\t%s\t%s\n", f_name, img_name, img_rename);
strcpy (mag_sum, img_rename);
strcat (img_rename, "_");
printf ("6%s\t%s\t%s\n", f_name, img_name, img_rename);
strcat (img_rename, gry_print);
printf ("7%s\t%s\t%s\n", f_name, img_name, img_rename);
strcat (img_rename, hue_print);
printf ("8%s\t%s\t%s\n", f_name, img_name, img_rename);
strcat (img_rename, FILE_EXTN);
printf ("9%s\t%s\t%s\n", f_name, img_name, img_rename);
printf ("-%s-\n", img_name);
printf ("=%d=\n", fn_ep);
printf (">%s>\n", mag_sum);
printf ("<%s<\n", f_name);
printf ("_%s_\n", fn_end + 1);
//printf ("-%s-\t=%d=\t>%s>\t<%s<\t_%s_\n", img_name, fn_ep, mag_sum, f_name, fn_end + 1);
return img_rename;
}
