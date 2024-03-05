#include <parser.h>

extern stack_t *STACK;

void cog_d(value_t *v);

void cog_i(value_t *v);

void cog_dflag(value_t *v) {
  contain_t *c = stack_peek(STACK);
  c->dflag = !c->dflag;
}

void cog_iflag(value_t *v) {
  contain_t *c = stack_peek(STACK);
  c->iflag = !c->iflag;
}

void cog_geti(value_t *v);

void cog_getd(value_t *v);

void add_funcs_parser(ht_t* flit) {
}
