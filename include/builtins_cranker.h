#ifndef BUILTINS_CRANKER_H_
#define BUILTINS_CRANKER_H_

#include <cognition.h>

/* n m metacrank sets metacrank n to period m */
void cog_metacrank(value_t *v);

/*sets 0th crank value to specified period*/
void cog_crank(value_t *v);

/* sets all crank values to specified period */
void cog_crankall(value_t *v);

/* instant 0 crankall */
void cog_reset(value_t *v);

/* evaluate child instantly if it is not high tide */
void cog_f(value_t *v);

/* returns the number of operations in a crank cycle */
void cog_crankbase(value_t *v);

/* returns the number of operations done since high tide */
void cog_modcrank(value_t *v);

/* returns the number of nth metacranks per cycle */
void cog_metacrankbase(value_t *v);

/* returns the number of operations done since nth metacrank high tide */
void cog_metamodcrank(value_t *v);

#endif // BUILTINS_CRANKER_H_
