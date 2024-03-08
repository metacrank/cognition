#include <builtins/stackops.h>

extern stack_t *STACK;

void cog_nop(value_t *v) {}

void cog_dsc(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  value_t *v1 = stack_pop(stack);
  if (!v1) {
    /* TODO: error */
    return;
  }
  value_free(v1);
}

void cog_dup(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  value_t *v1 = stack_pop(stack);
  if (!v1) {
    return;
  }
  stack_push(stack, value_copy(v1));
  stack_push(stack, v1);
}

void add_funcs_stackops(ht_t* flit) {
  add_func(flit, cog_nop, "nop");
  add_func(flit, cog_dsc, "dsc");
  add_func(flit, cog_dsc, "dup");
}
