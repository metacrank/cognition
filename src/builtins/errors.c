#include <builtins/errors.h>
#include <cognition.h>

extern stack_t *STACK;

void cog_err_clean(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *estack = cur->err_stack;
  for (int i = 0; i < estack->size; i ++) {
    value_free(estack->items[i]);
  }
  estack->size = 0;
}

void cog_err_peek(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *estack = cur->err_stack;
  stack_t *stack = cur->stack;
  value_t *v1 = stack_peek(estack);
  value_t *r1 = init_value(VWORD);
  value_t *r2 = init_value(VWORD);
  r1->str_word = string_copy(v1->error->error);
  r2->str_word = string_copy(v1->error->str_word);
  stack_push(stack, r1);
  stack_push(stack, r2);
}

void cog_err_pop(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *estack = cur->err_stack;
  stack_t *stack = cur->stack;
  value_t *v1 = stack_pop(estack);
  value_t *r1 = init_value(VWORD);
  value_t *r2 = init_value(VWORD);
  r1->str_word = v1->error->error;
  r2->str_word = v1->error->str_word;
  push_quoted(cur, r1);
  push_quoted(cur, r2);
  v1->error->str_word = NULL;
  v1->error->error = NULL;
  value_free(v1);
}

void cog_err_push(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *estack = cur->err_stack;
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
    push_quoted(cur, v1);
    push_quoted(cur, v2);
    return;
  }
  value_t *w1 = v1->container->stack->items[0];
  if (w1->type != VWORD) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    stack_push(stack, v2);
    return;
  }
  value_t *e = init_value(VERR);
  e->error = calloc(1, sizeof(error_t));
  e->error->str_word = w2->str_word;
  e->error->error = w1->str_word;
  w2->str_word = NULL;
  w1->str_word = NULL;
  value_free(v1);
  value_free(v2);
  stack_push(estack, e);
}

void add_funcs_errors(ht_t *flit) {
  add_func(flit, cog_err_clean, "clean");
  add_func(flit, cog_err_push, "epush");
  add_func(flit, cog_err_pop, "epop");
  add_func(flit, cog_err_peek, "epeek");
}
