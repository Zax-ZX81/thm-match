/* * * * * * * * * * * * * *
 *                         *
 *       tcheck 0.10       *
 *                         *
 *       2025-10-01        *
 *                         *
 *       Zax               *
 *                         *
 * * * * * * * * * * * * * */

#include <stdio.h>
#include "TMLib.h"
#include <stdlib.h>
#include <string.h>

#define PROG_VERSION "0.10"

struct tprint_flags tpflags [1] = {0};

int main (int argc, char *argv [])

{

FILE *DB_FP;

int arg_no, switch_pos;		// args section
int database_ferr, db_err;		// database file error
int db_cnt = 0, olp;
int db_alloc = DATABASE_INCREMENT;

char switch_chr;		// args section
char database_filename [FILENAME_LENGTH] = "";
char database_first_line = SW_ON;
char fileline [FILENAME_LENGTH];			// input line
char match_found = FALSE;				// if match is found
char database_good = FALSE;				// has database passed sha_verify?

struct tprint_database *tp_db;

tp_db = (struct tprint_database *) malloc (sizeof (struct tprint_database) * db_alloc);

/*

find_list = (struct find_list_entry *) malloc (sizeof (struct find_list_entry) * DATABASE_INITIAL_SIZE);
if (find_list_write + 1 == find_list_curr_size)		// allocated more memory if needed
{
find_list = (struct find_list_entry *) realloc (find_list, sizeof (struct find_list_entry) * find_list_curr_size);
}

*/


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
				case 'V':
					printf ("TCheck version %s\n", PROG_VERSION);
					exit (0);
				default:
					printf ("%s# TCheck [V] <database file>]\n%s", TEXT_YELLOW, TEXT_RESET);
					exit (0);
				}	// END switch
			}	// END for switch_pos
		}	// END if int argv
		else
		{
		if (!strcmp (database_filename, ""))
			{
			strncpy (database_filename, argv [arg_no], FILENAME_LENGTH);
			}
		}	// END else if int argv
	}	// END for arg_no
if (!strcmp (database_filename, ""))
	{
	printf ("%s# TCheck [V] <database file>\n%s", TEXT_YELLOW, TEXT_RESET);
	exit (0);
	}
printf ("%s\n", database_filename);
// File open section
DB_FP = fopen (database_filename, "r");
if (DB_FP == NULL)
	{
	exit_error ("Can't find Database: ", database_filename);
	}

// Search section
while (!feof (DB_FP))
	{
	database_ferr = (long)fgets (fileline, FILENAME_LENGTH, DB_FP);
	if (database_first_line)		// if database first line, sha_verify
		{
		db_err = tpdb_verify (fileline);
//printf ("DBER=%d=%s\n", db_err, fileline);
		if (db_err == 0)
			{
			fclose (DB_FP);
			exit_error ("# Unrecognised file type: ", database_filename);
			}
		database_first_line = SW_OFF;
		}
	if (fileline != NULL && database_ferr)
		{
		sscanf (fileline, "%s\t%s\t%c\t%s", tp_db[db_cnt].gry_print, tp_db[db_cnt].hue_print, tp_db[db_cnt].magnitude, tp_db[db_cnt].filepath);
		}
	if (db_cnt + 1 == db_alloc)              // check memory usage, reallocate
		{
		db_alloc += DATABASE_INCREMENT;
		tp_db = (struct tprint_database *) realloc (tp_db, sizeof (struct tprint_database) * db_alloc);
		}
	db_cnt++;
	}
db_cnt--;

// Clean up section
fclose (DB_FP);

for (olp = 0;olp < db_cnt; olp++)
	{
	printf ("%s\t%s\t%c\t%s\n", tp_db[olp].gry_print, tp_db[olp].hue_print, tp_db[olp].magnitude [0], tp_db[olp].filepath, olp);
	}
}
