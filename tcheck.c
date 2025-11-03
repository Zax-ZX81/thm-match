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
#include "search.h"
#include "six_sixfour.h"

#define PROG_VERSION "0.10"

struct tprint_flags tpflags [1] = {0};

int main (int argc, char *argv [])

{

FILE *DB_FP;

int arg_no, switch_pos;		// args section
int database_ferr, db_err;		// database file error
int db_cnt = 0, olp, slp;
int tp_db_alloc = DATABASE_INCREMENT;
int dup_db_alloc = DATABASE_INCREMENT;
int idx, s_res, fuzz_ents;

//unsigned char base_sixfour [65] = BASE_SIXTYFOUR;
char switch_chr;					// args section
char database_filename [FILENAME_LENGTH] = NULL_STRING;
char srch_print [5] = NULL_STRING;
char database_first_line = SW_ON;
char fileline [FILELINE_LENGTH];			// input line

struct tdup_flags tdflags [1] = {0};
struct tprint_database *tp_db;
struct tprint_db_lookup *db_lookup;
struct tmatch_database *dup_db;

tp_db = (struct tprint_database *) malloc (sizeof (struct tprint_database) * tp_db_alloc);
dup_db = (struct tmatch_database *) malloc (sizeof (struct tmatch_database) * dup_db_alloc);
db_lookup = (struct tprint_db_lookup *) malloc (sizeof (struct tprint_db_lookup) * 64);

tdflags->fuzzy = SW_OFF;

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
				case 'f':
					tdflags->fuzzy = SW_ON;
					break;
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
		if (strcmp (database_filename, NULL_STRING) && !strcmp (srch_print, NULL_STRING))
			{
			strncpy (srch_print, argv [arg_no], 5);
			}
		if (!strcmp (database_filename, NULL_STRING))
			{
			strncpy (database_filename, argv [arg_no], FILENAME_LENGTH);
			}
		}	// END else if int argv
	}	// END for arg_no
if (!strcmp (database_filename, NULL_STRING))
	{
	printf ("%s# TCheck [V] <database file>\n%s", TEXT_YELLOW, TEXT_RESET);
	exit (0);
	}
printf ("%s\t%s\n", database_filename, srch_print);

// File open section
DB_FP = fopen (database_filename, FOR_READ);
if (DB_FP == NULL)
	{
	error_message ("Can't find Database: ", database_filename);
	exit (1);
	}

// Database load section
while (!feof (DB_FP))
	{
	database_ferr = (long)fgets (fileline, FILELINE_LENGTH, DB_FP);
	if (database_first_line)		// if database first line, sha_verify
		{
		db_err = tpdb_verify (fileline);
//printf ("DBER=%d=%s\n", db_err, fileline);
		if (db_err == 0)
			{
			fclose (DB_FP);
			error_message ("# Unrecognised file type: ", database_filename);
			exit (1);
			}
		database_first_line = SW_OFF;
		}
	if (fileline != NULL && database_ferr)
		{
		sscanf (fileline, "%s\t%s\t%c\t%[^\t\n]", tp_db [db_cnt].gry_print, tp_db [db_cnt].hue_print, tp_db [db_cnt].magnitude, tp_db [db_cnt].filepath);
//printf ("=%s=\n", tp_db [db_cnt].filepath);
		if (db_lookup [sixfour_to_dec (tp_db [db_cnt].gry_print [0])].start < 0)
			{
			db_lookup [sixfour_to_dec (tp_db [db_cnt].gry_print [0])].start = db_cnt;
			}
		db_lookup [sixfour_to_dec (tp_db [db_cnt].gry_print [0])].ents++;
		}
	if (db_cnt + 1 == tp_db_alloc)              // check memory usage, reallocate
		{
		tp_db_alloc += DATABASE_INCREMENT;
		tp_db = (struct tprint_database *) realloc (tp_db, sizeof (struct tprint_database) * tp_db_alloc);
		}
	db_cnt++;
	}

// Clean up section
fclose (DB_FP);

// Search section

//for (slp = 0; slp < 64; slp++)
//	{
//	printf ("%d", );
//	}
db_cnt = 0;
fuzz_ents = 0;
olp = db_lookup [sixfour_to_dec (srch_print [0])].start;
//printf ("SP=%c\tSC=%s\tST=%d\tE=%d\n", srch_print [0], srch_print, olp, db_lookup [srch_print [0]].ents);
if (tdflags->fuzzy == SW_OFF)
	{
	for (slp = 0; slp < db_lookup [sixfour_to_dec (srch_print [0])].ents + 1; slp++)
		{
		s_res = exact_search (srch_print, tp_db [olp + slp].gry_print);
		if (s_res == 1)
			{
//			printf ("---- %s %s %c   %s\n", tp_db [olp + slp].gry_print, tp_db [olp + slp].hue_print, tp_db [olp + slp].magnitude [0], tp_db [olp + slp].filepath);
			strcpy (dup_db [db_cnt].gry_print, tp_db [olp + slp].gry_print);
			strcpy (dup_db [db_cnt].hue_print, tp_db [olp + slp].hue_print);
			dup_db [db_cnt].magnitude [0] = tp_db [olp + slp].magnitude [0];
			strcpy (dup_db [db_cnt].filepath, tp_db [olp + slp].filepath);
			dup_db [db_cnt].fuzz_match = FALSE;
			db_cnt++;
			}
		if (db_cnt + 1 == dup_db_alloc)              // check memory usage, reallocate
			{
			dup_db_alloc += DATABASE_INCREMENT;
			dup_db = (struct tmatch_database *) realloc (dup_db, sizeof (struct tmatch_database) * dup_db_alloc);
			}
		}
	}
	else
	{
//printf ("BC=%c\tBS=%d\tAC=%c\tAS=%d\n", base_sixfour [sixfour_to_dec (srch_print [0]) - 1], \
			db_lookup [sixfour_to_dec (srch_print [0]) - 1].start, \
			base_sixfour [sixfour_to_dec (srch_print [0]) + 1], \
			db_lookup [sixfour_to_dec (srch_print [0]) + 1].start);
//printf ("O=%d\tE=%d\n", olp, fuzz_ents);
	if (db_lookup [sixfour_to_dec (srch_print [0]) - 1].start > 0)
		{
		olp = db_lookup [sixfour_to_dec (srch_print [0]) - 1].start;
		fuzz_ents = db_lookup [sixfour_to_dec (srch_print [0])].ents + \
			db_lookup [sixfour_to_dec (srch_print [0]) - 1].ents;
		}
	if (db_lookup [sixfour_to_dec (srch_print [0]) + 1].start > 0)
		{
		fuzz_ents = fuzz_ents + db_lookup [sixfour_to_dec (srch_print [0])].ents + \
			db_lookup [sixfour_to_dec (srch_print [0]) + 1].ents;
		}
	for (slp = 0; slp < fuzz_ents + 1; slp++)
		{
//printf ("SP=%s\tTP=%s\n", srch_print, tp_db [olp + slp].gry_print);
		s_res = fuzz_search (srch_print, tp_db [olp + slp].gry_print);
		if (s_res > 0)
			{
//			printf ("#### %s %s %c   %s\n", tp_db [olp + slp].gry_print, tp_db [olp + slp].hue_print, tp_db [olp + slp].magnitude [0], tp_db [olp + slp].filepath);
			strcpy (dup_db [db_cnt].gry_print, tp_db [olp + slp].gry_print);
			strcpy (dup_db [db_cnt].hue_print, tp_db [olp + slp].hue_print);
			dup_db [db_cnt].magnitude [0] = tp_db [olp + slp].magnitude [0];
			strcpy (dup_db [db_cnt].filepath, tp_db [olp + slp].filepath);
			if (s_res == 2)
				{
				dup_db [db_cnt].fuzz_match = TRUE;
				}
				else
				{
				dup_db [db_cnt].fuzz_match = FALSE;
				}
			db_cnt++;
			}
		if (db_cnt + 1 == dup_db_alloc)              // check memory usage, reallocate
			{
			dup_db_alloc += DATABASE_INCREMENT;
			dup_db = (struct tmatch_database *) realloc (dup_db, sizeof (struct tmatch_database) * dup_db_alloc);
			}
		}
	}
for (olp = 0;olp < db_cnt; olp++)
	{
	printf ("%2d\t%s\t%s\t%c\t%s\t%d\n", olp, dup_db [olp].gry_print, dup_db [olp].hue_print, dup_db [olp].magnitude  [0], dup_db [olp].filepath, dup_db [olp].fuzz_match);
	}

/*for (olp = 0;olp < 64; olp++)
	{
	printf ("%d\t%d\t%d\n", olp, db_lookup [olp].start, db_lookup [olp].ents);
	}*/
}
