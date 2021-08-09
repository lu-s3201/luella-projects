/**************************************************************
 *
 *                     simlines.c
 *
 *     Assignment: COMP40 Homework 1 - Filesnpix
 *     Authors:  Luella Sugiman (lsugim01), Jun Han Huang (jhuang14)
 *     Date:     21/09/20
 *
 *     Driver file of the Simlines program - contains the main() function
 *     that will run the Simlines program.
 *
 **************************************************************/

#ifndef SIMLINES
#define SIMLINES

#include <stdlib.h>
#include <stdio.h>
#include <except.h>
#include <assert.h>
#include "readaline.h"
#include "atom.h"
#include "seq.h"
#include "table.h"
#include "sentence_table.h"

static FILE *open_or_abort(char *filename, char *mode);
void read_file(FILE *fp, char *filename, Table_T table);


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    integer indicating argument count from the command line,
    char pointer to an array containing arguments.
 * Purpose: reads from files, stores its data into data structures,
    outputs match groups (if any) and frees allocated memory.
 * Returns: integer indicating success of program.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int main(int argc, char *argv[])
{
    FILE *fp = NULL;
    Table_T table = NULL;

    if (argc == 1) {
        /* If there are no files given via the command line, exit the program */
        return EXIT_FAILURE;
    } else {
        /* Create table to store data and assert it's successfully created */
        table = Table_new(1000, NULL, NULL);
        assert(table != NULL);

        /* Read from every file given via command line */
        for (int i = 1; i < argc; i++) {
            fp = open_or_abort(argv[i], "r");
            read_file(fp, argv[i], table);
            fclose(fp);
        }
        fp = NULL;
    }

    /*
     * Call the print_match_group on every value in the table. The first
     * integer indicates whether or not the match group printed is the first,
     * which will have slightly different output.
     */
    int first = 1;
    Table_map(table, print_match_group, &first);

    /*
     * Call the free_sequence on every value (Sequence) in the table. This
     * will also free memory of the Structs in each Sequence value. Then,
     * free memory held by the Table structure.
     */
    Table_map(table, free_sequence, NULL);
    Table_free(&table);

    return EXIT_SUCCESS;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    pointer to the file currently being read,
    char pointer to the name of the file being read,
    pointer to a Table structure holding the data of the program.
 * Purpose: Reads into a given file and calls the add_sentence
    function, which adds each sentence of the file into the data
    structure.
 * Returns: nothing.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void read_file(FILE *fp, char *filename, Table_T table)
{
    char *line = NULL;
    size_t size = 0;
    int line_num = 1;

    /*
     * Read every line of the given file. If the line isn't an empty line,
     * then add it to the Table structure to be stored. If not, free the
     * memory held by it.
     */
    do {
        size = readaline(fp, &line);
        if (size != 0) {
            add_sentence(line, size, filename, line_num, table);
        } else {
            free(line);
        }
        ++line_num;
    } while (!((size == 0) && (line == NULL)));
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* Arguments:
   file name obtained throught the command line,
   mode to use for fopen.
 * Purpose: open a file stream to a file obtained through the command line
    and check if it's successfully opened. if not, exit the program.
 * Returns: a pointer to the successfully opened file stream.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
static FILE *open_or_abort(char *filename, char *mode)
{
    FILE *fp = fopen(filename, mode);

    /*
     * Open a file stream to the given file. Then, check if the file is
     * successfully opened. If not, exit the program.
     */
    assert(fp != NULL);

    return fp;
}


#endif
