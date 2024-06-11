#include <builtins/errors.h>
#include <builtinslib.h>
#include <cognition.h>
#include <strnum.h>
#include <macros.h>
#include <pool.h>

extern stack_t *STACK;

void cog_err_clean(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *estack = cur->err_stack;
  if (!estack) return;
  for (int i = 0; i < estack->size; i ++) {
    pool_addobj(POOL_VERR, estack->items[i]);
  }
  estack->size = 0;
}

void cog_err_peek(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *estack = cur->err_stack;
  value_t *v1 = stack_peek(estack);

  if (v1) {
    value_t *r1 = pool_req(v1->error->error->length, POOL_VWORD);
    value_t *r2 = pool_req(v1->error->str_word->length, POOL_VWORD);
    string_copy_buffer(v1->error->error, r1->str_word);
    string_copy_buffer(v1->error->str_word, r2->str_word);
    push_quoted(cur, r1);
    push_quoted(cur, r2);
    return;
  }
  eval_error(U"NO ERRORS", v);
  cog_err_pop(v);
}

void cog_err_pop(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *estack = cur->err_stack;
  value_t *v1 = stack_pop(estack);
  if (v1) {
    value_t *r1 = pool_req(DEFAULT_STRING_LENGTH, POOL_VWORD);
    pool_addobj(POOL_STRING, r1->str_word);
    value_t *r2 = pool_req(DEFAULT_STRING_LENGTH, POOL_VWORD);
    pool_addobj(POOL_STRING, r2->str_word);
    r1->str_word = v1->error->error;
    r2->str_word = v1->error->str_word;
    v1->error->str_word = NULL;
    v1->error->error = NULL;
    push_quoted(cur, r1);
    push_quoted(cur, r2);
    pool_addobj(POOL_VERR, v1);
    return;
  }
  eval_error(U"NO ERRORS", v);
  cog_err_pop(v);
}

void cog_err_push(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (stack->size < 2) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *v2 = stack_pop(stack);
  if (value_stack(v2)[0]->size != 1) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(stack, v2);
    return;
  }
  value_t *w2 = value_stack(v2)[0]->items[0];
  if (w2->type != VWORD) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(stack, v2);
    return;
  }
  value_t *v1 = stack_pop(stack);
  if (value_stack(v1)[0]->size != 1) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    stack_push(stack, v2);
    return;
  }
  value_t *w1 = value_stack(v1)[0]->items[0];
  if (w1->type != VWORD) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    stack_push(stack, v2);
    return;
  }
  value_t *e = pool_req(0, POOL_VERR);

  e->error->str_word = w2->str_word;
  e->error->error = w1->str_word;
  w2->str_word = NULL;
  w1->str_word = NULL;
  pool_addobj(POOL_VWORD, v1);
  pool_addobj(POOL_VWORD, v2);
  if (!cur->err_stack) {
    cur->err_stack = pool_req(DEFAULT_STACK_SIZE, POOL_STACK);
  }
  stack_push(cur->err_stack, e);
}

void cog_err_drop(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *error_stack = cur->err_stack;
  if (error_stack) {
    value_t *e = stack_pop(error_stack);
    if (e) {
      value_free_safe(e);
      return;
    }
  }
  eval_error(U"NO ERRORS", v);
}

void cog_err_print(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *error_stack = cur->err_stack;
  if (error_stack) {
    value_t *e = stack_peek(error_stack);
    if (e) {
      print_value(e, "\n");
      return;
    }
  }
  eval_error(U"NO ERRORS", v);
}

void cog_err_show(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *error_stack = cur->err_stack;
  printf("Error stack:\n");
  if (error_stack) {
    if (error_stack->size) {
      for (long i = 0; i < error_stack->size; i++) {
        value_t *e = error_stack->items[i];
        print_value(e, "\n");
      }
    }
  }
}

void cog_err_throw(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *errc = stack_pop(cur->stack);
  if (!errc) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  stack_t *errstack = *value_stack(errc);
  if (errstack->size != 1) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(cur->stack, errc);
    return;
  }
  value_t *errval = errstack->items[0];
  if (errval->type != VWORD) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(cur->stack, errc);
    return;
  }
  value_t *e = pool_req(0, POOL_VERR);
  if (v)
    e->error->str_word = string_copy(v->str_word);
  else
    e->error->str_word = NULL;
  e->error->error = errval->str_word;
  errval->str_word = NULL;
  value_free_safe(errc);
  if (cur->err_stack == NULL)
    cur->err_stack = pool_req(DEFAULT_STACK_SIZE, POOL_STACK);
  stack_push(cur->err_stack, e);
}

void cog_err_size(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *error_stack = cur->err_stack;
  value_t *sizeval = pool_req(DEFAULT_STRING_LENGTH, POOL_VWORD);
  if (error_stack)
    int_to_string_buf(error_stack->size, sizeval->str_word);
  else
    int_to_string_buf(0, sizeval->str_word);
  push_quoted(cur, sizeval);
}

void add_funcs_errors(ht_t *flit) {
  add_func(flit, cog_err_clean, U"eclean");
  add_func(flit, cog_err_peek, U"epeek");
  add_func(flit, cog_err_pop, U"epop");
  add_func(flit, cog_err_push, U"epush");
  add_func(flit, cog_err_drop, U"edrop");
  add_func(flit, cog_err_print, U"eprint");
  add_func(flit, cog_err_show, U"eshow");
  add_func(flit, cog_err_throw, U"ethrow");
  add_func(flit, cog_err_size, U"esize");
}
