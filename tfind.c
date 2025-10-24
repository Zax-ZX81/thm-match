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
#define PROG_VERSION "0.39"

struct tprint_database tprint_return;
struct tprint_flags tpflags [1] = {0};


int main (int argc, char *argv [])

{
struct dirent *dir_ents;
struct stat file_stat;
struct find_list_entry *find_list;
struct tprint_database tprint_return;
struct tfind_flags tfflags [1] = {0};

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
		tprint_return = thumbprint (find_list [mas_lp].filepath);
		printf ("%s\t%s\t%c\t%s\n", tprint_return.gry_print, tprint_return.hue_print, tprint_return.magnitude [0], tprint_return.filepath);
		} // end find list lp
	} // end lp

}
