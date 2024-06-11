#include <builtins/strings.h>
#include <builtinslib.h>
#include <better_string.h>
#include <strnum.h>
#include <pool.h>

extern stack_t *STACK;

void cog_concat(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (stack->size < 2) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *c2 = stack_pop(stack);
  stack_t *c2stack = *value_stack(c2);
  if (c2stack->size == 0) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(stack, c2);
    return;
  }
  value_t *val;
  for (long i = 0; i < c2stack->size; i++) {
    val = c2stack->items[i];
    if (val->type != VWORD) {
      eval_error(U"BAD ARGUMENT TYPE", v);
      stack_push(stack, c2);
      return;
    }
  }
  value_t *c1 = stack_peek(stack);
  stack_t *c1stack = *value_stack(c1);
  if (c1stack->size == 0) {
    stack_push(stack, c2);
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  for (long i = 0; i < c1stack->size; i++) {
    val = c1stack->items[i];
    if (val->type != VWORD) {
      eval_error(U"BAD ARGUMENT TYPE", v);
      stack_push(stack, c2);
      return;
    }
  }
  value_t *w1 = c1stack->items[0];
  for (long i = 1; i < c1stack->size; i++) {
    val = c1stack->items[i];
    string_concat(w1->str_word, val->str_word);
    value_free_safe(val);
  }
  for (long i = 0; i < c2stack->size; i++) {
    val = c2stack->items[i];
    string_concat(w1->str_word, val->str_word);
  }
  c1stack->size = 1;
  value_free_safe(c2);
}

void cog_unconcat(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  value_t *strc = stack_peek(stack);
  if (!strc) {
    eval_error(U"TOO FEW ARGUMENTS", v);
  }
  stack_t **sp = value_stack(strc);
  stack_t *strstack = *sp;
  value_t *strval;
  for (long i = 0; i < strstack->size; i++) {
    strval = strstack->items[i];
    if (strval->type != VWORD) {
      eval_error(U"BAD ARGUMENT TYPE", v);
      return;
    }
  }
  stack_t *newstack = pool_req(DEFAULT_STACK_SIZE, POOL_STACK);
  string_t *newstring;
  value_t *newval;
  for (long i = 0; i < strstack->size; i++) {
    strval = strstack->items[i];
    for (long i = 0; i < strval->str_word->length; i++) {
      newval = pool_req(DEFAULT_STRING_LENGTH, POOL_VWORD);
      string_append(newval->str_word, strval->str_word->value[i]);
      stack_push(newstack, newval);
    }
  }
  macro_def_stack_push(strstack);
  *sp = newstack;
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
  value_t *r1 = pool_req(DEFAULT_STRING_LENGTH, POOL_VWORD);
  int_to_string_buf(w1->str_word->length, r1->str_word);
  push_quoted(cur, r1);
}

void cog_cat(value_t *v) {
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

void cog_reverse(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *strc = stack_peek(cur->stack);
  if (!strc) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  stack_t *strstack = *value_stack(strc);
  if (strstack->size != 1) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  value_t *strval = strstack->items[0];
  if (strval->type != VWORD) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  string_reverse(strval->str_word);
}

void cog_isword(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *strval = stack_peek(cur->stack);
  if (!strval) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *ret = pool_req(DEFAULT_STRING_LENGTH, POOL_VWORD);
  stack_t *strstack = *value_stack(strval);
  if (strstack->size == 1) {
    value_t *val = strstack->items[0];
    if (val->type == VWORD)
      string_append(ret->str_word, U't');
  }
  push_quoted(cur, ret);
}

void cog_btoi(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *v1 = stack_peek(cur->stack);
  if (!v1) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  stack_t *v1stack = *value_stack(v1);
  if (v1stack->size != 1) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  value_t *str = v1stack->items[0];
  if (str->type != VWORD) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  string_t *bytestr = str->str_word;
  if (bytestr->length != 1) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  int byte = bytestr->value[0];
  int_to_string_buf(byte, str->str_word);
}

void cog_itob(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *v1 = stack_peek(cur->stack);
  if (!v1) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  stack_t *v1stack = *value_stack(v1);
  if (v1stack->size != 1) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  value_t *str = v1stack->items[0];
  if (str->type != VWORD) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  int byte = string_to_int(str->str_word);
  if (str->str_word->length == 0) {
    string_append(str->str_word, byte);
  } else {
    str->str_word->value[0] = byte;
    str->str_word->length = 1;
  }
}

void add_funcs_strings(ht_t *flit) {
  add_func(flit, cog_concat, U"concat");
  add_func(flit, cog_unconcat, U"unconcat");
  add_func(flit, cog_len, U"len");
  add_func(flit, cog_cut, U"cut");
  add_func(flit, cog_cat, U"cat");
  add_func(flit, cog_insert, U"insert");
  add_func(flit, cog_reverse, U"reverse");
  add_func(flit, cog_isword, U"word?");
  add_func(flit, cog_btoi, U"btoi");
  add_func(flit, cog_itob, U"itob");
}
