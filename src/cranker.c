#include <cognition.h>
#include <cranker.h>
#include <ctype.h>

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
    value_free(tmp);
    return;
  }
  value_t *v2 = stack_pop(ctmp->stack);

  value_t *tmp2 = stack_pop(stack);
  if (!tmp2) {
    value_free(tmp);
    return;
  }
  contain_t *ctmp2 = tmp2->container;
  if (ctmp2->stack->size != 1) {
    value_free(tmp);
    return;
  }
  value_t *v1 = stack_pop(ctmp->stack);
  if (v1 == NULL) {
    /* TODO: error out */
    value_free(v2);
    return;
  }

  if (v1->type != VWORD || v2->type != VWORD) {
    value_free(v1);
    value_free(v2);
    return;
  }
  if (!(strisint(v1->str_word) && strisint(v1->str_word))) {
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
  int(*cr)[2] = cranks->items[(int)v1val];
  *cr[0] = 0;
  *cr[1] = v2val;
  value_free(v1);
  value_free(v2);
}

/*sets 0th crank value to specified period*/
void cog_crank(value_t *v) {
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
  value_t *v1 = stack_pop(ctmp->stack);
  value_free(tmp);
  if (v1->type != VWORD) {
    value_free(v1);
    return;
  }
  if (!strisint(v1->str_word)) {
    value_free(v1);
    return;
  }
  int v1val = atoi(v1->str_word->value);
  stack_t *cranks = cur->cranks;
  if (stack->size == 0) {
    int(*arr)[2] = malloc(sizeof(int[2]));
    arr[0][0] = 0;
    arr[0][1] = 0;
    stack_push(cranks, arr);
  }
  int(*c)[2] = cranks->items[0];
  *c[1] = v1val;
  value_free(v1);
}

/* sets all crank values to specified period */
void cog_crankall(value_t *v) {}

/* instant 0 crankall */
void cog_reset(value_t *v) {}

void add_funcs_cranker(ht_t *flit) {}
