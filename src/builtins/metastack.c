#include <builtins/metastack.h>
#include <builtinslib.h>
#include <macros.h>
#include <math.h>
#include <limits.h>

extern stack_t *STACK;

void cog_cd(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *child = stack_peek(cur->stack);
  if (child == NULL) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  stack_push(STACK, child->container);
}

void cog_ccd(value_t *v) {
  contain_t *cur = stack_pop(STACK);
  value_t *child = stack_peek(cur->stack);
  if (child == NULL) {
    stack_push(STACK, cur);
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  stack_push(STACK, child->container);
  child->container = NULL;
  contain_free(cur);
}

void cog_uncd(value_t *v) {
  contain_t *old = stack_pop(STACK);
  if (STACK->size == 0) {
    contain_t *root = calloc(1, sizeof(contain_t));
    contain_copy_attributes(old, root);
    root->stack = init_stack(DEFAULT_STACK_SIZE);
    value_t *oldroot = init_value(VSTACK);
    oldroot->container = old;
    stack_push(root->stack, oldroot);
    stack_push(STACK, root);
  }
}

void cog_pop(value_t *v) {
  contain_t *old = stack_pop(STACK);
  value_t *popval = stack_pop(old->stack);
  if (!popval) {
    eval_error("TOO FEW ARGUMENTS", v);
    stack_push(old->stack, popval);
    stack_push(STACK, old);
  }
  if (STACK->size == 0) {
    contain_t *root = calloc(1, sizeof(contain_t));
    contain_copy_attributes(old, root);
    root->stack = init_stack(DEFAULT_STACK_SIZE);
    value_t *oldroot = init_value(VSTACK);
    oldroot->container = old;
    stack_push(root->stack, oldroot);
    stack_push(STACK, root);
  }
  contain_t *newc = stack_peek(STACK);
  stack_push(newc->stack, popval);
}

void cog_qstack(value_t *v) {
  contain_t *old = stack_pop(STACK);
  contain_t *new = calloc(1, sizeof(contain_t));
  contain_copy_attributes(old, new);
  new->stack = init_stack(DEFAULT_STACK_SIZE);
  value_t *oldval = init_value(VSTACK);
  oldval->container = old;
  stack_push(new->stack, oldval);
  stack_push(STACK, new);
}

void cog_pcd(value_t *v) {

}

void cog_gstack(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *ptrval = init_value(VWORD);
  int ptrsize = sizeof(contain_t *) * CHAR_BIT / 4 + 2;
  char buf[ptrsize + 1];
  snprintf(buf, ptrsize + 1, "%p", cur);
  ptrval->str_word = init_string(buf);
  push_quoted(cur, ptrval);
}

void add_funcs_metastack(ht_t *flit) {
  add_func(flit, cog_cd, "cd");
  add_func(flit, cog_ccd, "ccd");
  add_func(flit, cog_uncd, "uncd");
  add_func(flit, cog_pop, "pop");
  add_func(flit, cog_qstack, "qstack");
  add_func(flit, cog_gstack, "gstack");
}
