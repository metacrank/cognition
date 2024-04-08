#ifndef BUILTINS_MISC_H_
#define BUILTINS_MISC_H_

#include <cognition.h>

/* reset to baremetal cognition settings, definitions, and builtins */
void cog_reset(value_t *v);

/* adds a c library to the object table */
void cog_clib(value_t *v);

/* pops out of any stacks currently being executed and returns to the top of STACK */
void cog_return(value_t *v);

/* returns a list of arguments passed to the program */
void cog_getargs(value_t *v);

/* adds misc fuctions to flit */
void add_funcs_misc(ht_t *flit);

#endif // BUILTINS_MISC_H_
