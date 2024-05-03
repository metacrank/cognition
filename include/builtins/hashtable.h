#ifndef BUILTINS_HASHTABLE_H_
#define BUILTINS_HASHTABLE_H_

#include <cognition.h>

/* adds an entry for word with child at the top of the stack */
/* if child is a stack, store in word table, if child is a macro, store in flit */
void cog_def(value_t *v);

/* works on builtins/aliases */
void cog_undef(value_t *v);

/* if the word is defined in the word table, returns the definition of the word */
void cog_unglue(value_t *v);

/* returns false if and only if the word is not defined anywhere */
void cog_isdef(value_t *v);

/* returns an unordered list of the words defined in the current environment */
/* should this return two separate lists, one for each the word_table and the flit? */
void cog_wordlist(value_t *v);
/* stackwordlist */
/* macrowordlist */

/* copies a definition/alias/falias in the current stack to a child stack */
void cog_bequeath(value_t *v);

/* adds hashtable fuctions to flit */
void add_funcs_hashtable(ht_t *flit);

#endif // BUILTINS_HASHTABLE_H_
