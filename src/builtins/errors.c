#include <builtins/errors.h>
#include <builtinslib.h>
#include <cognition.h>
#include <macros.h>

extern stack_t *STACK;

void cog_err_clean(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *estack = cur->err_stack;
  if (!estack) return;
  for (int i = 0; i < estack->size; i ++) {
    value_free(estack->items[i]);
  }
  estack->size = 0;
}

void cog_err_peek(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *estack = cur->err_stack;
  value_t *v1 = stack_peek(estack);
  value_t *r1 = init_value(VWORD);
  value_t *r2 = init_value(VWORD);
  if (v1) {
    r1->str_word = string_copy(v1->error->error);
    r2->str_word = string_copy(v1->error->str_word);
  } else {
    r1->str_word = init_string(U"NO ERRORS");
    r2->str_word = string_copy(v->str_word);
  }
  push_quoted(cur, r1);
  push_quoted(cur, r2);
}

void cog_err_pop(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *estack = cur->err_stack;
  value_t *v1 = stack_pop(estack);
  value_t *r1 = init_value(VWORD);
  value_t *r2 = init_value(VWORD);
  if (v1) {
    r1->str_word = v1->error->error;
    r2->str_word = v1->error->str_word;
    v1->error->str_word = NULL;
    v1->error->error = NULL;
  } else {
    r1->str_word = init_string(U"NO ERRORS");
    r2->str_word = string_copy(v->str_word);
  }
  push_quoted(cur, r1);
  push_quoted(cur, r2);
  value_free(v1);
}

void cog_err_push(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *estack = cur->err_stack;
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
  value_t *e = init_value(VERR);
  e->error = calloc(1, sizeof(error_t));
  e->error->str_word = w2->str_word;
  e->error->error = w1->str_word;
  w2->str_word = NULL;
  w1->str_word = NULL;
  value_free_safe(v1);
  value_free_safe(v2);
  if (!estack) {
    cur->err_stack = init_stack(DEFAULT_STACK_SIZE);
  }
  stack_push(estack, e);
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
  printf("'");
  print(v->str_word);
  printf("':%sNO ERRORS%s\n", RED, COLOR_RESET);
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
  value_t *e = init_value(VERR);
  e->error = calloc(1, sizeof(error_t));
  if (v)
    e->error->str_word = string_copy(v->str_word);
  else
    e->error->str_word = NULL;
  e->error->error = errval->str_word;
  errval->str_word = NULL;
  value_free_safe(errc);
  if (cur->err_stack == NULL)
    cur->err_stack = init_stack(DEFAULT_STACK_SIZE);
  stack_push(cur->err_stack, e);
}

void add_funcs_errors(ht_t *flit) {
  add_func(flit, cog_err_clean, U"eclean");
  add_func(flit, cog_err_peek, U"epeek");
  add_func(flit, cog_err_pop, U"epop");
  add_func(flit, cog_err_push, U"epush");
  add_func(flit, cog_err_print, U"eprint");
  add_func(flit, cog_err_show, U"eshow");
  add_func(flit, cog_err_throw, U"ethrow");
}
