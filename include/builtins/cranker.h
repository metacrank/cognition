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
void cog_halt(value_t *v);

/* set child crank */
/* void cog_metacrankc(value_t *v); */
/* instant compose compose cd metacrank uncd */
/* void cog_crankc(value_t *v); */
/* instant compose cd crank uncd */
/* void cog_crankallc(value_t *v); */
/* instant compose cd crankall uncd */
/* void cog_haltc(value_t *v); */
/* instant cd halt uncd */

/* returns the number of operations in a crank cycle */
void cog_crankbase(value_t *v);

/* returns the number of operations done since high tide */
void cog_modcrank(value_t *v);

/* returns the number of nth metacranks per cycle */
void cog_metacrankbase(value_t *v);

/* returns the number of operations done since nth metacrank high tide */
void cog_metamodcrank(value_t *v);

/* get child crank */
/* void cog_crankbase_child(value_t *v); */
/* void cog_modcrank_child(value_t *v); */
/* void cog_metacrankbase_child(value_t *v); */
/* void cog_metamodcrank_child(value_t *v); */

/* adds cranker fuctions to flit */
void add_funcs_cranker(ht_t *flit);

#endif // BUILTINS_CRANKER_H_
