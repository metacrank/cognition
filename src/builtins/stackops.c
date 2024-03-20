#include <builtins/stackops.h>
#include <builtinslib.h>
#include <stdio.h>

extern stack_t *STACK;

void cog_nop(value_t *v) {}

void cog_clear(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  value_t *item;
  while((item = stack_pop(stack))) {
    value_free_safe(item);
  }
}

void cog_dsc(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  value_t *v1 = stack_pop(stack);
  if (!v1) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  value_free_safe(v1);
}

void cog_swap(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (stack->size < 2) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  int size = stack->size;
  value_t *tmp = stack->items[size - 2];
  stack->items[size - 2] = stack->items[size - 1];
  stack->items[size - 1] = tmp;
}

void cog_dup(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  value_t *v1 = stack_peek(stack);
  if (!v1) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  stack_push(stack, value_copy(v1));
}

/* void cog_copy(value_t *v) { */
/*   contain_t *cur = stack_peek(STACK); */
/*   stack_t *stack = cur->stack; */
/*   value_t *v1 = stack_peek(stack); */
/*   if (!v1) { */
/*     eval_error("TOO FEW ARGUMENTS", v); */
/*     return; */
/*   } */
/*   stack_push(stack, v1); */
/* } */

void cog_ssize(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  int size = stack->size;
  char buffer[11];
  snprintf(buffer, 11, "%d", size);
  value_t *sizeval = init_value(VWORD);
  sizeval->str_word = init_string(buffer);
  push_quoted(cur, sizeval);
}

void add_funcs_stackops(ht_t* flit) {
  add_func(flit, cog_nop, "nop");
  add_func(flit, cog_clear, "clear");
  add_func(flit, cog_dsc, "dsc");
  add_func(flit, cog_swap, "swap");
  add_func(flit, cog_dup, "dup");
  /* add_func(flit, cog_copy, "copy"); */
  add_func(flit, cog_ssize, "ssize");
}
