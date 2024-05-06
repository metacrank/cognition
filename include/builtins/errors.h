#ifndef ERRORS_H_
#define ERRORS_H_

#include <cognition.h>

/* cleans entire error stack */
void cog_err_clean(value_t *v);

/* returns the value of the last error on the error stack */
void cog_err_peek(value_t *v);

/* pops a value from the error stack and pushes it to the stack */
void cog_err_pop(value_t *v);

/* takes a word and converts it to a VERR, pushes it to the error stack */
void cog_err_push(value_t *v);

/* drops an error from the error stack */
void cog_err_drop(value_t *v);

/* prints top value on error stack */
void cog_err_print(value_t *v);

/* prints all errors on error stack */
void cog_err_show(value_t *v);

/* puts an error on the error stack with the current word's value */
void cog_err_throw(value_t *v);

/* returns the size of the current error stack */
void cog_err_size(value_t *v);

void add_funcs_errors(ht_t *flit);

#endif // ERRORS_H_
