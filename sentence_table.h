/**************************************************************
 *
 *                     sentence_table.h
 *
 *     Assignment: COMP40 Homework 1 - Filesnpix
 *     Authors:  Luella Sugiman (lsugim01), Jun Han Huang (jhuang14)
 *     Date:     21/09/20
 *
 *     Interface file of the sentence_table - contains the functions that
 *     will be implemented in the implementation file of sentence_table.
 *
 **************************************************************/

#ifndef SENTENCE_TABLE
#define SENTENCE_TABLE

#include <stdlib.h>
#include <stdio.h>
#include <except.h>
#include <assert.h>
#include <stddef.h>
#include "readaline.h"
#include "atom.h"
#include "seq.h"
#include "table.h"


extern void add_sentence(char *line, size_t size, char *filename,
    int line_num, Table_T table);
extern char *format_sentence(char *line, size_t size);
extern int is_word_char(char c);
extern void print_match_group(const void* key, void** value, void* cl);
extern void free_sequence(const void* key, void** value, void* cl);


#endif
