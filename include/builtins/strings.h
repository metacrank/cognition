#ifndef STRINGS_H_
#define STRINGS_H_

#include <cognition.h>

/* splits string in two */
void cog_cut(value_t *v);

/* string concatenation */
void cog_concat(value_t *v);

/* decompose string */
void cog_unconcat(value_t *v);

/* previously cog_nth */
void cog_cat(value_t *v);

/* length of a string */
void cog_len(value_t *v);

/* inserts a string into another string */
void cog_insert(value_t *v);

/* reverses order of characters in string */
void cog_reverse(value_t *v);

/* checks if a value is a string */
void cog_isword(value_t *v);

/* turns a single character into a numerical value */
void cog_btoi(value_t *v);

/* turns a numerical byte into a character */
void cog_itob(value_t *v);

void add_funcs_strings(ht_t *flit);

#endif // STRINGS_H_
