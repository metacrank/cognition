#ifndef BUILTINS_COMBINATORS_H_
#define BUILTINS_COMBINATORS_H_

#include <cognition.h>

/* pops the first element on the stack and pushes a child stack containing that element */
/* what configuration of definitions are passed down? */
void cog_quote(value_t *v);

/* calls each word in the child stack, or else pushes the value to the stack, using the
 * child's hashtable with precedence over the current stack */
void cog_eval(value_t *v);

/* creates a child stack inheriting the current stack's hashtable */
void cog_child(value_t *v);

/* creates a child stack with a blank hashtable (include builtins?) */
void cog_stack(value_t *v);

/* takes a list of words as a child stack and creates a new child stack
 * with those words defined in its hashtable */
void cog_wstack(value_t *v);

/* takes a list of words as a child stack and creates a new child stack
 * inheriting all but those words */
void cog_bstack(value_t *v);

/* creates a child stack with the default cognition environment */
void cog_sub(value_t *v);

void cog_compose(value_t *v);

void cog_strstack(value_t *v);

/*! @brief reads a file into a string, returns on the stack */
void cog_fread(value_t *v);

/* @brief [value] [quote] curry  */
void cog_curry(value_t *v);

/* @brief Inserts a value into a string or quote. */
/* call with `[quote/string] value index insert` */
void cog_insert(value_t *v);

void cog_dip(value_t *v);

/*! @brief keeps value on the stack after evaluating the value */
void keep(value_t *v);

/*! @brief [cond] [if true] [if false] if; evaluates 2nd term if cond is true;
 * otherwise evaluates the other one. */
void cog_if(value_t *v);

/* executes child, then pops element off stack and if true executes the same child again, in a loop */
void cog_loop(value_t *v);

/* [cond] [body] while; executes [body] while [cond] returns true, pops [cond] return value */
void cog_while(value_t *v);

/* splits child in two */
void cog_split(value_t *v);


/* From factor: */
/*
**dip
**dip2
**dip3
**dip4
**dipn
**keep
**keep2
**keep3
**keep4
**keepn
*/



/*current stack: x1...xn y1...yn q1 q2...qm
let a be the number of values in each set of x,y etc., b be the number of such sets, and c be the number of quotes to apply on each set of values.
let p be the numerical value of (bi/tri).*/

/*
then
'm(bi/tri/quad)n'  operates on a stack with a=n, b=m, c=q,
'm(bi/tri/quad)*n' operates on a stack with a=n, b=q, c=m,

'mcleaven' operates on a stack with a=n, b=m, c=(# of quotes in child stack)
'mspreadn' operates on a stack with a=n, b=(# of quotes in child stack), c=m

'l m call'    operates on a stack with a=1, b=l, c=m,
'l m n calln' operates on a stack with a=n, b=l, c=m,

'm(bi/tri/quad)&' operates on a stack with a=n, b=m, c=q, and multiplies the set of quotes by m,
'm(bi/tri/quad)@' operates on a stack with a=n, b=q, c=m, and multiplies the set of quotes by q,

'l m apply'    operates on a stack with a=1, b=l, c=m, and multiplies the set of quotes by l,
'l m n applyn' operates on a stack with a=n, b=l, c=m, and multiplies the set of quotes by l.
*/


/*
complete list of builtins:

bi
bi2
bi3
bi4
bin
tri
tri2
tri3
tri4
trin
quad
quad2
quad3
quad4
quadn
&
cleave
cleave2
cleave3
cleave4
cleaven
--
bi*
bi*2
bi*3
bi*4
bi*n
tri*
tri*2
tri*3
tri*4
tri*n
quad*
quad*2
quad*3
quad*4
quad*n
&
spread
spread2
spread3
spread4
spreadn

call
call2
call3
call4
calln

--
--

bi&
bi&2
bi&3
bi&4
bi&n
tri&
tri&2
tri&3
tri&4
tri&n
quad&
quad&2
quad&3
quad&4
quad&n
--
bi@
bi@2
bi@3
bi@4
bi@n
tri@
tri@2
tri@3
tri@4
tri@n

apply
apply2
apply3
apply4
applyn

*/










/* adds combinator builtins to flit */
void add_funcs_combinators(ht_t* flit);

#endif // BUILTINS_COMBINATORS_H_
