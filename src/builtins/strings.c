#include <builtins/strings.h>

extern stack_t *STACK;

void cog_concat(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (stack->size < 2) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *c2 = stack_pop(stack);
  if (c2->container->stack->size != 1) {
    eval_error("TYPE ERROR", v);
    stack_push(stack, c2);
    return;
  }
  value_t *w2 = c2->container->stack->items[0];
  if (w2->type != VWORD) {
    eval_error("TYPE ERROR", v);
    stack_push(stack, c2);
    return;
  }
  value_t *c1 = stack_peek(stack);
  if (c1->container->stack->size != 1) {
    eval_error("TYPE ERROR", v);
    stack_push(stack, c2);
    return;
  }
  value_t *w1 = c1->container->stack->items[0];
  if (w1->type != VWORD) {
    eval_error("TYPE ERROR", v);
    stack_push(stack, c2);
    return;
  }
  string_concat(w1->str_word, w2->str_word);
  value_free(c2);
}


void add_funcs_strings(ht_t *flit) {
  add_func(flit, cog_concat, "concat");
}
