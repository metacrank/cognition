#include <builtins/parser.h>
#include <stdio.h>

extern stack_t *STACK;

void cog_d(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *stack = stack_pop(cur->stack);
  if (stack == NULL) {
    eval_error("EMPTY STACK", v);
    return;
  }
  if (stack->container->stack->size != 1) {
    stack_push(cur->stack, stack);
    eval_error("TYPE ERROR", v);
    return;
  }
  value_t *word = stack->container->stack->items[0];
  if (word->type != VWORD) {
    stack_push(cur->stack, stack);
    eval_error("TYPE ERROR", v);
    return;
  }
  string_free(cur->delims);
  cur->delims = string_copy(word->str_word);
  value_free(stack);
}

void cog_i(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *stack = stack_pop(cur->stack);
  if (stack == NULL) {
    eval_error("EMPTY STACK", v);
    return;
  }
  if (stack->container->stack->size != 1) {
    stack_push(cur->stack, stack);
    eval_error("TYPE ERROR", v);
    return;
  }
  value_t *word = stack->container->stack->items[0];
  if (word->type != VWORD) {
    stack_push(cur->stack, stack);
    eval_error("TYPE ERROR", v);
    return;
  }
  string_free(cur->ignored);
  cur->ignored = string_copy(word->str_word);
  value_free(word);
}

void cog_dtgl(value_t *v) {
  contain_t *c = stack_peek(STACK);
  c->dflag = !c->dflag;
}

void cog_itgl(value_t *v) {
  contain_t *c = stack_peek(STACK);
  c->iflag = !c->iflag;
}

void cog_geti(value_t *v) {
  value_t *list = init_value(VWORD);
  contain_t *cur = stack_peek(STACK);
  list->str_word = string_copy(cur->ignored);
  if (list->str_word == NULL)
    list->str_word = init_string("");
  push_quoted(cur, list);
}

void cog_getd(value_t *v) {
  value_t *list = init_value(VWORD);
  contain_t *cur = stack_peek(STACK);
  list->str_word = string_copy(cur->delims);
  if (list->str_word == NULL)
    list->str_word = init_string("");
  push_quoted(cur, list);
}

void add_funcs_parser(ht_t* flit) {
  add_func(flit, cog_d, "d");
  add_func(flit, cog_i, "i");
  add_func(flit, cog_dtgl, "dtgl");
  add_func(flit, cog_itgl, "itgl");
  add_func(flit, cog_geti, "geti");
  add_func(flit, cog_getd, "getd");
}
