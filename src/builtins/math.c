#include <builtins/math.h>
#include <string.h>

extern stack_t *STACK;

void cog_equals(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (stack->size < 2) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *v2 = stack_pop(stack);
  if (v2->container->stack->size != 1) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v2);
    return;
  }
  value_t *w2 = v2->container->stack->items[0];
  if (w2->type != VWORD) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v2);
    return;
  }
  value_t *v1 = stack_pop(stack);
  if (v1->container->stack->size != 1) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    stack_push(stack, v2);
    return;
  }
  value_t *w1 = v1->container->stack->items[0];
  if (w1->type != VWORD) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    stack_push(stack, v2);
    return;
  }
  if (strcmp(w1->str_word->value, w2->str_word->value) == 0) {
    w1->str_word->length = 1;
    w1->str_word->value[0] = 't';
    w1->str_word->value[1] = '\0';
  } else {
    w1->str_word->length = 0;
    w1->str_word->value[0] = '\0';
  }
  value_free(v2);
  stack_push(stack, v1);
}

void add_funcs_math(ht_t *flit) {
  add_func(flit, cog_equals, "=");
}
