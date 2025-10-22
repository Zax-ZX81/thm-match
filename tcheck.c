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
#include <stdlib.h>
#include <string.h>
#include "TMLib.h"

#define PROG_VERSION "0.10"

int main (int argc, char *argv [])

{

FILE *DB_FP;

int arg_no, switch_pos;		// args section
int database_ferr, db_err;		// database file error

char switch_chr;		// args section
char database_filename [FILENAME_LENGTH] = "";
char database_first_line = SW_ON;
char fileline [FILENAME_LENGTH];			// input line
char match_found = FALSE;				// if match is found
char database_good = FALSE;				// has database passed sha_verify?

struct tprint_database tp_db;

/*tp_db = (struct tprint_database *) malloc (sizeof (struct tprint_database) * searchlist_alloc_size);
if (searchlist_lines + 1 == searchlist_alloc_size)              // check memory usage, reallocate
	{
	searchlist_alloc_size += DATABASE_INCREMENT;
	ssort_db = (struct sha_sort_database *) realloc (ssort_db, sizeof (struct sha_sort_database) * searchlist_alloc_size);
	}*/


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
do
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
		sscanf (fileline, "%s\t%s\t%c\t%s", tp_db.gry_print, tp_db.hue_print, tp_db.magnitude, tp_db.filepath);
		}
	printf ("G=%s\tH=%s\tM=%c\tF=%s\n", tp_db.gry_print, tp_db.hue_print, tp_db.magnitude [0], tp_db.filepath);
	} while (!feof (DB_FP));

// Clean up section
fclose (DB_FP);

}
