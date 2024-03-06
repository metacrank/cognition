#include <stackops.h>

void cog_nop(value_t *v) {}

void add_funcs_stackops(ht_t* flit) {
  add_func(flit, cog_nop, "nop");
}
