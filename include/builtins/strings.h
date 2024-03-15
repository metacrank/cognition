#ifndef STRINGS_H_
#define STRINGS_H_

#include <cognition.h>

/* splits string in two */
void cog_cut(value_t *v);
/* void cog_cut0(value_t *v); */
/* void cog_cut1(value_t *v); */
/* void cog_cut2(value_t *v); */
/* void cog_cut3(value_t *v); */

/* void cog_cutst(value_t *v); */
/* void cog_cut0(value_t *v); */
/* void cog_cut1(value_t *v); */
/* void cog_cut2(value_t *v); */
/* void cog_cut3(value_t *v); */

/* string concatenation */
void cog_concat(value_t *v);
/* void cog_concat2(value_t *v); */
/* void cog_concat3(value_t *v); */
/* void cog_concat4(value_t *v); */
/* void cog_concatn(value_t *v); */

/* void cog_precat(value_t *v); */
/* void cog_precat2(value_t *v); */
/* void cog_precat3(value_t *v); */
/* void cog_precat4(value_t *v); */
/* void cog_precatn(value_t *v); */

/* void cog_join(value_t *v); */

/* decompose string */
void cog_unconcat(value_t *v);

/* gets nth values */
/* void cog_cfirst(value_t *v); */
/* void cog_csecond(value_t *v); */
/* void cog_cthird(value_t *v); */
/* void cog_cfourth(value_t *v); */
/* void cog_clast(value_t *v); */
/* void cog_csecond_last(value_t *v); */
/* void cog_cthird_last(value_t *v); */
/* void cog_cfourth_last(value_t *v); */

void cog_nth(value_t *v);
//or
//void cog_cat(value_t *v); // cat may be useful for [ fread . ] word?
//or
//void cog_cvat(value_t *v);

/* length of a string */
void cog_len(value_t *v);

/* inserts a string into another string */
void cog_insert(value_t *v);

/*! @brief checks if string is a number. */
void cog_isnum(value_t *v);

void add_funcs_strings(ht_t *flit);

#endif // STRINGS_H_
