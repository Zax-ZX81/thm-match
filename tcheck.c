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
int db_cnt = 0, olp, slp;
int db_alloc = DATABASE_INCREMENT;
int idx, s_res;

char switch_chr;		// args section
char database_filename [FILENAME_LENGTH] = "";
char srch_print [5] = "";
char database_first_line = SW_ON;
char fileline [FILENAME_LENGTH];			// input line

struct tprint_database *tp_db;
struct tprint_db_lookup *db_lookup;
tp_db = (struct tprint_database *) malloc (sizeof (struct tprint_database) * db_alloc);
db_lookup = (struct tprint_db_lookup *) malloc (sizeof (struct tprint_db_lookup) * 64);


for (idx = 0; idx < 64; idx ++)             // initialise lookup table
{
	db_lookup [idx].start = -1;
	db_lookup [idx].ents = 0;
}

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
		if (strcmp (database_filename, "") && !strcmp (srch_print, ""))
			{
			strncpy (srch_print, argv [arg_no], 5);
			}
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
printf ("%s\t%s\n", database_filename, srch_print);
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
		sscanf (fileline, "%s\t%s\t%c\t%s", tp_db [db_cnt].gry_print, tp_db [db_cnt].hue_print, tp_db [db_cnt].magnitude, tp_db [db_cnt].filepath);
		if (db_lookup [sixfour_to_dec (tp_db [db_cnt].gry_print [0])].start < 0)
			{
			db_lookup [sixfour_to_dec (tp_db [db_cnt].gry_print [0])].start = db_cnt;
			}
		db_lookup [sixfour_to_dec (tp_db [db_cnt].gry_print [0])].ents++;
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
//for (slp = 0; slp < 64; slp++)
//	{
//	printf ("%d", );
//	}
olp = db_lookup [sixfour_to_dec (srch_print [0])].start;
//printf ("SP=%c\tSC=%s\tST=%d\tE=%d\n", srch_print [0], srch_print, olp, db_lookup [srch_print [0]].ents);
for (slp = 0; slp < db_lookup [sixfour_to_dec (srch_print [0])].ents + 1; slp++)
	{
	s_res = fuzz_search (srch_print, tp_db [olp + slp].gry_print);
//	printf ("DC=%d\tO=%d\tS=%d\tC=%d\tLE=%d\tGP=%s\n", db_cnt, olp, slp, olp + slp, db_lookup [sixfour_to_dec (tp_db [olp + slp].gry_print [0])].ents, tp_db [olp + slp].gry_print);
//	printf ("DC=%d\tO=%d\tS=%d\tC=%d\tGP=%s\tSC=%d\n", db_cnt, olp, slp, olp + slp, tp_db [olp + slp].gry_print, strcmp (tp_db [olp + slp].gry_print, "JOAD"));
//	printf ("DC=%d\tO=%d\tS=%d\tC=%d\tLE=%d\tSC=%d\n", db_cnt, olp, slp, olp + slp, db_lookup [sixfour_to_dec (tp_db [olp + slp].gry_print [0])].ents, strcmp (tp_db [olp + slp].gry_print, "JQAD"));
//	printf ("%s\n", tp_db [db_lookup [olp + slp].start].gry_print);
	if (s_res == 1)
		{
		printf ("---- %s %s %c   %s\n", tp_db [olp + slp].gry_print, tp_db [olp + slp].hue_print, tp_db [olp + slp].magnitude [0], tp_db [olp + slp].filepath);
		}
	if (s_res == 2)
		{
		printf ("#### %s %s %c   %s\n", tp_db [olp + slp].gry_print, tp_db [olp + slp].hue_print, tp_db [olp + slp].magnitude [0], tp_db [olp + slp].filepath);
		}
	}
/*for (olp = 0;olp < db_cnt; olp++)
	{
	printf ("%s\t%s\t%c\t%s\n", tp_db [olp].gry_print, tp_db [olp].hue_print, tp_db [olp].magnitude  [0], tp_db [olp].filepath, olp);
	}

for (olp = 0;olp < 64; olp++)
	{
	printf ("%d\t%d\t%d\n", olp, db_lookup [olp].start, db_lookup [olp].ents);
	}*/
}
