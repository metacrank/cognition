#include <builtins/stackops.h>
#include <builtinslib.h>
#include <strnum.h>
#include <pool.h>

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

void cog_drop(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  value_t *v1 = stack_pop(stack);
  if (!v1) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  value_free_safe(v1);
}

void cog_swap(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (stack->size < 2) {
    eval_error(U"TOO FEW ARGUMENTS", v);
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
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  stack_push(stack, value_copy(v1));
}

void cog_ssize(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  size_t size = stack->size;
  value_t *sizeval = pool_req(DEFAULT_STRING_LENGTH, POOL_VWORD);
  int_to_string_buf(size, sizeval->str_word);
  push_quoted(cur, sizeval);
}

void add_funcs_stackops(ht_t* flit) {
  add_func(flit, cog_nop, U"nop");
  add_func(flit, cog_clear, U"clear");
  add_func(flit, cog_drop, U"drop");
  add_func(flit, cog_swap, U"swap");
  add_func(flit, cog_dup, U"dup");
  add_func(flit, cog_ssize, U"ssize");
}
