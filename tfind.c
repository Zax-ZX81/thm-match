/* * * * * * * * * * * * * *
 *                         *
 *       tfind 0.30        *
 *                         *
 *       2025-10-18        *
 *                         *
 *       Zax               *
 *                         *
 * * * * * * * * * * * * * */

#include <stdio.h>
#include "TMLib.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

#define PROG_NAME "Tfind"
#define PROG_VERSION "0.39"

struct tprint_database tprint_return;
struct tfind_flags tfflags [1] = {0};


int main (int argc, char *argv [])

{
struct dirent *dir_ents;
struct stat file_stat;
struct find_list_entry *find_list;
struct tprint_database tprint_return;

char switch_chr;
const char gpx_file_ext [90] = GRAPHICS_EXTENSIONS;
char C_W_D [FILENAME_LENGTH];				// base directory of search
char path_sub [FILENAME_LENGTH];
char swap_made = TRUE;					// swap was made on last sort pass
char sort_need_check = TRUE;
char *ext_match;
char c_d, p_d;

int lp = 0;
int line_index;
int find_list_write = 0;			// number of file items found in search
int find_list_read = 0;
int find_list_curr_size = 0;
int arg_no, switch_pos, swap_index, mas_lp;


DIR *DIR_PATH;

tfflags->tprt = FALSE;
tfflags->verbose = FALSE;


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
				case 'r':
					tfflags->recurse = SW_ON;
					break;
				case 's':
					tfflags->sort = SW_ON;
					break;
				case 't':
					tfflags->tprt = SW_ON;
					break;
				case 'v':
					tfflags->verbose = SW_ON;
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
/*		if (strcmp (img_name, "") == 0)
			{
			strncpy (img_name, argv [arg_no], FILENAME_LENGTH);
			} */
		}	// END else if int argv
	}	// END for arg_no




// Initial search section
find_list = (struct find_list_entry *) malloc (sizeof (struct find_list_entry) * DATABASE_INITIAL_SIZE);
find_list_curr_size = DATABASE_INITIAL_SIZE;
getcwd (C_W_D, FILENAME_LENGTH);			// get present working directory
strcat (C_W_D, SLASH_TERM);
DIR_PATH = opendir (PATH_CURRENT);			// open directory

if (DIR_PATH != NULL)
	{
	while ((dir_ents = readdir (DIR_PATH)))		// get directory listing
		{
		lstat (dir_ents->d_name, &file_stat);
		if (file_stat.st_mode & S_IFREG)
			{
			ext_match = strstr (gpx_file_ext, get_gpx_ext (dir_ents->d_name));
			if ((ext_match - gpx_file_ext) > 0)
				{
				strcpy (find_list [find_list_write].file_ext, get_gpx_ext (dir_ents->d_name));
				strcpy (find_list [find_list_write].filepath, dir_ents->d_name);
				find_list [find_list_write].object_type = FILE_ENTRY;	// set type to file
				find_list_write ++;
				}
				else
				{
				find_list [find_list_write].object_type = T_REJ;
				}
			}
			else
			{
			c_d = !strcmp (dir_ents->d_name, DIR_CURRENT);
			p_d = !strcmp (dir_ents->d_name, DIR_PREV);
			if ((file_stat.st_mode & S_IFDIR) && !(c_d || p_d))
				{
				strcpy (find_list [find_list_write].filepath, dir_ents->d_name);
				find_list [find_list_write].object_type = DIR_ENTRY;	// set type to directory
				find_list_write ++;
				}
				else
				{
				find_list [find_list_write].object_type = UNKNOWN_ENTRY;	// mark as unneeded type
				}
			}
		if (find_list_write + 1 == find_list_curr_size)		// allocated more memory if needed
			{
			find_list_curr_size += DATABASE_INCREMENT;
			find_list = (struct find_list_entry *) realloc (find_list, sizeof (struct find_list_entry) * find_list_curr_size);
			}
		}
	(void) closedir (DIR_PATH);
	}
	else
	{
	perror ("Couldn't open the directory");		// FIX
	}

// Feedback search section
if (tfflags->recurse)
{
while (find_list_read < find_list_write)
	{
	chdir (C_W_D);					// go back to the starting directory
	if (find_list [find_list_read].object_type == DIR_ENTRY)
		{
		strcpy (path_sub, C_W_D);
		strcat (path_sub, find_list [find_list_read].filepath);		// compose directory location for search
		chdir (path_sub);						// move to search directory
		DIR_PATH = opendir (path_sub);					// open directory
		if (DIR_PATH != NULL)
			{
			while ((dir_ents = readdir (DIR_PATH)))			// get directory listing
				{
				lstat (dir_ents->d_name, &file_stat);
				if (file_stat.st_mode & S_IFREG)
					{
					ext_match = strstr (gpx_file_ext, get_gpx_ext (dir_ents->d_name));
					c_d = !strcmp (dir_ents->d_name, DIR_CURRENT);
                        		p_d = !strcmp (dir_ents->d_name, DIR_PREV);
					if ((ext_match - gpx_file_ext) > 0 && !(c_d || p_d))
						{
						strcpy (find_list [find_list_write].file_ext, get_gpx_ext (dir_ents->d_name));
						strcat (find_list [find_list_write].filepath, find_list [find_list_read].filepath);
						strcat (find_list [find_list_write].filepath, "/");
						strcat (find_list [find_list_write].filepath, dir_ents->d_name);
						find_list [find_list_write].object_type = FILE_ENTRY;	// set type to file
						find_list_write ++;
						}
						else
						{
						find_list [find_list_write].object_type = T_REJ;
						}
					}
					else
					{
					c_d = !strcmp (dir_ents->d_name, DIR_CURRENT);
                        		p_d = !strcmp (dir_ents->d_name, DIR_PREV);
					if ((file_stat.st_mode & S_IFDIR) && !(c_d || p_d))
						{
						strcat (find_list [find_list_write].filepath, find_list [find_list_read].filepath);
						strcat (find_list [find_list_write].filepath, "/");
						strcat (find_list [find_list_write].filepath, dir_ents->d_name);
						find_list [find_list_write].object_type = DIR_ENTRY;	// set type to directory
						find_list_write ++;
						}
						else
						{
						find_list [find_list_write].object_type = UNKNOWN_ENTRY;	// mark as unneeded type
						}
					}
				if (find_list_write + 1 == find_list_curr_size)			// allocated more memory if needed
					{
					find_list_curr_size += DATABASE_INCREMENT;
					find_list = (struct find_list_entry *) realloc (find_list, sizeof (struct find_list_entry) * find_list_curr_size);
					}
				}
			closedir (DIR_PATH);
			}
		}
	find_list_read ++;
	}
}

for (mas_lp = 0; mas_lp < find_list_write; mas_lp++)
	{
	if (find_list [mas_lp].object_type == FILE_ENTRY)
		{
		tprint_return = get_thumbprint (find_list [mas_lp].filepath);
		printf ("%s %s %c %s\n", tprint_return.gry_print, tprint_return.hue_print, tprint_return.magnitude, tprint_return.filepath);
		} // end find list lp
	} // end lp

}


struct tprint_database get_thumbprint (char *img_name)

/* * * * * * * * * * * * * *
 *                         *
 *       tprint 0.30       *
 *                         *
 *       2025-10-08        *
 *                         *
 *       Zax               *
 *                         *
 * * * * * * * * * * * * * */


{
struct rgb_accumulator rgb_return;
struct maxmin_return limits_return;
struct colgry_accumulator quad_accum [4] = {{0}};
struct dimension_return mag_separation;
struct stat sb;
struct tprint_database tprint_return;

char out_name [FILENAME_LENGTH] = NULL_STRING;
char cmd_line [FILENAME_LENGTH] = NULL_STRING;
unsigned char nine_byte_chunk [9];
unsigned char *thm_buffer;
unsigned char base_sixfour [65] = BASE_SIXTYFOUR;
char mag_string [12];
char hue_print [5] = NULL_STRING;
char gry_print [5] = NULL_STRING;

int lp = 0;
int qlp, llp, hlp, olp, rerr, pos, nn_len;

float hue_value, red_dec, grn_dec, blu_dec, mag_n;

FILE *IMGFILE;
FILE *rgb_thumbnail;
FILE *out_thumbnail;

rerr = snprintf (cmd_line, FILENAME_LENGTH, "%s%s%s", MAGICK_COMMAND, img_name, RGB_ARGS);
//printf ("CL=%s=\n", cmd_line);
rgb_thumbnail = popen (cmd_line, "r");
thm_buffer = (unsigned char *) calloc (1, THUMBNAIL_BYTES + 1);
rerr = fread (thm_buffer, 1, THUMBNAIL_BYTES, rgb_thumbnail);

for (olp = 0; olp < 4; olp += 2)
	{
	for (hlp = 1; hlp < 33; hlp++)
		{
		for (llp = 0; llp < 2; llp++)
			{
			for (qlp = 1; qlp < 9; qlp++)
				{
				for (pos = 0; pos <10 ; pos++)
					{
					nine_byte_chunk [pos] = thm_buffer [lp];
					lp++;
					}
				lp = lp - 1;
				rgb_return = get_nine_six (nine_byte_chunk);
				quad_accum[olp + llp].red_val = quad_accum[olp + llp].red_val + rgb_return.red_val;
				quad_accum[olp + llp].grn_val = quad_accum[olp + llp].grn_val + rgb_return.grn_val;
				quad_accum[olp + llp].blu_val = quad_accum[olp + llp].blu_val + rgb_return.blu_val;
				quad_accum[olp + llp].gry_val = quad_accum[olp + llp].gry_val + (rgb_return.red_val + rgb_return.grn_val + rgb_return.blu_val) / 3;
				base_sixfour [0] = '0';
//printf ("%c%c%c ", base_sixfour [(int) rgb_return.red_val], base_sixfour [(int) rgb_return.grn_val], base_sixfour [(int) rgb_return.blu_val]);
//printf ("\n%f  %f  %f\n", rgb_return.red_val, rgb_return.grn_val, rgb_return.blu_val);
				}	// end qlp
			}	// end llp
		}	// end hlp
	}	// end olp

nn_len = snprintf (out_name, FILENAME_LENGTH, "s/%s%s", img_name, FILE_EXTN);
//printf ("%s\n", out_name);
if (tfflags->tprt == SW_ON)
	{
	if (stat ("s", &sb) == -1)
		{
		mkdir ("s", 0700);
		}
	out_thumbnail = fopen (out_name, "wb");
	rerr = fwrite (thm_buffer, THUMBNAIL_BYTES, 1, out_thumbnail);
	fclose (out_thumbnail);
	}

fclose (rgb_thumbnail);
free (thm_buffer);

for (olp = 0; olp < 4; olp++)
	{
	quad_accum[olp].red_val = quad_accum[olp].red_val / QUADRANT_DIVIDER;
	quad_accum[olp].grn_val = quad_accum[olp].grn_val / QUADRANT_DIVIDER;
	quad_accum[olp].blu_val = quad_accum[olp].blu_val / QUADRANT_DIVIDER;
	quad_accum[olp].gry_val = quad_accum[olp].gry_val / QUADRANT_DIVIDER;
	}

for (olp = 0; olp < 4; olp++)
	{
	limits_return = find_limits (quad_accum[olp].red_val, quad_accum[olp].grn_val, quad_accum[olp].blu_val);
	limits_return.max_val = limits_return.max_val / DECIMAL_DIVIDER;
	limits_return.min_val = limits_return.min_val / DECIMAL_DIVIDER;
//printf ("%s\tQuad: %d\tMax: %5.2f\tMin: %5.2f\tMaxChan: %d\n", img_name, olp, limits_return.max_val, limits_return.min_val, limits_return.channel);
	red_dec = quad_accum[olp].red_val / DECIMAL_DIVIDER;
	grn_dec = quad_accum[olp].grn_val / DECIMAL_DIVIDER;
	blu_dec = quad_accum[olp].blu_val / DECIMAL_DIVIDER;
//printf ("R: %f\tG: %f\tB: %f\n", red_dec, grn_dec, blu_dec);
	if (limits_return.channel == RED_CHAN )
		{
		if (limits_return.max_val - limits_return.min_val == 0)
			{
			if (limits_return.min_val == 0)
				{
				limits_return.max_val = limits_return.max_val + 0.00001;
				}
				else
				{
				limits_return.min_val = limits_return.min_val - 0.00001;
				}
			}
		hue_value = (grn_dec - blu_dec) / (limits_return.max_val - limits_return.min_val);
		if (hue_value < 0)
			{
			hue_value = hue_value + 6;
			}
		hue_value = hue_value * SIXBIT_MULTIPLIER;
		}
	if (limits_return.channel == GRN_CHAN )
		{
		hue_value = (2 + ((blu_dec - red_dec) / (limits_return.max_val - limits_return.min_val))) * SIXBIT_MULTIPLIER;
		}
	if (limits_return.channel == BLU_CHAN )
		{
		hue_value = (4 + ((red_dec - grn_dec) / (limits_return.max_val - limits_return.min_val))) * SIXBIT_MULTIPLIER;
		}
//	printf ("\tHue %d: %5.2f\t%c\n", olp, hue_value, base_sixfour [(int) hue_value]);
	base_sixfour [0] = '0';
	hue_print [olp] = base_sixfour [(int) hue_value];
	base_sixfour [0] = '0';
	gry_print[olp] = base_sixfour [(int) quad_accum[olp].gry_val];
//printf ("%f\n", quad_accum[olp].gry_val);
	}
base_sixfour [0] = '0';

rerr = snprintf (cmd_line, FILENAME_LENGTH, "%s%s%s", MAGICK_COMMAND,  MAG_ARGS, img_name);
IMGFILE = popen (cmd_line, "r");
rerr = (long) fgets (mag_string, 12, IMGFILE);
fclose (IMGFILE);
mag_separation = separate_magnitude (mag_string);
mag_n = (powf ((float) (mag_separation.width * mag_separation.height), EXPONENT) / DIVIDER) - SUBTRACTOR;
if (mag_n < 0)
	{
	mag_n = 0;
	}
strcpy (tprint_return.gry_print, gry_print);
strcpy (tprint_return.hue_print, hue_print);
tprint_return.magnitude = base_sixfour [(int) mag_n];
strcpy (tprint_return.filepath, img_name);
return (tprint_return);
//nn_len = snprintf (new_name, FILENAME_LENGTH, "%s_%s%s%c%s", img_name, gry_print, hue_print, base_sixfour [(int) mag_n], FILE_EXTN);
//printf ("%s\t%s\n", img_name, new_name);
}
