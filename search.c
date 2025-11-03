/* * * * * * * * * * * * * *
 *                         *
 *       search.c 0.35     *
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
#include "six_sixfour.h"


int exact_search (char *srch_prnt, char *tgt_prnt)
{
//unsigned char base_sixfour [65] = BASE_SIXTYFOUR;

int pos = 0;
int ret_code = 0;

//printf ("Exact search\n");
while (pos < 4)
	{
	if (srch_prnt [pos] == tgt_prnt [pos])
		{
		ret_code++;
		}
//printf ("Exact\tP=%d\tS=%c\tT=%c\tR=%d\n", pos, srch_prnt [pos], tgt_prnt [pos], ret_code);
	pos++;
	}
//printf ("R=%d\n", ret_code);
if (ret_code > 3)
	{
	return (1);
	}

return (FALSE);
}


int fuzz_search (char *srch_prnt, char *tgt_prnt)
{
//unsigned char base_sixfour [65] = BASE_SIXTYFOUR;

int pos = 0;
int ret_code = 0;
int t_val, s_val;
int exact = TRUE;

//printf ("Fuzz search\n");
while (pos < 4)
	{
	s_val = sixfour_to_dec (srch_prnt [pos]);
	t_val = sixfour_to_dec (tgt_prnt [pos]);
//printf ("S=%d\tT=%d\n", s_val, t_val);
	if (s_val == t_val)
		{
		ret_code++;
		}
	if (s_val < 63 && s_val + 1 == t_val || s_val > 0 && s_val - 1 == t_val)
		{
		ret_code++;
		exact = FALSE;
		}
/*printf ("Fuzzy\tP=%d\tS=%c\tB=%c\tA=%c\tT=%c\tR=%d\t", pos, base_sixfour [sixfour_to_dec (srch_prnt [pos])], \
							base_sixfour [s_val - 1], \
							base_sixfour [s_val + 1], \
							tgt_prnt [pos], ret_code);*/
//printf ("lt63=%d\t+1=%d\tgt0=%d\t-1=%d\n", s_val < 63, s_val + 1 == t_val, s_val > 0, s_val - 1 == t_val);
//printf ("Fuzzy\tP=%d\tt=%d\tb=%d\ta=%d\ts=%d\tR=%d\n", pos, t_val, t_val - 1, t_val + 1, \
//							s_val, ret_code);
	pos++;
	}
//printf ("R=%d\n", ret_code);
if (ret_code > 3)
	{
	if (exact)
		{
		return (1);
		}
		else
		{
		return (2);
		}
	}

return (FALSE);
}
