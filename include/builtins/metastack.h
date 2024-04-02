#ifndef BUILTINS_METASTACK_H_
#define BUILTINS_METASTACK_H_

#include <cognition.h>

/* pushes the stack at the top of the stack to the stack stack */
void cog_cd(value_t *v);

/* replaces the current stack with the child stack at the top of the stack */
void cog_ccd(value_t *v);

/* pops one element from the stack stack */
void cog_uncd(value_t *v);

/* takes top of stack with you during uncd */
void cog_pop(value_t *v);

/* instant qstack reset */
/* void cog_meta(value_t *v); */

/* creates a new stack above the current one, inheriting everything, and uncd's into it */
void cog_qstack(value_t *v);

/* instant qstack 0 crankall */
/* void cog_parent(value_t *v); */

/* cd's into stack with pointer at the top of the stack */
/* void cog_pcd(value_t *v); */

/* returns a pointer to the current stack */
/* void cog_gstack(value_t *v); */

/* returns a pointer to the stack at the top of the stack */
/* coglib -- instant cd gstack pop */
/* void cog_gchild(value_t *v); */

/* returns a pointer to the root stack */
/* void cog_groot(value_t *v); */

/* returns to root stack */
void cog_root(value_t *v);

/* replaces the entire contents of the stack stack with the stack at the top of the stack */
void cog_chroot(value_t *v);

/* exits from the root stack, no matter what stack you're in */
void cog_exit(value_t *v);

/* adds metastack fuctions to flit */
void add_funcs_metastack(ht_t *flit);

#endif // BUILTINS_METASTACK_H_
