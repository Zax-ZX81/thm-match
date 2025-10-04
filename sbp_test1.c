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

struct four_six_bit_pixels fsbp;
struct rgb_accumulator rgb_return;
//unsigned char sb_a, sb_b, sb_c, fsbp.red_b, fsbp.grn_b, fsbp.blu_b, fsbp.red_c, fsbp.grn_c, fsbp.blu_c, fsbp.red_d, fsbp.grn_d, fsbp.blu_d;
unsigned char tmpa, tmpb;
char nine_byte_string [9] = "aBcDeFgH0";
float red_avg, grn_avg, blu_avg;

strcpy (fsbp, nine_byte_string);

/*tmpa = nine_byte_string [0];
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
*/
rgb_return.red_val = (float) (fsbp.red_a + fsbp.red_b + fsbp.red_c + fsbp.red_d)/4;
rgb_return.grn_val = (float) (fsbp.grn_a + fsbp.grn_b + fsbp.grn_c + fsbp.grn_d)/4;
rgb_return.blu_val = (float) (fsbp.blu_a + fsbp.blu_b + fsbp.blu_c + fsbp.blu_d)/4;

printf ("%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\n", fsbp.red_a, fsbp.grn_a, fsbp.blu_a, fsbp.red_b, fsbp.grn_b, fsbp.blu_b, fsbp.red_c, fsbp.grn_c, fsbp.blu_c, fsbp.red_d, fsbp.grn_d, fsbp.blu_d);
printf ("Red average: %f, Green average: %f, Blue average: %f\n", rgb_return.red_val, rgb_return.grn_val, rgb_return.blu_val);

printf ("Size of FSBP: %lu	%lu", sizeof(fsbp), sizeof(fsbp.red_a));


//printf ("24	20	9	35	17	6	21	6	25	52	32	48\n");

}
/*
https://www.geeksforgeeks.org/c/how-to-read-struct-from-binary-file-in-c/


FILE* file = fopen("employee_data.bin", "rb");

if (file == NULL) {
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


char *getninesix (char *dblock)  // Encapsulate a string in double quotes
{
char *out_string = malloc (FILELINE_LENGTH);
char quote_marks [] = {'"', '\0'};

strcpy (out_string, quote_marks);
strcat (out_string, filepath);
strcat (out_string, quote_marks);

return out_string;
}


{
*/
