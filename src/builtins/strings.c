#include <builtins/strings.h>
#include <builtinslib.h>
#include <math.h>
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
  value_free_safe(c2);
}

void cog_cut(value_t *v) {
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
  value_t *quot = stack_pop(stack);
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
  value_t *r1 = init_value(VWORD);
  r1->str_word = init_string(NULL);
  for (int i = n; i < wq->str_word->length; i++) {
    string_append(r1->str_word, wq->str_word->value[i]);
  }
  wq->str_word->length = n;
  wq->str_word->value[n] = '\0';
  stack_push(stack, quot);
  push_quoted(cur, r1);
}

void cog_len(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (stack->size < 1) {
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
  value_t *r1 = init_value(VWORD);
  r1->str_word = init_string(NULL);
  int x = w1->str_word->length;
  int len = (int)((ceil(log10(x))+1)*sizeof(char));
  char *result = malloc(len + 10);
  snprintf(result, len + 1, "%d", x);
  /* string_t *s = calloc(1, sizeof(string_t)); */
  r1->str_word->bufsize = len + 10;
  r1->str_word->length = len;
  r1->str_word->value = result;
  stack_push(stack, v1);
  push_quoted(cur, r1);
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
  value_free_safe(v1);
  str->value[0] = str->value[n];
  str->value[1] = '\0';
  str->length = 1;
}


void add_funcs_strings(ht_t *flit) {
  add_func(flit, cog_concat, "concat");
  add_func(flit, cog_len, "len");
  add_func(flit, cog_cut, "cut");
  add_func(flit, cog_nth, "nth");
}
