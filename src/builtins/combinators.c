#include <builtins/combinators.h>

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

// necessary? Should this just be evalc?
void cog_eval() {
  contain_t *cur = stack_peek(STACK);
  value_t *v = stack_pop(cur->stack);
  if (!cur->stack->size) {
    eval_error("EMPTY STACK");
    return;
  }
  stack_push(EVAL_STACK, v);
  for (int i = 0; i < v->container->stack->size; i++) {
    value_t *newval = v->container->stack->items[i];
    switch (newval->type) {
      case VWORD:
        evalword(newval, cur, v->container);
        break;
      case VSTACK:
        contain_push(cur, newval);
        crank();
        break;
      case VCLIB:
        ((void(*)())(newval->custom))();
        crank();
        break;
      default:
        push_quoted(cur, newval);
        crank();
    }
  }
  value_t *vf = stack_pop(EVAL_STACK);
  if (vf) {
    value_free(vf);
  }
}

void cog_evalc() {
  contain_t *cur = stack_peek(STACK);
  value_t *v = stack_pop(cur->stack);
  if (!cur->stack->size) {
    eval_error("EMPTY STACK");
    return;
  }
  stack_push(EVAL_STACK, v);
  evalstack(v->container, cur);
  value_t *vf = stack_pop(EVAL_STACK);
  if (vf) {
    value_free(vf);
  }
}

void cog_child() {
  contain_t *cur = stack_peek(STACK);
  value_t *v = init_value(VSTACK);
  v->container = calloc(1, sizeof(contain_t));
  if (c->word_table->size) {
    v->container->word_table = ht_copy(c->word_table, contain_value_copy);
  } else {
    v->container->word_table = init_ht(10);
  }
  if (c->flit->size) {
    v->container->flit = ht_copy(c->flit, value_stack_copy);
  } else {
    v->container->flit = init_ht(10);
  }
  v->container->cranks = stack_copy(c->cranks, cranks_copy);
  v->container->err_stack = stack_copy(c->err_stack, value_copy);
  v->container->stack = stack_copy(c->stack, value_copy);
  v->container->faliases = stack_copy(c->faliases, (void*(*)(void*))string_copy);
  v->container->delims = string_copy(c->delims);
  v->container->ignored = string_copy(c->ignored);
  v->container->dflag = c->dflag;
  v->container->iflag = c->iflag;
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
