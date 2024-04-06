#include <builtins/metastack.h>
#include <builtinslib.h>
#include <macros.h>

extern stack_t *STACK;
extern stack_t *CONTAIN_DEF_STACK;
extern string_t *ROOT;
extern stack_t *OBJ_TABLE_STACK;
extern stack_t *OBJ_TABLE_REF_STACK;
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
  value_t *child = stack_pop(cur->stack);
  if (child == NULL) {
    stack_push(STACK, cur);
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  if (child->type != VSTACK) {
    stack_push(cur->stack, cur);
    stack_push(STACK, cur);
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  stack_push(CONTAIN_DEF_STACK, cur);
  contain_t *newcur = stack_peek(STACK);
  if (newcur) {
    for (long i = newcur->stack->size - 1; i >= 0; i--) {
      value_t *val = newcur->stack->items[i];
      if (val->type == VSTACK) {
        if (val->container == cur) {
          val->container = child->container;
          break;
        }
      }
    }
  }
  if (CURRENT_ROOT == cur)
    CURRENT_ROOT = child->container;
  cur = child->container;
  child->container = NULL;
  value_free_safe(child);
  stack_push(STACK, cur);
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
  if (old == CURRENT_ROOT) {
    stack_push(STACK, old);
    cog_qstack(v);
  }
}

void cog_uncdf(value_t *v) {
  contain_t *old = stack_pop(STACK);
  if (STACK->size == 0) {
    stack_push(STACK, old);
    cog_qstack(v);
  } else if (CURRENT_ROOT == old) {
    ROOT->length--;
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
  if (old == CURRENT_ROOT) {
    cog_qstack(v);
  }
  contain_t *newc = stack_peek(STACK);
  stack_push(newc->stack, popval);
}

void cog_popf(value_t *v) {
  contain_t *old = stack_pop(STACK);
  value_t *popval = stack_pop(old->stack);
  if (!popval) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    stack_push(old->stack, popval);
    stack_push(STACK, old);
  }
  if (STACK->size == 0) {
    cog_qstack(v);
  } else if (CURRENT_ROOT == old) {
    ROOT->length--;
  }
  contain_t *newc = stack_peek(STACK);
  stack_push(newc->stack, popval);
}

void cog_qstack(value_t *v) {
  contain_t *oldc = stack_pop(STACK);
  contain_t *cur = stack_peek(STACK);
  if (!cur) {
    value_t *oldval = init_value(VSTACK);
    oldval->container = oldc;
    contain_t *new = calloc(1, sizeof(contain_t));
    contain_copy_attributes(oldc, new);
    new->stack = init_stack(DEFAULT_STACK_SIZE);
    stack_push(new->stack, oldval);
    stack_push(STACK, new);
    CURRENT_ROOT = new;
    return;
  }
  value_t *oldval = stack_pop(cur->stack);
  value_t *new = init_value(VSTACK);
  new->container = calloc(1, sizeof(contain_t));
  contain_copy_attributes(oldval->container, new->container);
  new->container->stack = init_stack(DEFAULT_STACK_SIZE);
  stack_push(new->container->stack, oldval);
  stack_push(cur->stack, new);
  if (CURRENT_ROOT == oldc)
    CURRENT_ROOT = new->container;
  stack_push(STACK, new->container);
}

void cog_root(value_t *v) {
  while (stack_peek(STACK) != CURRENT_ROOT) {
    stack_pop(STACK);
  }
}

void cog_su(value_t *v) {
  while (ROOT->length > 1) {
    ROOT->length--;
  }
  cog_root(v);
}

void cog_chroot(value_t *v) {
  contain_t *old = stack_peek(STACK);
  cog_cd(v);
  contain_t *cur = stack_peek(STACK);
  if (cur == old) return;
  for (char32_t i = 0; i < OBJ_TABLE_REF_STACK->size; i++) {
    if (OBJ_TABLE_REF_STACK->items[i] == cur) {
      string_append(ROOT, i);
      return;
    }
  }
  stack_push(OBJ_TABLE_REF_STACK, cur);
  stack_push(OBJ_TABLE_STACK, init_ht(DEFAULT_HT_SIZE));
  char32_t index = OBJ_TABLE_REF_STACK->size - 1;
  string_append(ROOT, index);
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
  add_func(flit, cog_uncdf, U"uncdf");
  add_func(flit, cog_pop, U"pop");
  add_func(flit, cog_popf, U"popf");
  add_func(flit, cog_qstack, U"qstack");
  add_func(flit, cog_root, U"root");
  add_func(flit, cog_su, U"su");
  add_func(flit, cog_chroot, U"chroot");
  add_func(flit, cog_exit, U"exit");
}
