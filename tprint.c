/* * * * * * * * * * * * * *
 *                         *
 *       tprint 0.30       *
 *                         *
 *       2025-10-08        *
 *                         *
 *       Zax               *
 *                         *
 * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TMLib.h"
#include "thumbprint.h"

#define PROG_NAME "Thumbprint"
#define PROG_VERSION "0.30"

struct tprint_flags tpflags [1] = {0};

int main (int argc, char *argv [])

{
struct tprint_database tprint_return;

char img_name [FILENAME_LENGTH] = NULL_STRING;
char switch_chr;

int arg_no, switch_pos;

tpflags->tprt = FALSE;
tpflags->verbose = FALSE;

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
					tpflags->tprt = SW_ON;
					break;
				case 'v':
					tpflags->verbose = SW_ON;
					break;
				case 'V':
					printf ("%s version %s\n", PROG_NAME, PROG_VERSION);
					exit (0);
				default:
					printf ("%s# Thumbprint [pstvV] <image file>%s\n", TEXT_YELLOW, TEXT_RESET);
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
tprint_return = thumbprint (img_name);

printf ("%s\t%s\t%c\t%s\n", tprint_return.gry_print, tprint_return.hue_print, tprint_return.magnitude [0], tprint_return.filepath);
//nn_len = snprintf (new_name, FILENAME_LENGTH, "%s_%s%s%c%s", img_name, gry_print, hue_print, base_sixfour [(int) mag_n], FILE_EXTN);
//printf ("%s\t%s\n", img_name, new_name);
}
