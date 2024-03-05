#ifndef BUILTINS_PARSER_H_
#define BUILTINS_PARSER_H_

#include <cognition.h>

/* returns a list of f aliases */
void cog_getf(value_t *v);

/* sets the f alias list to elt */
void cog_setf(value_t *v);

/* if elt is not in the f alias list, add it */
void cog_aliasf(value_t *v);

/* if elt is in the f alias list, remove it */
void cog_unaliasf(value_t *v);

void cog_d(value_t *v);

void cog_i(value_t *v);

void cog_dflag(value_t *v);

void cog_iflag(value_t *v);

void cog_geti(value_t *v);

void cog_getd(value_t *v);

/* adds parser fuctions to flit */
void add_funcs_parser(ht_t *flit);

#endif // BUILTINS_PARSER_H_
