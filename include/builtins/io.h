#ifndef BUILTINS_IO_H_
#define BUILTINS_IO_H_

#include <cognition.h>

/* prints a value_t */
void print_value(value_t *v, const char *end);

/*! @brief prints the entire stack */
void cog_questionmark(value_t *v);

/*! @brief pops first value off of the stack and prints it */
void cog_period(value_t *v);

/* prints a word-string */
/* string search and replacement formatting functions like C's printf() can be added in an io library */
void cog_print(value_t *v);

/*! @brief reads stdin into string */
void cog_read(value_t *v);

/* prints the entire stack to file */
void cog_fquestionmark(value_t *v);

/* ... */
void cog_fperiod(value_t *v);

/*! @brief reads a file into a string, returns on the stack */
void cog_fread(value_t *v);

/*! @brief writes string to file */
void cog_fwrite(value_t *v);

/* adds io fuctions to flit */
void add_funcs_io(ht_t *flit);

#endif // BUILTINS_IO_H_
