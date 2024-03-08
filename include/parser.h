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

/* set delims */
void cog_d(value_t *v);

/* set ignored */
void cog_i(value_t *v);

/* flip delim flag */
void cog_dtgl(value_t *v);

/* flip ignore flag */
void cog_itgl(value_t *v);

/* get list of ignored */
void cog_geti(value_t *v);

/* get list of delims */
void cog_getd(value_t *v);

/* goes through list of characters in word, if c is not a delim, set it to be a delim */
void cog_delim(value_t *v);

/* same for ignored characters */
void cog_ignore(value_t *v);

/* if c in word is a delim, remove it from delim string */
void cog_undelim(value_t *v);

/* same for ignored characters */
void cog_unignore(value_t *v);

/* parse and eval string */
void evalstr(value_t *v);

/* adds parser fuctions to flit */
void add_funcs_parser(ht_t *flit);

#endif // BUILTINS_PARSER_H_
