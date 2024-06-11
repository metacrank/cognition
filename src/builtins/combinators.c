#include <builtins/combinators.h>
#include <builtinslib.h>
#include <macros.h>
#include <pool.h>
#include <string.h>
#include <stdio.h>
#include <strnum.h>

extern stack_t *STACK;
extern stack_t *EVAL_STACK;
string_t **CAST_ARGS;
string_t *F;
string_t *ING;

extern void add_funcs(ht_t *flit);

void cog_quote(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *v1 = stack_pop(cur->stack);
  if (v1 == NULL) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  push_quoted(cur, v1);
}

void cog_eval(value_t *v) {
  contain_t *old = stack_peek(STACK);
  evalf(v);
  old = stack_peek(STACK);
  if (!old) return;
  dec_crank(old);
}

void cog_child(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *v1 = pool_req(DEFAULT_STACK_SIZE, POOL_VSTACK);
  contain_copy_attributes(cur, v1->container);
  if (v1->container->err_stack)
    for (long i = 0; i < cur->err_stack->size; i++)
      stack_push(v1->container->err_stack, value_copy(cur->err_stack->items[i]));
  else
    v1->container->err_stack = stack_copy(cur->err_stack, value_copy);
  stack_push(cur->stack, v1);
}

void cog_stack(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *v1 = pool_req(DEFAULT_STACK_SIZE, POOL_VSTACK);
  v1->container->delims = string_copy(cur->delims);
  v1->container->singlets = string_copy(cur->singlets);
  v1->container->ignored = string_copy(cur->ignored);
  v1->container->dflag = cur->dflag;
  v1->container->sflag = cur->sflag;
  v1->container->iflag = cur->iflag;
  stack_push(cur->stack, v1);
}


void cog_macro(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *v1 = pool_req(DEFAULT_STACK_SIZE, POOL_VMACRO);
  stack_push(cur->stack, v1);
}

void cog_cast(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *v1 = stack_pop(cur->stack);
  if (!v1) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *quot = stack_peek(cur->stack);
  stack_t *v1stack = *value_stack(v1);
  if (v1stack->size != 1) {
    stack_push(cur->stack, v1);
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  value_t *num = v1stack->items[0];
  if (num->type != VWORD) {
    stack_push(cur->stack, v1);
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  if (string_comp(num->str_word, CAST_ARGS[0]) == 0 || string_comp(num->str_word, CAST_ARGS[1]) == 0) {
    value_free_safe(v1);
    if (quot->type == VSTACK)
      return;
    quot->type = VSTACK;
    contain_t *c = calloc(1, sizeof(contain_t));
    c->iflag = true;
    c->sflag = true;
    c->stack = quot->macro;
    quot->container = c;
    return;
  }
  if (string_comp(num->str_word, CAST_ARGS[2]) == 0 || string_comp(num->str_word, CAST_ARGS[3]) == 0) {
    value_free_safe(v1);
    if (quot->type == VMACRO)
      return;
    quot->type = VMACRO;
    stack_t *s = quot->container->stack;
    quot->container->stack = NULL;
    contain_def_stack_push(quot->container);
    quot->macro = s;
    return;
  }
  eval_error(U"INDEX OUT OF RANGE", v);
  stack_push(cur->stack, v1);
}

void cog_sub(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *v1 = pool_req(DEFAULT_STACK_SIZE, POOL_VSTACK);
  v1->container->flit = pool_req(0, POOL_HT);
  if (!v1->container->faliases)
    v1->container->faliases = pool_req(DEFAULT_STACK_SIZE, POOL_STACK);
  stack_push(v1->container->faliases, string_copy(F));
  stack_push(v1->container->faliases, string_copy(ING));
  add_funcs(v1->container->flit);
  stack_push(cur->stack, v1);
}

void cog_compose(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  if (cur->stack->size < 2) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *v2 = stack_pop(cur->stack);
  stack_t **v2stack = value_stack(v2);
  if ((*v2stack)->size == 0) {
    value_free_safe(v2);
    return;
  }
  value_t *v1 = stack_peek(cur->stack);
  stack_t **v1stack = value_stack(v1);
  stack_extend(*v1stack, *v2stack);
  (*v2stack)->size = 0;
  value_free_safe(v2);
}

void cog_prepose(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  if (cur->stack->size < 2) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *v2 = stack_pop(cur->stack);
  stack_t **v2stackp = value_stack(v2);
  stack_t *v2stack = *v2stackp;
  if (v2stack->size == 0) {
    value_free_safe(v2);
    return;
  }
  value_t *v1 = stack_peek(cur->stack);
  stack_t **v1stackp = value_stack(v1);
  stack_t *v1stack = *v1stackp;
  stack_t *stack = v2stack;
  stack_extend(stack, v1stack);
  v1stack->size = 0;
  *v2stackp = *v1stackp;
  *v1stackp = stack;
  value_free_safe(v2);
}

void cog_put(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  if (cur->stack->size < 3) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *index = stack_pop(cur->stack);
  if (value_stack(index)[0]->size != 1) {
    stack_push(cur->stack, index);
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  value_t *idxval = value_stack(index)[0]->items[0];
  if (idxval->type != VWORD) {
    stack_push(cur->stack, index);
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  value_t *v1 = stack_pop(cur->stack);
  value_t *stack = stack_peek(cur->stack);
  long idx = string_to_int(idxval->str_word);
  stack_t *stackstack = *value_stack(stack);
  if (idx < 0 || idx > stackstack->size) {
    stack_push(cur->stack, v1);
    stack_push(cur->stack, index);
    eval_error(U"INDEX OUT OF RANGE", v);
    return;
  }
  value_free_safe(index);
  stack_t *v1stack = *value_stack(v1);
  long v1size = v1stack->size;
  for (long i = 0; i < v1size; i++)
    stack_push(stackstack, NULL);
  for (long i = stackstack->size - 1; i >= idx + v1size; i--) {
    stackstack->items[i] = stackstack->items[i-v1size];
  }
  for (long i = 0; i < v1size; i++)
    stackstack->items[idx + i] = v1stack->items[i];
  v1stack->size = 0;
  value_free_safe(v1);
}

void cog_dip(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (cur->stack->size < 2) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *quot = stack_pop(stack);
  value_t *v1 = stack_pop(stack);
  stack_push(stack, quot);
  evalf(v);
  if (!cur) return; // this will never work
  stack_push(stack, v1);
  dec_crank(cur);
}

void cog_if(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  value_t *v3 = stack_pop(stack);
  if (!v3) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *v2 = stack_pop(stack);
  if (!v2) {
    stack_push(cur->stack, v3);
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *v1 = stack_pop(stack);
  if (!v1) {
    stack_push(cur->stack, v2);
    stack_push(cur->stack, v3);
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *w1 = value_stack(v1)[0]->items[0];
  if (w1->type != VWORD) {
    stack_push(stack, v1);
    stack_push(stack, v2);
    stack_push(stack, v3);
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  bool v1_fixed = word_truth(w1);
  value_free_safe(v1);
  if (v1_fixed) {
    stack_push(stack, v2);
    value_free_safe(v3);
    evalf(v);
  } else {
    stack_push(stack, v3);
    value_free_safe(v2);
    evalf(v);
  }
  if (!cur) return;
  dec_crank(cur);
}

void cog_split(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  if (cur->stack->size < 2) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *num = stack_pop(cur->stack);
  if (value_stack(num)[0]->size != 1) {
    stack_push(cur->stack, num);
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  value_t *numval = value_stack(num)[0]->items[0];
  if (numval->type != VWORD) {
    stack_push(cur->stack, num);
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  long n = string_to_int(numval->str_word);
  value_t *q = stack_peek(cur->stack);
  stack_t *qstack = *value_stack(q);
  if (n < 0 || n > qstack->size) {
    stack_push(cur->stack, num);
    eval_error(U"INDEX OUT OF RANGE", v);
    return;
  }
  value_t *q2 = pool_req(qstack->size - n, val2pool_type(q));
  stack_t *q2stack;
  if (q->type == VSTACK) {
    if (q2->container->err_stack)
      for (long i = 0; i < q->container->err_stack->size; i++)
        stack_push(q2->container->err_stack, value_copy(q->container->err_stack->items[i]));
    else
      q2->container->err_stack = stack_copy(q->container->err_stack, value_copy);
    contain_copy_attributes(q->container, q2->container);
    q2stack = q2->container->stack;
  } else {
    q2stack = q2->macro;
  }
  for (long i = n; i < qstack->size; i++) {
    stack_push(q2stack, qstack->items[i]);
  }
  qstack->size = n;
  stack_push(cur->stack, q2);
  value_free_safe(num);
}

void cog_vat(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (stack->size < 2) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *index = stack_pop(stack);
  if (value_stack(index)[0]->size != 1) {
    stack_push(stack, index);
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  value_t *idxval = value_stack(index)[0]->items[0];
  if (idxval->type != VWORD) {
    stack_push(stack, index);
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  long n = string_to_int(idxval->str_word);
  value_t *quot = stack_peek(stack);
  if (n < 0 || n >= value_stack(quot)[0]->size) {
    stack_push(stack, index);
    eval_error(U"INDEX OUT OF RANGE", v);
    return;
  }
  value_free_safe(index);
  value_t *v1 = value_copy(value_stack(quot)[0]->items[n]);
  push_quoted(cur, v1);
}

void cog_substack(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (stack->size < 3) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *index2 = stack_pop(stack);
  value_t *index1 = stack_pop(stack);
  if (value_stack(index1)[0]->size != 1 || value_stack(index2)[0]->size != 1) {
    stack_push(stack, index1);
    stack_push(stack, index2);
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  value_t *idxval1 = value_stack(index1)[0]->items[0];
  value_t *idxval2 = value_stack(index2)[0]->items[0];
  if (idxval1->type != VWORD || idxval2->type != VWORD) {
    stack_push(stack, index1);
    stack_push(stack, index2);
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  long n1 = string_to_int(idxval1->str_word);
  long n2 = string_to_int(idxval2->str_word);
  value_t *quot = stack_peek(stack);
  stack_t *qstack = *value_stack(quot);
  if (n1 < 0 || n2 < 0 || n1 > qstack->size || n2 > qstack->size) {
    stack_push(stack, index1);
    stack_push(stack, index2);
    eval_error(U"INDEX OUT OF RANGE", v);
    return;
  }
  value_free_safe(index1);
  value_free_safe(index2);
  for (long i = 0; i < n1; i++) {
    value_free_safe(qstack->items[i]);
    qstack->items[i] = NULL;
  }
  for (long i = n2; i < qstack->size; i++) {
    value_free_safe(qstack->items[i]);
    qstack->items[i] = NULL;
  }
  for (long i = 0; i < n2 - n1; i++) {
    qstack->items[i] = qstack->items[i + n1];
  }
  qstack->size = n2 - n1;
  if (qstack->size < 0) {
    qstack->size = 0;
  }
}

void cog_del(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (stack->size < 2) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *index = stack_pop(stack);
  if (value_stack(index)[0]->size != 1) {
    stack_push(stack, index);
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  value_t *idxval = value_stack(index)[0]->items[0];
  if (idxval->type != VWORD) {
    stack_push(stack, index);
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  long n = string_to_int(idxval->str_word);
  value_t *quot = stack_peek(stack);
  if (n < 0 || n >= value_stack(quot)[0]->size) {
    stack_push(stack, index);
    eval_error(U"INDEX OUT OF RANGE", v);
    return;
  }
  value_free_safe(index);
  value_free_safe(stack_popdeep(value_stack(quot)[0], n));
}

void cog_uncompose(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  value_t *quot = stack_pop(stack);
  if (quot == NULL) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  stack_t *qstack = *value_stack(quot);
  for (int i = 0; i < qstack->size; i++) {
    /* value_t *element = init_value(VSTACK); */
    /* if (quot->type == VSTACK) { */
    /*   element->container = calloc(1, sizeof(contain_t)); */
    /*   element->container->stack = init_stack(DEFAULT_STACK_SIZE); */
    /*   element->container->err_stack = stack_copy(quot->container->err_stack, value_copy); */
    /*   contain_copy_attributes(quot->container, element->container); */
    /*   stack_push(element->container->stack, qstack->items[i]); */
    /*   stack_push(stack, element); */
    /* } else { */
    /*   push_quoted(cur, quot->macro->items[i]); */
    /* } */
    push_quoted(cur, qstack->items[i]);
  }
  qstack->size = 0;
  value_free_safe(quot);
}

void cog_size(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  value_t *v1 = stack_peek(stack);
  if (v1 == NULL) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  size_t size = value_stack(v1)[0]->size;
  value_t *sizeval = pool_req(DEFAULT_STRING_LENGTH, POOL_VWORD);
  int_to_string_buf(size, sizeval->str_word);
  push_quoted(cur, sizeval);
}

void cog_type(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  value_t *v1 = stack_peek(stack);
  if (v1 == NULL) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *typeval = pool_req(DEFAULT_STRING_LENGTH, POOL_VWORD);
  if (v1->type == VSTACK)
    string_copy_buffer(CAST_ARGS[0], typeval->str_word);
  else if (v1->type == VMACRO)
    string_copy_buffer(CAST_ARGS[2], typeval->str_word);
  else die("BAD VALUE ON STACK");
  push_quoted(cur, typeval);
}

void add_funcs_combinators(ht_t *flit) {
  add_func(flit, cog_quote, U"quote");
  add_func(flit, cog_eval, U"eval");
  add_func(flit, cog_child, U"child");
  add_func(flit, cog_stack, U"stack");
  add_func(flit, cog_macro, U"macro");
  add_func(flit, cog_cast, U"cast");
  add_func(flit, cog_sub, U"sub");
  add_func(flit, cog_compose, U"compose");
  add_func(flit, cog_prepose, U"prepose");
  add_func(flit, cog_put, U"put");
  add_func(flit, cog_dip, U"dip");
  add_func(flit, cog_if, U"if");
  add_func(flit, cog_split, U"split");
  add_func(flit, cog_vat, U"vat");
  add_func(flit, cog_substack, U"substack");
  add_func(flit, cog_del, U"del");
  add_func(flit, cog_uncompose, U"uncompose");
  add_func(flit, cog_size, U"size");
  add_func(flit, cog_type, U"type");
}
