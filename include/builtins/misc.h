#ifndef BUILTINS_MISC_H_
#define BUILTINS_MISC_H_

#include <cognition.h>

/* takes a word and converts it to a VERR, pushes it to the error stack */
void cog_err_push(value_t *v);

/* pops a value from the error stack and pushes it to the stack */
void cog_err_pop(value_t *v);

/* returns the value of the last error on the error stack */
//void cog_error(value_t *v);
//or
void cog_err_peek(value_t *v);

/* quotes the entire error stack (as a copy) and returns on the stack */
//void cog_errors(value_t *v);
//or
void cog_error_stack(value_t *v);

/* cleans entire error stack */
void cog_err_clean(value_t *v);

/* adds misc fuctions to flit */
void add_funcs_misc(ht_t *flit);

#endif // BUILTINS_MISC_H_
