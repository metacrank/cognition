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

/* void cog_compose(value_t *v); */
/* void cog_compose2(value_t *v); */
/* void cog_compose3(value_t *v); */
/* void cog_compose4(value_t *v); */
void cog_composen(value_t *v);

/* void cog_prepose(value_t *v); */
/* void cog_prepose2(value_t *v); */
/* void cog_prepose3(value_t *v); */
/* void cog_prepose4(value_t *v); */
void cog_preposen(value_t *v);

void cog_strstack(value_t *v);

/* @brief [value] [quote] curry  */
void cog_curry(value_t *v);
/* void cog_curry2(value_t *v); */
/* void cog_curry3(value_t *v); */
/* void cog_curry4(value_t *v); */
/* void cog_curryn(value_t *v); */

/* @brief [quote] [value] push  */
void cog_push(value_t *v);
/* void cog_push2(value_t *v); */
/* void cog_push3(value_t *v); */
/* void cog_push4(value_t *v); */
/* void cog_pushn(value_t *v); */

/* curries elements further down */
void cog_with(value_t *v);
/* void cog_with2(value_t *v); */
/* void cog_with3(value_t *v); */
/* void cog_with4(value_t *v); */
/* void cog_withn(value_t *v); */

/* uncurries elements from stack */
void cog_uncurry(value_t *v);
/* void cog_uncurry2(value_t *v); */
/* void cog_uncurry3(value_t *v); */
/* void cog_uncurry4(value_t *v); */
/* void cog_uncurryn(value_t *v); */

/* @brief Inserts a value into a quote. */
/* call with `[quote] value index put` */
void cog_put(value_t *v);

void cog_dip(value_t *v);
/* void cog_dip2(value_t *v); */
/* void cog_dip3(value_t *v); */
/* void cog_dip4(value_t *v); */
/* void cog_dipn(value_t *v); */

/*! @brief keeps value on the stack after evaluating the value */
void cog_keep(value_t *v);
/* void cog_keep2(value_t *v); */
/* void cog_keep3(value_t *v); */
/* void cog_keep4(value_t *v); */
/* void cog_keepn(value_t *v); */


/*! @brief [cond] [if true] [if false] if; evaluates 2nd term if cond is true;
 * otherwise evaluates the other one. */
void cog_if(value_t *v);

void cog_when(value_t *v);

/* executes child, then pops element off stack and if true executes the same child again, in a loop */
void cog_loop(value_t *v);

/* [cond] [body] while; executes [body] while [cond] returns true, pops [cond] return value */
void cog_while(value_t *v);

void cog_until(value_t *v);

void cog_do(value_t *v);

/* splits child in two */
void cog_split(value_t *v);
/* void cog_split0(value_t *v); */
/* void cog_split1(value_t *v); */
/* void cog_split2(value_t *v); */
/* void cog_split3(value_t *v); */

/* void cog_splitst(value_t *v); */
/* void cog_splitst0(value_t *v); */
/* void cog_splitst1(value_t *v); */
/* void cog_splitst2(value_t *v); */
/* void cog_splitst3(value_t *v); */


/* gets nth values */
/* void cog_first(value_t *v); */
/* void cog_second(value_t *v); */
/* void cog_third(value_t *v); */
/* void cog_fourth(value_t *v); */
/* void cog_last(value_t *v); */
/* void cog_second_last(value_t *v); */
/* void cog_third_last(value_t *v); */
/* void cog_fourth_last(value_t *v); */

void cog_vat(value_t *v);

/*! @brief deletes value at index from child */
void cog_del(value_t *v);

/* uncompose all values in quote */
void cog_uncompose(value_t *v);

/* size of a stack */
void cog_size(value_t *v);



/*current stack: x1...xn y1...yn q1 q2...qm
let a be the number of values in each set of x,y etc., b be the number of such sets, and c be the number of quotes to apply on each set of values.
let p be the numerical value of (bi/tri).*/

/*
then
'm(bi/tri/quad)n'  operates on a stack with a=n, b=m, c=q,
'm(bi/tri/quad)*n' operates on a stack with a=n, b=q, c=m,
*/
void cog_bi(value_t *v);
void cog_bi2(value_t *v);
void cog_bi3(value_t *v);
void cog_bi4(value_t *v);
void cog_bin(value_t *v);
void cog_tri(value_t *v);
void cog_tri2(value_t *v);
void cog_tri3(value_t *v);
void cog_tri4(value_t *v);
void cog_trin(value_t *v);
void cog_quad(value_t *v);
void cog_quad2(value_t *v);
void cog_quad3(value_t *v);
void cog_quad4(value_t *v);
void cog_quadn(value_t *v);

void cog_biast(value_t *v);
void cog_biast2(value_t *v);
void cog_biast3(value_t *v);
void cog_biast4(value_t *v);
void cog_biastn(value_t *v);
void cog_triast(value_t *v);
void cog_triast2(value_t *v);
void cog_triast3(value_t *v);
void cog_triast4(value_t *v);
void cog_triastn(value_t *v);
void cog_quadast(value_t *v);
void cog_quadast2(value_t *v);
void cog_quadast3(value_t *v);
void cog_quadast4(value_t *v);
void cog_quadastn(value_t *v);

/*
'mcleaven' operates on a stack with a=n, b=m, c=(# of quotes in child stack)
'mspreadn' operates on a stack with a=n, b=(# of quotes in child stack), c=m
*/
void cog_cleave(value_t *v);
void cog_cleave2(value_t *v);
void cog_cleave3(value_t *v);
void cog_cleave4(value_t *v);
void cog_cleaven(value_t *v);

void cog_spread(value_t *v);
void cog_spread2(value_t *v);
void cog_spread3(value_t *v);
void cog_spread4(value_t *v);
void cog_spreadn(value_t *v);

/*
'l m call'    operates on a stack with a=1, b=l, c=m,
'l m n calln' operates on a stack with a=n, b=l, c=m,
*/
void cog_call(value_t *v);
void cog_call2(value_t *v);
void cog_call3(value_t *v);
void cog_call4(value_t *v);
void cog_calln(value_t *v);

/*
'm(bi/tri/quad)&' operates on a stack with a=n, b=m, c=q, and multiplies the set of quotes by m,
'm(bi/tri/quad)@' operates on a stack with a=n, b=q, c=m, and multiplies the set of quotes by q,
*/
void cog_biamp(value_t *v);
void cog_biamp2(value_t *v);
void cog_biamp3(value_t *v);
void cog_biamp4(value_t *v);
void cog_biampn(value_t *v);
void cog_triamp(value_t *v);
void cog_triamp2(value_t *v);
void cog_triamp3(value_t *v);
void cog_triamp4(value_t *v);
void cog_triampn(value_t *v);
void cog_quadamp(value_t *v);
void cog_quadamp2(value_t *v);
void cog_quadamp3(value_t *v);
void cog_quadamp4(value_t *v);
void cog_quadampn(value_t *v);
void cog_bia(value_t *v);
void cog_bia2(value_t *v);
void cog_bia3(value_t *v);
void cog_bia4(value_t *v);
void cog_bian(value_t *v);
void cog_tria(value_t *v);
void cog_tria2(value_t *v);
void cog_tria3(value_t *v);
void cog_tria4(value_t *v);
void cog_trian(value_t *v);
void cog_quada(value_t *v);
void cog_quada2(value_t *v);
void cog_quada3(value_t *v);
void cog_quada4(value_t *v);
void cog_quadan(value_t *v);

/*
'l m apply'    operates on a stack with a=1, b=l, c=m, and multiplies the set of quotes by l,
'l m n applyn' operates on a stack with a=n, b=l, c=m, and multiplies the set of quotes by l.
*/
void cog_apply(value_t *v);
void cog_apply2(value_t *v);
void cog_apply3(value_t *v);
void cog_apply4(value_t *v);
void cog_applyn(value_t *v);

/* [ q1 q2 ... q3 ] n &&n applies each quote in a child stack on the top n items of the stack and returns the output of the last quote if all return true */

void cog_and0(value_t *v);
void cog_and1(value_t *v);
void cog_and2(value_t *v);
void cog_and3(value_t *v);
void cog_and4(value_t *v);
void cog_andn(value_t *v);

/* similar but for at least one truth value */
void cog_or0(value_t *v);
void cog_or1(value_t *v);
void cog_or2(value_t *v);
void cog_or3(value_t *v);
void cog_or4(value_t *v);
void cog_orn(value_t *v);


/* adds combinator builtins to flit */
void add_funcs_combinators(ht_t* flit);

#endif // BUILTINS_COMBINATORS_H_
