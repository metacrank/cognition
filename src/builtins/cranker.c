#include <cognition.h>
#include <builtins/cranker.h>
#include <builtinslib.h>
#include <macros.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <strnum.h>

extern stack_t *STACK;
extern stack_t *EVAL_STACK;

/* n m metacrank sets metacrank n to period m */
void cog_metacrank(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  value_t *tmp = stack_pop(stack);
  if (!tmp) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *tmp2 = stack_pop(stack);
  if (!tmp2) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    stack_push(stack, tmp);
    return;
  }
  if (value_stack(tmp)[0]->size != 1) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(stack, tmp2);
    stack_push(stack, tmp);
    return;
  }
  value_t *v2 = value_stack(tmp)[0]->items[0];
  if (value_stack(tmp2)[0]->size != 1) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(stack, tmp2);
    stack_push(stack, tmp);
    return;
  }
  value_t *v1 = value_stack(tmp2)[0]->items[0];
  if (v1 == NULL || v2 == NULL) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(stack, tmp2);
    stack_push(stack, tmp);
    return;
  }
  if (v1->type != VWORD || v2->type != VWORD) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(stack, tmp2);
    stack_push(stack, tmp);
    return;
  }
  int v1val = string_to_int(v1->str_word);
  int v2val = string_to_int(v2->str_word);
  if (v1val < 0 || v2val < 0) {
    eval_error(U"INDEX OUT OF RANGE", v);
    stack_push(stack, tmp2);
    stack_push(stack, tmp);
    return;
  }
  if (cur->cranks == NULL) cur->cranks = init_stack(DEFAULT_STACK_SIZE);
  stack_t *cranks = cur->cranks;
  while (cranks->size <= v1val) {
    int(*arr)[2] = malloc(sizeof(int[2]));
    arr[0][0] = 0;
    arr[0][1] = 0;
    stack_push(cranks, arr);
  }
  int(*cr)[2] = cranks->items[v1val];
  cr[0][0] = 0;
  cr[0][1] = v2val;
  value_free_safe(tmp);
  value_free_safe(tmp2);
}

/*sets 0th crank value to specified period*/
void cog_crank(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  value_t *tmp = stack_pop(stack);
  if (!tmp) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  if (value_stack(tmp)[0]->size != 1) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(stack, tmp);
    return;
  }
  value_t *v1 = value_stack(tmp)[0]->items[0];
  if (v1->type != VWORD) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(stack, tmp);
    return;
  }
  int v1val = string_to_int(v1->str_word);
  if (v1val < 0) {
    eval_error(U"INDEX OUT OF RANGE", v);
    stack_push(stack, tmp);
    return;
  }
  if (cur->cranks == NULL) cur->cranks = init_stack(DEFAULT_STACK_SIZE);
  stack_t *cranks = cur->cranks;
  if (cranks->size == 0) {
    int(*arr)[2] = malloc(sizeof(int[2]));
    arr[0][0] = 0;
    arr[0][1] = 0;
    stack_push(cranks, arr);
  }
  int(*c)[2] = cranks->items[0];
  c[0][0] = 0;
  c[0][1] = v1val;
  value_free_safe(tmp);
}

/* sets all crank values to specified period */
void cog_crankall(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  value_t *tmp = stack_pop(stack);
  if (!tmp) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  if (value_stack(tmp)[0]->size != 1) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(stack, tmp);
    return;
  }
  value_t *v1 = value_stack(tmp)[0]->items[0];
  if (v1->type != VWORD) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(stack, tmp);
    return;
  }
  int v1val = string_to_int(v1->str_word);
  if (v1val < 0) {
    eval_error(U"INDEX OUT OF RANGE", v);
    stack_push(stack, tmp);
    return;
  }
  if (cur->cranks == NULL)
    cur->cranks = init_stack(DEFAULT_STACK_SIZE);
  stack_t *cranks = cur->cranks;
  if (cranks->size <= stack->size) {
    for (int i = 0; i < cranks->size; i++) {
      int(*arr)[2] = cranks->items[i];
      arr[0][0] = 0;
      arr[0][1] = v1val;
    }
    for (int i = cranks->size; i < stack->size; i++) {
      int(*arr)[2] = malloc(sizeof(int[2]));
      arr[0][0] = 0;
      arr[0][1] = v1val;
      stack_push(cranks, arr);
    }
  } else {
    for (int i = 0; i < stack->size; i++) {
      int(*arr)[2] = cranks->items[i];
      arr[0][0] = 0;
      arr[0][1] = v1val;
    }
    for (int i = stack->size; i < cranks->size; i++) {
      free(stack_pop(cranks));
    }
  }
  value_free_safe(tmp);
}

void cog_halt(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_free(cur->cranks, free);
  cur->cranks = NULL;
}

void cog_crankbase(value_t *v) {
  value_t *v1 = init_value(VWORD);
  contain_t *cur = stack_peek(STACK);
  int base = 0;
  if (cur->cranks) {
    if (cur->cranks->size) {
      int(*cr)[2] = cur->cranks->items[0];
      base = cr[0][1];
    }
  }
  v1->str_word = int_to_string(base);
  push_quoted(cur, v1);
}

void cog_modcrank(value_t *v) {
  value_t *v1 = init_value(VWORD);
  contain_t *cur = stack_peek(STACK);
  int mod = 0;
  if (cur->cranks) {
    if (cur->cranks->size) {
      int(*cr)[2] = cur->cranks->items[0];
      mod = cr[0][0];
    }
  }
  v1->str_word = int_to_string(mod);
  push_quoted(cur, v1);
}

void cog_metacrankbase(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *idxq = stack_peek(cur->stack);
  if (idxq == NULL) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  if (value_stack(idxq)[0]->size != 1) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  value_t *idxval = value_stack(idxq)[0]->items[0];
  if (idxval->type != VWORD) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  size_t idx = string_to_int(idxval->str_word);
  if (idx < 0) {
    eval_error(U"INDEX OUT OF RANGE", v);
    return;
  }
  size_t cranksize = 1;
  if (cur->cranks)
    if (cur->cranks->size)
      cranksize = cur->cranks->size;
  int base = 0;
  if (idx < cranksize && cur->cranks) {
    if (cur->cranks->size) {
      int(*cr)[2] = cur->cranks->items[idx];
      base = cr[0][1];
    }
  }
  string_free(idxval->str_word);
  idxval->str_word = int_to_string(base);
}

void cog_metamodcrank(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *idxq = stack_peek(cur->stack);
  if (idxq == NULL) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  if (value_stack(idxq)[0]->size != 1) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  value_t *idxval = value_stack(idxq)[0]->items[0];
  if (idxval->type != VWORD) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  size_t idx = string_to_int(idxval->str_word);
  if (idx < 0) {
    eval_error(U"INDEX OUT OF RANGE", v);
    return;
  }
  size_t cranksize = 1;
  if (cur->cranks)
    if (cur->cranks->size)
      cranksize = cur->cranks->size;
  int mod = 0;
  if (idx < cranksize && cur->cranks) {
    if (cur->cranks->size) {
      int(*cr)[2] = cur->cranks->items[idx];
      mod = cr[0][0];
    }
  }
  string_free(idxval->str_word);
  idxval->str_word = int_to_string(mod);
}

void add_funcs_cranker(ht_t *flit) {
  add_func(flit, cog_metacrank, U"metacrank");
  add_func(flit, cog_crank, U"crank");
  //add_func(flit, cog_crankall, U"crankall");
  add_func(flit, cog_halt, U"halt");
  add_func(flit, cog_crankbase, U"crankbase");
  add_func(flit, cog_modcrank, U"modcrank");
  add_func(flit, cog_metacrankbase, U"metacrankbase");
  add_func(flit, cog_metamodcrank, U"metamodcrank");
}
