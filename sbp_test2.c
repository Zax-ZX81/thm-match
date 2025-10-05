/* * * * * * * * * * * * * *
 *                         *
 *    six bit pixel test   *
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
#include <sys/types.h>
#include <sys/stat.h>

#define PROG_VERSION "0.10"

int main (int argc, char *argv [])

{
struct rgb_accumulator rgb_return;
struct rgb_accumulator rgb_accum;

char nine_byte_string [9];

int lp;

FILE* rgb_thumbnail = fopen("Nomads.jpg.rgb", "rb");

if (rgb_thumbnail == NULL)
	{
	perror("Error opening file");
	return 1;
	}
for (lp = 1; lp < 9; lp++)
	{
	fread (nine_byte_string, 9, 1, rgb_thumbnail);
	rgb_return = get_nine_six (nine_byte_string);
	rgb_accum.red_val = rgb_accum.red_val + rgb_return.red_val;
	rgb_accum.grn_val = rgb_accum.grn_val + rgb_return.grn_val;
	rgb_accum.blu_val = rgb_accum.blu_val + rgb_return.blu_val;
	printf ("%d\tRed total: %f\tGreen total: %f\tBlue total: %f\n", lp, rgb_accum.red_val, rgb_accum.grn_val, rgb_accum.blu_val);
	}
rgb_accum.red_val = rgb_accum.red_val / 8;
rgb_accum.grn_val = rgb_accum.grn_val / 8;
rgb_accum.blu_val = rgb_accum.blu_val / 8;
printf ("\tRed average: %f\tGreen average: %f\tBlue average: %f\n", rgb_accum.red_val, rgb_accum.grn_val, rgb_accum.blu_val);

fclose(rgb_thumbnail);
}



/*
https://www.geeksforgeeks.org/c/how-to-read-struct-from-binary-file-in-c/


FILE* rgb_thumbnail = fopen("Nomads.jpg.rgb", "rb");

if (rgb_thumbnail == NULL) {
	perror("Error opening file");
	return 1;
	}
while (fread(&employee, sizeof(Employee), 1, file)
           == 1) {
        // Process the read data (e.g., print or manipulate)
        printf("Employee ID: %d, Name: %s, Salary: %.2f\n",
               employee.id, employee.name, employee.salary);
    }
    // close the file
    fclose(file);

}

*/
