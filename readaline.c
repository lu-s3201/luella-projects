/**************************************************************
 *
 *                     readaline.c
 *
 *     Assignment: COMP40 Homework 1 - Filesnpix
 *     Authors:  Luella Sugiman (lsugim01), Jun Han Huang (jhuang14)
 *     Date:     21/09/20
 *
 *     Implementation for the readaline function. This program
 *     will take a line from an already opened file, store the
 *     file contents in a string pointer and return the number
 *     of bytes in that line.
 *
 **************************************************************/

#ifndef READALINE
#define READALINE

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "readaline.h"


/*
 * Arguments:
 * A pointer to the file being read, and a pointer to the 'string' to store
 * file contents
 * Purpose:
 * 1. Determine and return the number of bytes in a line for a file.
 * 2. Stores the file line content in char **datapp.
 * Returns:
 * size_t line_len, which contains the number of bytes stored in the read line
 */
size_t readaline(FILE *inputfd, char **datapp)
{
    /* Checks for Runtime Error in inputfd or datapp */
    assert(inputfd != NULL);
    assert(datapp != NULL);

    /* Assign memory to hold 100 characters at first */
    size_t allocated_len = 100;
    size_t line_len = -1;
    char *cp = (char *) malloc(allocated_len * sizeof(cp));

    /* Checks for Runtime Error in assigning memory to hold the line */
    assert(cp != NULL);

    do {
        ++line_len;

        /* If the current line exceeds the assigned number of  bytes,
         * assign a larger array of bytes to store the sentence.
         */
        if (line_len == (allocated_len - 1)) {
            /*
             * Allocate memory twice the size of the current memory and
             * transfer the sentence read from the file to the new location.
             */
            allocated_len *= 2;
            char *temp_c = malloc(allocated_len * sizeof(temp_c));
            for (size_t i = 0; i < line_len; i++) {
                temp_c[i] = cp[i];
            }
            char *temp_holder = cp;
            cp = temp_c;

            /* Free memory containing the previous array of bytes */
            free(temp_holder);
            temp_c = NULL;
            temp_holder = NULL;
        }
        /* Assign the next character of the line to the end of the array */
        cp[line_len] = getc(inputfd);

        /* Check for Runtime Error in reading the next character */
        assert(!ferror(inputfd));
    } while (cp[line_len] != '\n' && cp[line_len] != EOF);

    /* Add a null character to the end of the sentence */
    cp[line_len + 1] = '\0';

    /* Checks if the current line is already at End of File */
    if (cp[line_len] == EOF && line_len == 0) {
        free(cp);
        *datapp = NULL;
        return 0;
    }

    *datapp = cp;
    return (line_len + 1);
}

#endif
