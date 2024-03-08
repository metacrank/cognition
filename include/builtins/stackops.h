#ifndef BUILTINS_STACKOPS_H_
#define BUILTINS_STACKOPS_H_

#include <cognition.h>

// note: can generalise the behaviour of 'word'd with n'word' if desired


/*! @brief does literally nothing */
void cog_nop(value_t *v);

/*! @brief clears the stack and frees all elements */
void cog_clear(value_t *v);

/*! @brief discards and frees the first element(s) of the stack */
void cog_dsc(value_t *v);
/* void cog_dsc2(value_t *v); */
/* void cog_dsc3(value_t *v); */
/* void cog_dsc4(value_t *v); */
/* void cog_dscn(value_t *v); */

/* discarding values further down the stack */
void cog_nip(value_t *v); // dscd
/* void cog_nip2(value_t *v); */
/* void cog_nip3(value_t *v); */
/* void cog_nip4(value_t *v); */
/* void cog_nipn(value_t *v); */

void cog_nipd(value_t *v); // dscdd
/* void cog_nipd2(value_t *v); */
/* void cog_nipd3(value_t *v); */
/* void cog_nipd4(value_t *v); */
/* void cog_nipdn(value_t *v); */

/*! @brief swaps the top two values of the stack */
void cog_swap(value_t *v);
/* void cog_swapd(value_t *v); */
/* void cog_swapdd(value_t *v); */

/* reverses order of more than two elements */
/* void cog_spin(value_t *v); */
/* void cog_spin4(value_t *v); */
/* void cog_spinn(value_t *v); */

/* void cog_spind(value_t *v); */
/* void cog_spind4(value_t *v); */
/* void cog_spindn(value_t *v); */

/*! @brief deep copies first value of the stack */
void cog_dup(value_t *v);
/* void cog_dup2(value_t *v); */
/* void cog_dup3(value_t *v); */
/* void cog_dup4(value_t *v); */
/* void cog_dupn(value_t *v); */

/* void cog_dupd(value_t *v); */
/* void cog_dupd2(value_t *v); */
/* void cog_dupd3(value_t *v); */
/* void cog_dupd4(value_t *v); */
/* void cog_dupdn(value_t *v); */

/* void cog_dupdd(value_t *v); */
/* void cog_dupdd2(value_t *v); */
/* void cog_dupdd3(value_t *v); */
/* void cog_dupdd4(value_t *v); */
/* void cog_dupddn(value_t *v); */

/* deep copies values deeper in the stack and pushes them */
/* void cog_over(value_t *v); */
/* void cog_pick(value_t *v); */
/* void cog_reach(value_t *v); */
/* void cog_pickn(value_t *v); */

/* void cog_overd(value_t *v); */
/* void cog_pickd(value_t *v); */
/* void cog_reachd(value_t *v); */
/* void cog_pickdn(value_t *v); */

/* void cog_overdd(value_t *v); */
/* void cog_pickdd(value_t *v); */
/* void cog_reachdd(value_t *v); */
/* void cog_pickddn(value_t *v); */

/* rotates consecutive elements */
/* void cog_rot(value_t *v); // x y z --> y z x */
/* void cog_negrot(value_t *v); // call as '-rot' */
/* void cog_roll(value_t *v); // w x y z --> x y z w */
/* void cog_negroll(value_t *v); // call as '-roll' */
/* void cog_rotn(value_t *v); */
/* void cog_negrotn(value_t *v); */

/* void cog_rotd(value_t *v); */
/* void cog_negrotd(value_t *v); */
/* void cog_rolld(value_t *v); */
/* void cog_negrolld(value_t *v); */
/* void cog_rotdn(value_t *v); */
/* void cog_negrotdn(value_t *v); */


/*! @brief returns current stack size */
void cog_ssize(value_t *v);

/*! @brief returns type of top stack element as an integer */
void cog_type(value_t *v);


/* adds stack operation fuctions to flit */
void add_funcs_stackops(ht_t *flit);

#endif // BUILTINS_STACKOPS_H_
