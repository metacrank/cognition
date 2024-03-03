#ifndef BUILTINS_COMBINATORS_H_
#define BUILTINS_COMBINATORS_H_

#include <builtins.h>

/* pops the first element on the stack and pushes a child stack containing that element */
/* what configuration of definitions are passed down? */
void cog_quote();

/* calls each word in the child stack, or else pushes the value to the stack, using the
 * current stack's hastable with precedence over the child's hashtable */
void cog_eval();

/* same as cog_eval, except uses the child stack's hashtable (only? or just with precedence?) */
void cog_evalc();

/* creates a child stack inheriting the current stack's hashtable */
void cog_child();

/* creates a child stack with a blank hashtable (include builtins?) */
void cog_stack();

/* takes a list of words as a child stack and creates a new child stack with those words defined in its hashtable */
void cog_wstack();

/* takes a list of words as a child stack and creates a new child stack inheriting all but those words */
void cog_bstack();

/* creates a child stack with the default cognition environment */
void cog_sub();

void cog_compose();

void cog_strstack();

/*! @brief reads a file into a string, returns on the stack */
void cog_fread();

/* @brief [value] [quote] curry  */
void cog_curry();

/* @brief Inserts a value into a string or quote. */
/* call with `[quote/string] value index insert` */
void cog_insert();

void cog_dip();

/*! @brief keeps value on the stack after evaluating the value */
void keep();

/*! @brief [cond] [if true] [if false] if; evaluates 2nd term if cond is true;
 * otherwise evaluates the other one. */
void cog_if();

/* executes child, then pops element off stack and if true executes the same child again, in a loop */
void cog_loop();

/* [cond] [body] while; executes [body] while [cond] returns true, pops [cond] return value */
void cog_while();

/* splits child in two */
void cog_split();

/* adds combinator builtins to flit */
void add_funcs_combinators(ht_t* flit);

#endif // BUILTINS_COMBINATORS_H_
