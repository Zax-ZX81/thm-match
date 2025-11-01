/* * * * * * * * * * * * * *
 *                         *
 *       tfind 0.35        *
 *                         *
 *       2025-10-18        *
 *                         *
 *       Zax               *
 *                         *
 * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include "TMLib.h"
#include "thumbprint.h"

#define PROG_NAME "Tfind"
#define PROG_VERSION "0.35"

//struct tprint_database tprint_return;
struct tprint_flags tpflags [1] = {0};


int main (int argc, char *argv [])

{
struct tfind_database *tfind_db;
struct dirent *dir_ents;
struct stat file_stat;
struct find_list_entry *find_list;
struct tprint_database tprint_return;
struct tfind_flags tfflags [1] = {0};

char switch_chr;
const char gpx_file_ext [90] = GRAPHICS_EXTENSIONS;
char C_W_D [FILENAME_LENGTH];				// base directory of search
char db_name [FILENAME_LENGTH];
char path_sub [FILENAME_LENGTH];
char swap_made = TRUE;					// swap was made on last sort pass
char *ext_match;
char c_d, p_d;

int line_index;
int db_count = 0;
int find_list_write = 0;			// number of file items found in search
int find_list_read = 0;
int find_list_curr_size = 0;
int arg_no, switch_pos, swap_index, mas_lp;
int sort_start = TRUE;
unsigned long file_size_total = 0;
unsigned long file_size_accum = 0;

double file_size_mult = 0;
double file_progress = 0;

DIR *DIR_PATH;
FILE *DB_OUT;

tfflags->tprt = FALSE;
tfflags->verbose = FALSE;
tfflags->std_out = SW_OFF;
tfflags->sort = SORT_DB;

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
				case 'd':
					tfflags->sort = NO_SORT_DB;
					break;
				case 'r':
					tfflags->recurse = SW_ON;
					break;
				case 's':
					tfflags->std_out = SW_ON;
					break;
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
					printf ("%s# Thumbprint [rstvV] <image file>%s\n", TEXT_YELLOW, TEXT_RESET);
					exit (0);
				}	// END switch
			}	// END for switch_pos
		}	// END if int argv
		else
		{
		if (strcmp (db_name, "") == 0)
			{
			strncpy (db_name, argv [arg_no], FILENAME_LENGTH);
			}
		}	// END else if int argv
	}	// END for arg_no

if (strcmp (db_name, "") != 0)
	{
	strcat (db_name, ".tpdb");
	}
	else
	{
	tfflags->std_out = SW_ON;
	}

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
						find_list [find_list_write].filesize = file_stat.st_size;
						file_size_total += find_list [find_list_write].filesize;
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

//Load database
tfind_db = (struct tfind_database *) malloc (sizeof (struct tfind_database) * DATABASE_INITIAL_SIZE);
file_size_mult = 100.0 / (float)file_size_total;
for (mas_lp = 0; mas_lp < find_list_write; mas_lp++)
	{
	if (find_list [mas_lp].object_type == FILE_ENTRY)
		{
//printf ("F=%s\n", find_list [mas_lp].filepath);
		file_size_accum += find_list [mas_lp].filesize;
		file_progress = file_size_mult * (float)file_size_accum;
		tprint_return = thumbprint (find_list [mas_lp].filepath);
		strcpy (tfind_db [db_count].gry_print, tprint_return.gry_print);
		strcpy (tfind_db [db_count].hue_print, tprint_return.hue_print);
		strcpy (tfind_db [db_count].magnitude, tprint_return.magnitude);
		strcpy (tfind_db [db_count].filepath, tprint_return.filepath);
		tfind_db [db_count].index = db_count;
		fprintf (stderr, "%c%6.2f%%", CGE_RET, file_progress);
//		fprintf (stderr, "%c%d\t%ld\t%6.2f%%", CGE_RET, db_count, file_size_accum, file_progress);
//		printf ("%s\t%s\t%c\t%s\t%d\n", tfind_db [db_count].gry_print, tfind_db [db_count].hue_print, tfind_db [db_count].magnitude [0], tfind_db [db_count].filepath, db_count);
		db_count++;
		} // end find list lp
	} // end lp

// Sort section
while (swap_made && tfflags->sort)
	{
	swap_made = FALSE;
	for (line_index = 0; line_index < db_count - 1; line_index ++)
		{
		if (strcmp (tfind_db [tfind_db [line_index].index].gry_print, \
				tfind_db [tfind_db [line_index + 1].index].gry_print) > 0)
			{
			swap_index = tfind_db [line_index + 1].index;
			tfind_db [line_index + 1].index = tfind_db [line_index].index;
			tfind_db [line_index].index = swap_index;
			swap_made = TRUE;
			}
		}
	if (sort_start && tfflags->std_out == SW_OFF)
		{
		printf ("# %sSorting...%s\n", TEXT_YELLOW, TEXT_RESET);
		sort_start = FALSE;
		}
	}

//Write database
if (tfflags->std_out == SW_OFF)
        {
	DB_OUT = fopen (db_name, "w");           // open output database
	if (DB_OUT == NULL)
		{
		exit_error ("Can't open database for output: ", db_name);
		}
	}

for (line_index = 0; line_index < db_count; line_index++)
	{
	if (tfflags->std_out == SW_OFF)
		{
		fprintf (DB_OUT, "%s\t%s\t%c\t%s\n", tfind_db [tfind_db [line_index].index].gry_print, \
							tfind_db [tfind_db [line_index].index].hue_print, \
							tfind_db [tfind_db [line_index].index].magnitude [0], \
							tfind_db [tfind_db [line_index].index].filepath);
		}
		else
		{
		printf ("%s\t%s\t%c\t%s\n", tfind_db [tfind_db [line_index].index].gry_print, \
					tfind_db [tfind_db [line_index].index].hue_print, \
					tfind_db [tfind_db [line_index].index].magnitude [0], \
					tfind_db [tfind_db [line_index].index].filepath);
		}
	} // end find list lp

if (tfflags->std_out == SW_OFF)
        {
	printf ("%d lines written to %s%s%s\n", db_count, TEXT_BLUE, db_name, TEXT_RESET);
	fclose (DB_OUT);
	}
}
