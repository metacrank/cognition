#ifndef BUILTINS_HASHTABLE_H_
#define BUILTINS_HASHTABLE_H_

#include <cognition.h>

/* adds an entry for word with child at the top of the stack, recording the current stack's pointer */
void cog_def(value_t *v);

/* works on builtins/aliases */
void cog_undef(value_t *v);

/* if the word is associated with the current stack, returns the definition of the word */
/* should this be changed to allow the ungluing of any word for the default environment?
 * probably not
 * because we can always have a word defined with [ a ] [ [ quote ] eval ] def,
 * where evaluating it in crank 2: 'a dsc 1 crank'  will return [ quote ]
 */
void cog_unglue(value_t *v);

/* returns false if and only if the word is not defined */
void cog_isdef(value_t *v);

/* turns a child stack definition into a 'builtin' */
void cog_alias(value_t *v);

/* returns an unordered list of the words defined in the current environment */
void cog_wordlist(value_t *v);

/* def in stack given a stack pointer, storing given pointer with word */
/* [ ptr ] [ word ] [ body ] defin */
void cog_defin(value_t *v);

/* undef in given stack */
/* [ ptr ] [ word ] */
void cog_undefin(value_t *v);

/* unglue word as defined in stack, if stack given is where word was defined */
void cog_unglue_from(value_t *v);

/* ... */
void cog_isdefin(value_t *v);
void cog_aliasin(value_t *v);

/* returns wordlist from stack pointer */
void cog_wordlist_of(value_t *v);

/* copies a definition/alias/falias in the current stack to a child stack */
void cog_bequeath(value_t *v);

/* copies a definition/alias/falias to the given stack pointer */
void cog_bequeath_to(value_t *v);

/* adds hashtable fuctions to flit */
void add_funcs_hashtable(ht_t *flit);

#endif // BUILTINS_HASHTABLE_H_
