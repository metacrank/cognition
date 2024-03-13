#include <builtins/strings.h>
#include <builtinslib.h>

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

void cog_nth(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (stack->size < 2) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *v1 = stack_pop(stack);
  if (v1->container->stack->size != 1) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    return;
  }
  value_t *w1 = v1->container->stack->items[0];
  if (w1->type != VWORD) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    return;
  }
  if (!strisint(w1->str_word)) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    return;
  }
  value_t *quot = stack_peek(stack);
  if (quot->container->stack->size != 1) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    return;
  }
  value_t *wq = quot->container->stack->items[0];
  if (wq->type != VWORD) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    return;
  }
  string_t *str = wq->str_word;
  int n = atoi(w1->str_word->value);
  if (n < 0 || n >= str->length) {
    eval_error("INDEX OUT OF RANGE", v);
    stack_push(stack, v1);
    return;
  }
  value_free(v1);
  str->value[0] = str->value[n];
  str->value[1] = '\0';
  str->length = 1;
}


void add_funcs_strings(ht_t *flit) {
  add_func(flit, cog_concat, "concat");
  add_func(flit, cog_nth, "nth");
}
