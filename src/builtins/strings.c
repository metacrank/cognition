#include <builtins/strings.h>
#include <builtinslib.h>
#include <better_string.h>
#include <strnum.h>

extern stack_t *STACK;

void cog_concat(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (stack->size < 2) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *c2 = stack_pop(stack);
  if (value_stack(c2)[0]->size != 1) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(stack, c2);
    return;
  }
  value_t *w2 = value_stack(c2)[0]->items[0];
  if (w2->type != VWORD) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(stack, c2);
    return;
  }
  value_t *c1 = stack_peek(stack);
  if (value_stack(c1)[0]->size != 1) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(stack, c2);
    return;
  }
  value_t *w1 = value_stack(c1)[0]->items[0];
  if (w1->type != VWORD) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(stack, c2);
    return;
  }
  string_concat(w1->str_word, w2->str_word);
  value_free_safe(c2);
}

void cog_cut(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (stack->size < 2) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *v1 = stack_pop(stack);
  if (value_stack(v1)[0]->size != 1) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    return;
  }
  value_t *w1 = value_stack(v1)[0]->items[0];
  if (w1->type != VWORD) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    return;
  }
  value_t *quot = stack_pop(stack);
  if (value_stack(quot)[0]->size != 1) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(stack, quot);
    stack_push(stack, v1);
    return;
  }
  value_t *wq = value_stack(quot)[0]->items[0];
  if (wq->type != VWORD) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(stack, quot);
    stack_push(stack, v1);
    return;
  }
  string_t *str = wq->str_word;
  long n = string_to_int(w1->str_word);
  if (n < 0 || n > str->length) {
    eval_error(U"INDEX OUT OF RANGE", v);
    stack_push(stack, quot);
    stack_push(stack, v1);
    return;
  }
  w1->str_word->length = 0;
  for (int i = n; i < wq->str_word->length; i++) {
    string_append(w1->str_word, wq->str_word->value[i]);
  }
  wq->str_word->length = n;
  stack_push(stack, quot);
  stack_push(stack, v1);
}

void cog_len(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (stack->size < 1) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *v1 = stack_peek(stack);
  if (value_stack(v1)[0]->size != 1) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  value_t *w1 = value_stack(v1)[0]->items[0];
  if (w1->type != VWORD) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  value_t *r1 = init_value(VWORD);
  r1->str_word = int_to_string(w1->str_word->length);
  push_quoted(cur, r1);
}

void cog_nth(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (stack->size < 2) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *v1 = stack_pop(stack);
  if (value_stack(v1)[0]->size != 1) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    return;
  }
  value_t *w1 = value_stack(v1)[0]->items[0];
  if (w1->type != VWORD) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    return;
  }
  value_t *quot = stack_peek(stack);
  if (value_stack(quot)[0]->size != 1) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    return;
  }
  value_t *wq = value_stack(quot)[0]->items[0];
  if (wq->type != VWORD) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    return;
  }
  string_t *str = wq->str_word;
  long n = string_to_int(w1->str_word);
  if (n < 0 || n >= str->length) {
    eval_error(U"INDEX OUT OF RANGE", v);
    stack_push(stack, v1);
    return;
  }
  value_free_safe(v1);
  str->value[0] = str->value[n];
  str->value[1] = '\0';
  str->length = 1;
}

void cog_insert(value_t *v) {
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
  stack_t *v1stack = *value_stack(v1);
  if (v1stack->size != 1) {
    stack_push(cur->stack, v1);
    stack_push(cur->stack, index);
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  value_t *substr = v1stack->items[0];
  if (substr->type != VWORD) {
    stack_push(cur->stack, v1);
    stack_push(cur->stack, index);
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  value_t *string = stack_peek(cur->stack);
  stack_t *stringstack = *value_stack(string);
  if (stringstack->size != 1) {
    stack_push(cur->stack, v1);
    stack_push(cur->stack, index);
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  value_t *strval = stringstack->items[0];
  if (strval->type != VWORD) {
    stack_push(cur->stack, v1);
    stack_push(cur->stack, index);
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  string_t *str = strval->str_word;
  long idx = string_to_int(idxval->str_word);
  if (idx < 0 || idx > str->length) {
    stack_push(cur->stack, v1);
    stack_push(cur->stack, index);
    eval_error(U"INDEX OUT OF RANGE", v);
    return;
  }
  value_free_safe(index);
  value_free_safe(v1);
  string_t *insert_str = substr->str_word;
  size_t insert_length = insert_str->length;
  size_t str_length = str->length;
  for (size_t i = 0; i < insert_length; i++) {
    string_append(str, 0);
  }
  for (long i = str_length - 1; i >= idx; i--) {
    str->value[i + insert_length] = str->value[i];
  }
  for (size_t i = 0; i < insert_length; i++) {
    str->value[i + idx] = insert_str->value[i];
  }
}

void cog_isword(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *strval = stack_peek(cur->stack);
  if (!strval) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  stack_t *strstack = *value_stack(strval);
  if (strstack->size != 1) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  value_t *val = strstack->items[0];
  value_t *ret = init_value(VWORD);
  if (val->type == VWORD) {
    ret->str_word = init_string(U"t");
  } else {
    ret->str_word = init_string(U"");
  }
  push_quoted(cur, ret);
}

void add_funcs_strings(ht_t *flit) {
  add_func(flit, cog_concat, U"concat");
  add_func(flit, cog_len, U"len");
  add_func(flit, cog_cut, U"cut");
  add_func(flit, cog_nth, U"nth");
  add_func(flit, cog_insert, U"insert");
  add_func(flit, cog_isword, U"isword");
}
