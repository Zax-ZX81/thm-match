/* * * * * * * * * * * * * *
 *                         *
 *    filename seperator   *
 *    test                 *
 *                         *
 *       2025-10-11        *
 *                         *
 *       Zax               *
 *                         *
 * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "TMLib.h"
#include <sys/types.h>
#include <sys/stat.h>

#define PROG_VERSION "0.10"

int main (int argc, char *argv [])

{
struct file_name_return file_return;

char thm_filename [FILENAME_LENGTH];

int pos, wid, hig;
int digit = 1;

strcpy (thm_filename, argv [FILE_ARG - 1]);
pos = strlen (thm_filename) - 1;
if (!(pos > 4 && thm_filename [pos] == 'b' && thm_filename [pos -1] == 'g' && thm_filename [pos - 2] == 'r' && thm_filename [pos - 3] == '.'))
	{
	strcpy (file_return.name, "ERROR");
	exit (0);
	}
pos = pos - 4;

while (pos && thm_filename[pos] != 'x')
	{
//	printf ("%d\t%c\n", pos, thm_filename[pos]);
	switch (digit)
		{
		case 1:
			wid = thm_filename[pos--] - 48;
			break;
		case 2:
			wid = (thm_filename[pos--] - 48) * 10;
			break;
		case 3:
			wid = (thm_filename[pos--] - 48) * 100;
			break;
		case 4:
			wid = (thm_filename[pos--] - 48) * 1000;
			break;
		case 5:
			wid = (thm_filename[pos--] - 48) * 10000;
			break;
		}
	digit++;
	}
digit = 1;
pos--;
while (pos && thm_filename[pos] != '_')
	{
//	printf ("%d\t%c\n", pos, thm_filename[pos]);
	switch (digit)
		{
		case 1:
			hig = thm_filename[pos--] - 48;
			break;
		case 2:
			hig = (thm_filename[pos--] - 48) * 10;
			break;
		case 3:
			hig = (thm_filename[pos--] - 48) * 100;
			break;
		case 4:
			hig = (thm_filename[pos--] - 48) * 1000;
			break;
		case 5:
			hig = (thm_filename[pos--] - 48) * 10000;
			break;
		}
	digit++;
	}
pos--;
thm_filename [pos + 1] = '\0';
strcpy (file_return.name, thm_filename);
file_return.width = wid;
file_return.height = hig;
printf ("%s\tW=%d\tH=%d\n", thm_filename, wid, hig);
printf ("%s\tW=%d\tH=%d\n", file_return.name, file_return.width, file_return.height);
}
