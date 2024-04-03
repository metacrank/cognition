#include <builtins/metastack.h>
#include <builtinslib.h>
#include <macros.h>

extern stack_t *STACK;
extern stack_t *CONTAIN_DEF_STACK;
extern string_t *EXIT_CODE;
extern bool EXITED;

void cog_cd(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *child = stack_peek(cur->stack);
  if (child == NULL) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  if (child->type != VSTACK) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(cur->stack, child);
    return;
  }
  stack_push(STACK, child->container);
  cur = child->container;
  for (int i = 0; i < cur->stack->size; i++) {
    value_t *val = cur->stack->items[i];
    if (val->type != VSTACK && val->type != VMACRO) {
      value_t *newval = init_value(VSTACK);
      newval->container = init_contain(NULL, NULL, NULL);
      stack_push(newval->container->stack, val);
      cur->stack->items[i] = newval;
    }
  }
}

void cog_ccd(value_t *v) {
  contain_t *cur = stack_pop(STACK);
  value_t *child = stack_peek(cur->stack);
  if (child == NULL) {
    stack_push(STACK, cur);
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  if (child->type != VSTACK) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(STACK, cur);
    stack_push(cur->stack, child);
    return;
  }
  stack_push(CONTAIN_DEF_STACK, cur);
  cur = child->container;
  stack_push(STACK, cur);
  child->container = NULL;
  for (int i = 0; i < cur->stack->size; i++) {
    value_t *val = cur->stack->items[i];
    if (val->type != VSTACK && val->type != VMACRO) {
      value_t *newval = init_value(VSTACK);
      newval->container = init_contain(NULL, NULL, NULL);
      stack_push(newval->container->stack, val);
      cur->stack->items[i] = newval;
    }
  }
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
    eval_error(U"TOO FEW ARGUMENTS", v);
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

void cog_root(value_t *v) {
  while (STACK->size > 1) {
    stack_pop(STACK);
  }
}

void cog_exit(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *codec = stack_pop(cur->stack);
  if (!codec) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  if (value_stack(codec)[0]->size != 1) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(cur->stack, codec);
    return;
  }
  value_t *code = value_stack(codec)[0]->items[0];
  if (code->type != VWORD) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(cur->stack, codec);
    return;
  }
  EXIT_CODE = code->str_word;
  code->str_word = NULL;
  value_free_safe(codec);
  contain_def_stack_push(STACK->items[0]);
  EXITED = true;
}

void add_funcs_metastack(ht_t *flit) {
  add_func(flit, cog_cd, U"cd");
  add_func(flit, cog_ccd, U"ccd");
  add_func(flit, cog_uncd, U"uncd");
  add_func(flit, cog_pop, U"pop");
  add_func(flit, cog_qstack, U"qstack");
  add_func(flit, cog_root, U"root");
  add_func(flit, cog_exit, U"exit");
}
