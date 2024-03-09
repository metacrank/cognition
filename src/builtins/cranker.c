#include <cognition.h>
#include <builtins/cranker.h>
#include <builtinslib.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>

extern stack_t *STACK;
extern stack_t *EVAL_STACK;

/* n m metacrank sets metacrank n to period m */
void cog_metacrank(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  value_t *tmp = stack_pop(stack);
  if (!tmp) {
    eval_error("EMPTY STACK", v);
    return;
  }
  contain_t *ctmp = tmp->container;
  if (ctmp->stack->size != 1) {
    eval_error("TYPE ERROR", v);
    stack_push(stack, tmp);
    return;
  }
  value_t *v2 = ctmp->stack->items[0];

  value_t *tmp2 = stack_pop(stack);
  if (!tmp2) {
    eval_error("EMPTY STACK", v);
    stack_push(stack, tmp);
    return;
  }
  contain_t *ctmp2 = tmp2->container;
  if (ctmp2->stack->size != 1) {
    eval_error("TYPE ERROR", v);
    stack_push(stack, tmp2);
    stack_push(stack, tmp);
    return;
  }
  value_t *v1 = ctmp2->stack->items[0];

  if (v1 == NULL || v2 == NULL) {
    eval_error("TYPE ERROR", v);
    stack_push(stack, tmp2);
    stack_push(stack, tmp);
    return;
  }
  if (v1->type != VWORD || v2->type != VWORD) {
    eval_error("TYPE ERROR", v);
    stack_push(stack, tmp2);
    stack_push(stack, tmp);
    return;
  }
  if (!(strisint(v1->str_word) && strisint(v1->str_word))) {
    eval_error("TYPE ERROR", v);
    stack_push(stack, tmp2);
    stack_push(stack, tmp);
    return;
  }
  stack_t *cranks = cur->cranks;
  int v1val = atoi(v1->str_word->value);
  int v2val = atoi(v2->str_word->value);
  if (v1val >= cranks->size) {
    for (int _ = 0; _ < v1val - cranks->size + 1; _++) {
      int(*arr)[2] = malloc(sizeof(int[2]));
      arr[0][0] = 0;
      arr[0][1] = 0;
      stack_push(cranks, arr);
    }
  }
  int(*cr)[2] = cranks->items[v1val];
  cr[0][0] = 0;
  cr[0][1] = v2val;
  value_free(tmp);
  value_free(tmp2);
}

/*sets 0th crank value to specified period*/
void cog_crank(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  value_t *tmp = stack_pop(stack);
  if (!tmp) {
    eval_error("EMPTY STACK", v);
    return;
  }
  contain_t *ctmp = tmp->container;
  if (ctmp->stack->size != 1) {
    eval_error("TYPE ERROR", v);
    stack_push(stack, tmp);
    return;
  }
  value_t *v1 = ctmp->stack->items[0];
  if (v1->type != VWORD) {
    eval_error("TYPE ERROR", v);
    stack_push(stack, tmp);
    return;
  }
  if (!strisint(v1->str_word)) {
    eval_error("TYPE ERROR", v);
    stack_push(stack, tmp);
    return;
  }
  int v1val = atoi(v1->str_word->value);
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
  value_free(tmp);
}

/* sets all crank values to specified period */
void cog_crankall(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  value_t *tmp = stack_pop(stack);
  if (!tmp) {
    eval_error("EMPTY STACK", v);
    return;
  }
  contain_t *ctmp = tmp->container;
  if (ctmp->stack->size != 1) {
    eval_error("TYPE ERROR", v);
    stack_push(stack, tmp);
    return;
  }
  value_t *v1 = ctmp->stack->items[0];
  if (v1->type != VWORD) {
    eval_error("TYPE ERROR", v);
    stack_push(stack, tmp);
    return;
  }
  if (!strisint(v1->str_word)) {
    eval_error("TYPE ERROR", v);
    stack_push(stack, tmp);
    return;
  }
  int v1val = atoi(v1->str_word->value);
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
  value_free(tmp);
}

/* instant 0 crankall */
void cog_reset(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_free(cur->cranks, free);
  cur->cranks = init_stack(10);
}

void cog_crankbase(value_t *v) {
  value_t *v1 = init_value(VWORD);
  contain_t *cur = stack_peek(STACK);
  int(*cr)[2] = cur->cranks->items[0];
  int base = cr[0][1];
  char *str = malloc(sizeof(char) * (int)(log10(base) + 2));
  sprintf(str, "%d", base);
  v1->str_word = init_string(str);
  free(str);
  stack_push(cur->stack, v1);
}

void add_funcs_cranker(ht_t *flit) {
  add_func(flit, cog_metacrank, "metacrank");
  add_func(flit, cog_crank, "crank");
  add_func(flit, cog_crankall, "crankall");
  add_func(flit, cog_reset, "reset");
}
