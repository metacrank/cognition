#include <cognition.h>
#include <cranker.h>

extern stack_t *STACK;
/* n m metacrank sets metacrank n to period m */
void cog_metacrank(value_t *v) {
  contain_t *cur = stack_pop(STACK);
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

  if (v1->type != VINT || v2->type != VINT) {
    value_free(v1);
    value_free(v2);
    return;
  }
  stack_t *cranks = cur->cranks;
  if (v1->int_float >= cranks->size) {
    for (int _ = 0; _ < v1->int_float - cranks->size + 1; _++) {
      int(*arr)[2] = malloc(sizeof(int[2]));
      arr[0][0] = 0;
      arr[0][1] = 0;
      stack_push(cranks, arr);
    }
  }
  int(*cr)[2] = cranks->items[(int)v1->int_float];
  *cr[0] = 0;
  *cr[1] = v2->int_float;
  value_free(v1);
  value_free(v2);
}

/*sets 0th crank value to specified period*/
void cog_crank(value_t *v) {
  contain_t *cur = stack_pop(STACK);
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
  if (v1->type != VINT) {
    value_free(v1);
    return;
  }
  stack_t *cranks = cur->cranks;
  if (stack->size == 0) {
    int(*arr)[2] = malloc(sizeof(int[2]));
    arr[0][0] = 0;
    arr[0][1] = 0;
    stack_push(cranks, arr);
  }
  int(*c)[2] = cranks->items[0];
  *c[1] = v1->int_float;
  value_free(v1);
}

/* sets all crank values to specified period */
void cog_crankall(value_t *v) {}

/* instant 0 crankall */
void cog_reset(value_t *v) {}

void add_funcs_cranker(ht_t *flit) {}
