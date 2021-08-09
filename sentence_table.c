/**************************************************************
 *
 *                     sentence_table.c
 *
 *     Assignment: COMP40 Homework 1 - Filesnpix
 *     Authors:  Luella Sugiman (lsugim01), Jun Han Huang (jhuang14)
 *     Date:     21/09/20
 *
 *     Implementation file of sentence_table.c - defines the functions
 *     listed in sentence_table.c, which are used to put and retrieve
 *     data from the data structures in the Simlines program.
 *
 **************************************************************/

#include "sentence_table.h"

struct Info {
    char *filename;
    int line_num;
};

typedef struct Info Info;


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    char pointer to the line just read from the file,
    size_t containing the size (in bytres) of the given line,
    char pointer to the name of the file being read from,
    integer containing the line number of the given line,
    Table pointer containing the table structure holding all the data.
 * Purpose: adds the given line into the given Table structure (if the line
    is not empty) as an Atom key, and adds a Struct containing its information
    (filename it's from and line number) to a Sequence as the Table's value.
 * Returns: nothing.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void add_sentence(char *line, size_t size, char *filename,
    int line_num, Table_T table)
{
    /*
     * Format the sentence to filter out non-word characters, and convert
     * it into an Atom to use as the key for the Table. If the returned
     * line is a NULL pointer, the line has no word characters so nothing
     * should be added to the Table.
     */
    char *formatted_line = format_sentence(line, size);
    if (formatted_line == NULL) {
        return;
    }
    const char *key = Atom_string(formatted_line);

    /*
     * Create a new Info Struct, containing the information of the given
     * sentence.
     */
    Info *info = malloc(sizeof(info->filename) + sizeof(info->line_num));
    assert(info != NULL);
    info->filename = filename;
    info->line_num = line_num;

    /*
     * If there's a value under the given key, concatenate the new
     * Struct to the existing Sequence and push into the Table. If not,
     * push a new Sequence into the Table.
     */
    if (Table_get(table, key) == NULL) {
        Seq_T value = Seq_seq((void *)info, NULL);
        assert(value != NULL);
        Table_put(table, key, (void *)value);
    } else {
        Seq_addhi(Table_get(table, key), (void *)info);
    }

    free(formatted_line);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    char pointer to the sentence line to format,
    size_t containing the size (in bytes) of the given line.
 * Purpose: formats the sentence to contain only word characters, and for
     the word character groupings to be seperated by a single space.
 * Returns: char pointer to the formatted sentence.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
char *format_sentence(char *line, size_t size)
{
    char *formatted = malloc(size * sizeof(formatted));
    size_t count = 0;

    /*
     * Read through the whole line to determine if they're word characters,
     * and format the sentence into word character groupings, with a space
     * seperating each grouping. The for loop leaves off the last character
     * of the sentence, not including the \n character in the fomatted
     * sentence.
     */
    for (size_t i = 0; i < size; i++) {
        if (is_word_char(line[i]) == 1) {
            /* If the character is a word character, add it to the sentence */
            formatted[count] = line[i];
            ++count;
        } else {
            /*
             * If there isn't already a space added and if the formatted
             * sentence isn't empty, add a space character to the formatted
             * sentence. The (count != 0) conditional prevents the valgrind
             * 'invalid read' warning.
             */
            if (count != 0) {
                if (formatted[count - 1] != ' ') {
                    formatted[count] = ' ';
                    ++count;
                }
            }
        }
    }
    /*
     * If the last character of the formatted sentence is a space, delete
     * the space. The (count != 0) conditional also prevents the valgrind
     * 'invalid read' warning.
     */
    if (count != 0) {
        if (formatted[count - 1] != ' ') {
            formatted[count] = '\0';
        } else {
            formatted[count - 1] = '\0';
        }
    } else {
        /*
         * If the line had no word characters, free memory of the formatted
         * sentence and return a NULL pointer.
         */
        free(formatted);
        free(line);
        return NULL;
    }

    /*
     * Free memory containing the unformatted sentence and then return the
     * formatted sentence.
     */
    free(line);
    return formatted;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    char indicating the character to assess
 * Purpose: indicates whether or not a character is a word character (A-Z,
    a-z, 0-9 or -)
 * Returns: integer indicating if it's a word character (1) or not (0)
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int is_word_char(char c)
{
    /* If the character is a word character, return 1 */
    if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z')) {
        return 1;
    } else if ((c >= 'a' && c <= 'z') || (c == '-')) {
        return 1;
    } else {
        /* If the character is not a word character, return 0 */
        return 0;
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    void pointer to the current key of Table_T (an Atom),
    void pointer to the current value of Table_T (a Sequence),
    void pointer to a third argument (integer pointer to a 'first' variable
        indicating whether or not the match group is the first being printed).
 * Purpose: if the current value (Sequence) of the Table_T has 2/more values,
     the specified standard output will be printed to stdout.
 * Returns: nothing.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void print_match_group(const void* key, void** value, void* cl)
{
    const char *atom = (char *) key;
    Seq_T seq = (Seq_T) *value;
    int *first = (int *) cl;

    /*
     * In the case of a whole line being empty except for a null character,
     * meaning the whole line was comprised of non-ASCII characters, then
     * don't print out the match group.
     */
    const char *space = Atom_string("\0");
    if (*atom == *space) {
        return;
    }

    /*
     * If the current sentence has a match group (2/more occurences), then
     * print out the match group standard output.
     */
    if (Seq_length(seq) >= 2) {
        /*
         * If the match group isn't the first, print out an extra /n character
         * to seperate match groups.
         */
        if (*first == 1) {
            printf("%s\n", atom);
            *first = 0;
        } else {
            printf("\n%s\n", atom);
        }

        for (int i = 0; i < Seq_length(seq); i++) {
            Info *info = Seq_get(seq, i);
            printf("%-20s %7d\n", info->filename, info->line_num);
        }
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    void pointer to the current key of Table_T (an Atom),
    void pointer to the current value of Table_T (a Sequence),
    void pointer to a third argument (NULL).
 * Purpose: frees memory taken by the Sequence (current value in Table_T).
 * Returns: nothing.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void free_sequence(const void* key, void** value, void* cl)
{
    (void) key;
    (void) cl;
    Seq_T seq = (Seq_T) *value;
    Info *info = NULL;

    /* Free all values (Info Structs) in the Sequence */
    for (int i = 0; i < Seq_length(seq); i++) {
        info = Seq_get(seq, i);
        free(info);
    }

    /* Free memory used by the Sequence */
    Seq_free(&seq);
    seq = NULL;
    info = NULL;
}
