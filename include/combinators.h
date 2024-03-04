#ifndef BUILTINS_COMBINATORS_H_
#define BUILTINS_COMBINATORS_H_

#include <builtins.h>

/* pops the first element on the stack and pushes a child stack containing that element */
/* what configuration of definitions are passed down? */
void cog_quote(value_t *v)
;

/* calls each word in the child stack, or else pushes the value to the stack, using the
 * current stack's hastable with precedence over the child's hashtable */
void cog_eval(value_t *v)
;

/* same as cog_eval, except uses the child stack's hashtable (only? or just with precedence?) */
void cog_evalc(value_t *v)
;

/* creates a child stack inheriting the current stack's hashtable */
void cog_child(value_t *v)
;

/* creates a child stack with a blank hashtable (include builtins?) */
void cog_stack(value_t *v)
;

/* takes a list of words as a child stack and creates a new child stack with those words defined in its hashtable */
void cog_wstack(value_t *v)
;

/* takes a list of words as a child stack and creates a new child stack inheriting all but those words */
void cog_bstack(value_t *v)
;

/* creates a child stack with the default cognition environment */
void cog_sub(value_t *v)
;

void cog_compose(value_t *v)
;

void cog_strstack(value_t *v)
;

/*! @brief reads a file into a string, returns on the stack */
void cog_fread(value_t *v)
;

/* @brief [value] [quote] curry  */
void cog_curry(value_t *v)
;

/* @brief Inserts a value into a string or quote. */
/* call with `[quote/string] value index insert` */
void cog_insert(value_t *v)
;

void cog_dip(value_t *v)
;

/*! @brief keeps value on the stack after evaluating the value */
void keep(value_t *v)
;

/*! @brief [cond] [if true] [if false] if; evaluates 2nd term if cond is true;
 * otherwise evaluates the other one. */
void cog_if(value_t *v)
;

/* executes child, then pops element off stack and if true executes the same child again, in a loop */
void cog_loop(value_t *v)
;

/* [cond] [body] while; executes [body] while [cond] returns true, pops [cond] return value */
void cog_while(value_t *v)
;

/* splits child in two */
void cog_split(value_t *v)
;

/* adds combinator builtins to flit */
void add_funcs_combinators(ht_t* flit);

#endif // BUILTINS_COMBINATORS_H_
