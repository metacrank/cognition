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

#endif // BUILTINS_PARSER_H_
