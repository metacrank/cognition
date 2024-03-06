#include <parser.h>
#include <stdio.h>

extern stack_t *STACK;

void cog_d(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *stack = stack_pop(cur->stack);
  if (stack == NULL) {
    eval_error("EMPTY STACK");
    return;
  }
  if (stack->container->stack->size != 1) {
    value_free(stack);
    /* TODO: error out */
    return;
  }
  value_t *word = stack_pop(stack->container->stack);
  value_free(stack);
  if (word->type != VWORD) {
    value_free(word);
    /* TODO: error out */
    return;
  }
  string_free(cur->delims);
  cur->delims = string_copy(word->str_word);
  value_free(word);
}

void cog_i(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *stack = stack_pop(cur->stack);
  if (stack == NULL) {
    eval_error("EMPTY STACK");
    return;
  }
  if (stack->container->stack->size != 1) {
    value_free(stack);
    /* TODO: error out */
    return;
  }
  value_t *word = stack_pop(stack->container->stack);
  value_free(stack);
  if (word->type != VWORD) {
    value_free(word);
    /* TODO: error out */
    return;
  }
  string_free(cur->ignored);
  cur->ignored = string_copy(word->str_word);
  value_free(word);
}

void cog_dflag(value_t *v) {
  contain_t *c = stack_peek(STACK);
  c->dflag = !c->dflag;
}

void cog_iflag(value_t *v) {
  contain_t *c = stack_peek(STACK);
  c->iflag = !c->iflag;
}

void cog_geti(value_t *v) {
  value_t *list = init_value(VWORD);
  contain_t *cur = stack_peek(STACK);
  list->str_word = string_copy(cur->ignored);
  push_quoted(cur, list);
}

void cog_getd(value_t *v) {
  value_t *list = init_value(VWORD);
  contain_t *cur = stack_peek(STACK);
  list->str_word = string_copy(cur->delims);
  push_quoted(cur, list);
}

void add_funcs_parser(ht_t* flit) {
  add_func(flit, cog_d, "d");
  add_func(flit, cog_i, "i");
  add_func(flit, cog_dflag, "dtgl");
  add_func(flit, cog_iflag, "itgl");
  add_func(flit, cog_geti, "geti");
  add_func(flit, cog_getd, "getd");
}
