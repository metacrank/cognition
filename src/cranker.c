#include <cognition.h>
#include <cranker.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>

extern stack_t *STACK;

bool strisint(string_t *s) {
  for (int i = 0; i < s->length; i++) {
    if (!isdigit(s->value[i]))
      return false;
  }
  return true;
}
/* n m metacrank sets metacrank n to period m */
void cog_metacrank(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  value_t *tmp = stack_pop(stack);
  if (!tmp) {
    return;
  }
  contain_t *ctmp = tmp->container;
  if (ctmp->stack->size != 1) {
    /* TODO: error out */
    value_free(tmp);
    return;
  }
  value_t *v2 = stack_pop(ctmp->stack);

  value_t *tmp2 = stack_pop(stack);
  if (!tmp) {
    eval_error("EMPTY STACK");
    value_free(tmp);
    return;
  }
  contain_t *ctmp2 = tmp2->container;
  if (ctmp2->stack->size != 1) {
    eval_error("TYPE ERROR");
    value_free(tmp);
    value_free(tmp2);
    return;
  }
  value_t *v1 = stack_pop(ctmp2->stack);
  // never going to be NULL?
  /* if (v1 == NULL) { */
  /*   /\* TODO: error out *\/ */
  /*   value_free(v2); */
  /*   return; */
  /* } */
  value_free(tmp);
  value_free(tmp2);

  if (v1->type != VWORD || v2->type != VWORD) {
    eval_error("TYPE ERROR");
    value_free(v1);
    value_free(v2);
    return;
  }
  if (!(strisint(v1->str_word) && strisint(v1->str_word))) {
    eval_error("TYPE ERROR");
    value_free(v1);
    value_free(v2);
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
  value_free(v1);
  value_free(v2);
}

/*sets 0th crank value to specified period*/
void cog_crank(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  value_t *tmp = stack_pop(stack);
  if (!tmp) {
    eval_error("EMPTY STACK");
    return;
  }
  contain_t *ctmp = tmp->container;
  if (ctmp->stack->size != 1) {
    eval_error("TYPE ERROR");
    value_free(tmp);
    return;
  }
  value_t *v1 = stack_pop(ctmp->stack);
  value_free(tmp);
  if (v1->type != VWORD) {
    eval_error("TYPE ERROR");
    value_free(v1);
    return;
  }
  if (!strisint(v1->str_word)) {
    eval_error("TYPE ERROR");
    value_free(v1);
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
  value_free(v1);
}

/* sets all crank values to specified period */
void cog_crankall(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  value_t *tmp = stack_pop(stack);
  if (!tmp) {
    eval_error("EMPTY STACK");
    return;
  }
  contain_t *ctmp = tmp->container;
  if (ctmp->stack->size != 1) {
    eval_error("TYPE ERROR");
    value_free(tmp);
    return;
  }
  value_t *v1 = stack_pop(ctmp->stack);
  value_free(tmp);
  if (v1->type != VWORD) {
    eval_error("TYPE ERROR");
    value_free(v1);
    return;
  }
  if (!strisint(v1->str_word)) {
    eval_error("TYPE ERROR");
    value_free(v1);
    return;
  }
  int v1val = atoi(v1->str_word->value);
  value_free(v1);
  stack_t *cranks = cur->cranks;
  if (cranks->size <= stack->size) {
    for (int i = 0; i < cranks->size; i++) {
      int(*arr)[2] = cranks->items[i];
      arr[0][0] = (v1val > 1);
      arr[0][1] = v1val;
    }
    for (int i = cranks->size; i < stack->size; i++) {
      int(*arr)[2] = malloc(sizeof(int[2]));
      arr[0][0] = (v1val > 1);
      arr[0][1] = v1val;
      stack_push(cranks, arr);
    }
  } else {
    for (int i = 0; i < stack->size; i++) {
      int(*arr)[2] = cranks->items[i];
      arr[0][0] = (v1val > 1);
      arr[0][1] = v1val;
    }
    for (int i = stack->size; i < cranks->size; i++) {
      free(stack_pop(cranks));
    }
  }
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
