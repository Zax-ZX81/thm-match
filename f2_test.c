/* * * * * * * * * * * * * * *
 *                           *
 *       tftest1             *
 *                           *
 *       2025-10-18          *
 *                           *
 *       Zax                 *
 *                           *
 * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include "TMLib.h"


int main (int argc, char *argv [])

{
struct dirent *dir_ents;
struct stat file_stat;
struct find_list_entry *find_list;

DIR *DIR_PATH;

int line_index;
int find_list_write = 0;			// number of file items found in search
int find_list_read = 0;
int find_list_curr_size = 0;
int swap_index, lp;

const char gpx_file_ext [90] = GRAPHICS_EXTENSIONS;
char C_W_D [FILENAME_LENGTH];				// base directory of search
char path_sub [FILENAME_LENGTH];
char swap_made = TRUE;					// swap was made on last sort pass
char sort_need_check = TRUE;
char *ext_match;
char header = TRUE;
char c_d, p_d;
//char ext_string [8];

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
//printf ("DN=%s=\tE=%s=\t%ld\n", find_list [find_list_write].filepath, find_list [find_list_write].file_ext, strlen (find_list [find_list_write].file_ext));
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
//printf ("_%s\tdc=%d=\tdp=%d=\tboth=%d=\n", dir_ents->d_name, c_d, p_d, c_d || p_d);
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
//		if (!(strcmp (dir_ents->d_name, DIR_CURRENT) && strcmp (dir_ents->d_name, DIR_PREV)))
//			{						// Filter out ".", ".." from search
//			find_list [find_list_write].object_type = T_REJ;
//			}
/*if (header)
	{
	fprintf (stderr, "FindListWr\tObjType\tDType\tDName\n", find_list_write, \
                                        find_list [find_list_write].object_type, \
                                        dir_ents->d_type, dir_ents->d_name);
	header = FALSE;
	}
fprintf (stderr, "\tFLW=%3d\tOT=%c\tDT=%d\tDN=%s=\t%s\n", find_list_write, \
					find_list [find_list_write].object_type, \
					dir_ents->d_type, dir_ents->d_name, find_list [find_list_write].file_ext);*/
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
if (argc > 2)
{
while (find_list_read < find_list_write)
	{
	chdir (C_W_D);					// go back to the starting directory
	if (find_list [find_list_read].object_type == DIR_ENTRY)
		{
		strcpy (path_sub, C_W_D);
		strcat (path_sub, find_list [find_list_read].filepath);		// compose directory location for search
		chdir (path_sub);						// move to search directory
//printf ("BEG\tflfp=%s=\n", find_list [find_list_read].filepath);
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
//printf ("FILE\t%s\tboth=%d=\n", dir_ents->d_name, !(c_d || p_d));
					if ((ext_match - gpx_file_ext) > 0 && !(c_d || p_d))
						{
						strcpy (find_list [find_list_write].file_ext, get_gpx_ext (dir_ents->d_name));
						strcat (find_list [find_list_write].filepath, find_list [find_list_read].filepath);
						strcat (find_list [find_list_write].filepath, "/");
						strcat (find_list [find_list_write].filepath, dir_ents->d_name);
//printf ("NAME=%s=\n", find_list [find_list_write].filepath);
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
//printf ("DIRS\t%s\tboth=%d=\n", dir_ents->d_name, !(c_d || p_d));
					if ((file_stat.st_mode & S_IFDIR) && !(c_d || p_d))
						{
						strcat (find_list [find_list_write].filepath, find_list [find_list_read].filepath);
						strcat (find_list [find_list_write].filepath, "/");
						strcat (find_list [find_list_write].filepath, dir_ents->d_name);
//printf ("DIR=%s=\n", find_list [find_list_write].filepath);
						find_list [find_list_write].object_type = DIR_ENTRY;	// set type to directory
						find_list_write ++;
						}
						else
						{
						find_list [find_list_write].object_type = UNKNOWN_ENTRY;	// mark as unneeded type
						}
					}
//fprintf (stderr, "DTP\tN=%s\tT=%c\n", dir_ents->d_name, find_list [find_list_write].object_type);
//				strcpy (find_list [find_list_write].filepath, find_list [find_list_read].filepath);
//				strcat (find_list [find_list_write].filepath, "/");
//				strcat (find_list [find_list_write].filepath, dir_ents->d_name);
				if (find_list_write + 1 == find_list_curr_size)			// allocated more memory if needed
					{
					find_list_curr_size += DATABASE_INCREMENT;
					find_list = (struct find_list_entry *) realloc (find_list, sizeof (struct find_list_entry) * find_list_curr_size);
					}
/*				if (sfflags->verbose)
					{
					fprintf (stderr, "FBS\tFLW=%3d\tOT=%c\tFP=%s=\n", find_list_write, \
									find_list [find_list_write].object_type, \
									find_list [find_list_write].filepath);
					}*/
				}
			closedir (DIR_PATH);
			}
//			else
//			{
//			perror ("Couldn't open the directory");		// FIX
//			}
		}
	find_list_read ++;
	}
}
for (lp = 0; lp < find_list_write; lp++)
	{
	if (find_list [lp].object_type == FILE_ENTRY || find_list [lp].object_type == DIR_ENTRY)
		{
		printf ("%s\t\t%s\n", find_list [lp].filepath, find_list [lp].file_ext);
//		printf ("%d\t%s\t%s\n", lp, find_list [lp].filepath, find_list [lp].file_ext);
		}
	}
/*
// Sort section
while (swap_made == TRUE && sfflags->sort > 0)
	{
	swap_made = FALSE;
	for (line_index = 0; line_index < database_index - 1; line_index ++)
		{
		if (sfflags->sort == SORT_SHA)		// sort by SHA256sum, default
			{
			if (strcmp (sfind_db [sfind_db [line_index].index].sha, sfind_db [sfind_db [line_index + 1].index].sha) > 0)
				{
				swap_index = sfind_db [line_index + 1].index;
				sfind_db [line_index + 1].index = sfind_db [line_index].index;
				sfind_db [line_index].index = swap_index;
				swap_made = TRUE;
				}
			}
		if (sfflags->sort == SORT_FILE)		// sort by filepath
			{
			if (strcmp (sfind_db [sfind_db [line_index].index].filepath, sfind_db [sfind_db [line_index + 1].index].filepath) > 0)
				{
				swap_index = sfind_db [line_index + 1].index;
				sfind_db [line_index + 1].index = sfind_db [line_index].index;
				sfind_db [line_index].index = swap_index;
				swap_made = TRUE;
				}
			}
		}
	if (sort_need_check && swap_made)
		{
		if (database_index > SORT_MAX_LINES)		// abandon sort if file too big
			{
			printf ("# %sNot sorting, file too big.%s\n", TEXT_YELLOW, TEXT_RESET);
			sfflags->sort = SORT_NONE;
			}
		if (sfflags->sort != SORT_NONE)			// show that we're sorting
			{
			printf ("# %sSorting...%s\n", TEXT_YELLOW, TEXT_RESET);
			}
		}
	sort_need_check = FALSE;
	}
*/
// Clean-up section
chdir (C_W_D);
free (find_list);
find_list = NULL;

}
