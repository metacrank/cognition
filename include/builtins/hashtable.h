#ifndef BUILTINS_HASHTABLE_H_
#define BUILTINS_HASHTABLE_H_

#include <cognition.h>

/* adds an entry for word with child at the top of the stack, recording the current stack's pointer */
void cog_def(value_t *v);

/* works on builtins/aliases */
void cog_undef(value_t *v);

/* if the word is associated with the current stack, returns the definition of the word */
/* should this be changed to allow the ungluing of any word for the default environment?
 * we could have another word to get the stack attribute of a word's definition
 */
void cog_unglue(value_t *v);

/* returns 0 if and only if the word is not defined */
void cog_isdef(value_t *v);

/* turns a child stack definition into a 'builtin' */
void cog_alias(value_t *v);

/* returns an unordered list of the words defined in the current environment */
void cog_wordlist(value_t *v);

/* adds hashtable fuctions to flit */
void add_funcs_hashtable(ht_t *flit);

#endif // BUILTINS_HASHTABLE_H_
