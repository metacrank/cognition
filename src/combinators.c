// #include <builtins/combinators.h>
#include <combinators.h>

extern stack_t *STACK;
extern stack_t *EVAL_STACK;

/* passes down nothing. should make child? */
void cog_quote() {
  contain_t *cur = stack_peek(STACK);
  value_t *v = stack_pop(cur->stack);
  if (v == NULL) {
    eval_error("EMPTY STACK");
    return;
  }
  push_quoted(cur, v);
}

void cog_eval() {
  contain_t *cur = stack_peek(STACK);
  value_t *v = stack_pop(cur->stack);
  if (!cur->stack->size) {
    eval_error("EMPTY STACK");
    return;
  }
  stack_push(EVAL_STACK, v);
  stack_t *family = init_stack(10);
  stack_push(family, cur);
  evalstack(v->container, family);
  value_t *vf = stack_pop(EVAL_STACK);
  if (vf) {
    value_free(vf);
  }
}

void cog_child() {
  contain_t *cur = stack_peek(STACK);
  value_t *v = init_value(VSTACK);
  v->container = calloc(1, sizeof(contain_t));
  if (cur->word_table->size) {
    v->container->word_table = ht_copy(cur->word_table, contain_value_copy);
  } else {
    v->container->word_table = init_ht(10);
  }
  if (cur->flit->size) {
    v->container->flit = ht_copy(cur->flit, value_stack_copy);
  } else {
    v->container->flit = init_ht(10);
  }
  v->container->cranks = stack_copy(cur->cranks, cranks_copy);
  v->container->err_stack = stack_copy(cur->err_stack, value_copy);
  v->container->stack = stack_copy(cur->stack, value_copy);
  v->container->faliases = stack_copy(cur->faliases, (void*(*)(void*))string_copy);
  v->container->delims = string_copy(cur->delims);
  v->container->ignored = string_copy(cur->ignored);
  v->container->dflag = cur->dflag;
  v->container->iflag = cur->iflag;
  contain_push(cur, v);
}

void cog_stack() {
  contain_t *cur = stack_peek(STACK);
  value_t *v = init_value(VSTACK);
  v->container = init_contain(init_ht(10), init_ht(10), init_stack(10));
  contain_push(cur, v);
}

void add_funcs_combinators(ht_t* flit) {
  add_func(flit, cog_quote, "quote");
  add_func(flit, cog_eval, "eval");
  add_func(flit, cog_child, "child");
  add_func(flit, cog_stack, "stack");
}
