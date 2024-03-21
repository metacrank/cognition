#include <builtins/math.h>
#include <builtinslib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

extern stack_t *STACK;


void cog_multiply(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (stack->size < 2) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *v2 = stack_pop(stack);
  if (value_stack(v2)[0]->size != 1) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v2);
    return;
  }
  value_t *w2 = value_stack(v2)[0]->items[0];
  if (w2->type != VWORD) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v2);
    return;
  }
  value_t *v1 = stack_pop(stack);
  if (value_stack(v1)[0]->size != 1) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    stack_push(stack, v2);
    return;
  }
  value_t *w1 = value_stack(v1)[0]->items[0];
  if (w1->type != VWORD) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    stack_push(stack, v2);
    return;
  }
  double y = atof(w1->str_word->value);
  double x = atof(w2->str_word->value);
  double z = x * y;
  int len = snprintf(NULL, 0, "%f", z);
  char *result = malloc(len + 10);
  snprintf(result, len, "%f", z);
  /* string_t *s = calloc(1, sizeof(string_t)); */
  free(w1->str_word->value);
  w1->str_word->bufsize = len + 10;
  w1->str_word->length = len;
  w1->str_word->value = result;
  value_free_safe(v2);
  stack_push(stack, v1);
}
void cog_add(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (stack->size < 2) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *v2 = stack_pop(stack);
  if (value_stack(v2)[0]->size != 1) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v2);
    return;
  }
  value_t *w2 = value_stack(v2)[0]->items[0];
  if (w2->type != VWORD) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v2);
    return;
  }
  value_t *v1 = stack_pop(stack);
  if (value_stack(v1)[0]->size != 1) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    stack_push(stack, v2);
    return;
  }
  value_t *w1 = value_stack(v1)[0]->items[0];
  if (w1->type != VWORD) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    stack_push(stack, v2);
    return;
  }
  double y = atof(w1->str_word->value);
  double x = atof(w2->str_word->value);
  double z = x + y;
  int len = snprintf(NULL, 0, "%f", z);
  char *result = malloc(len + 10);
  snprintf(result, len, "%f", z);
  /* string_t *s = calloc(1, sizeof(string_t)); */
  free(w1->str_word->value);
  w1->str_word->bufsize = len + 10;
  w1->str_word->length = len;
  w1->str_word->value = result;
  value_free_safe(v2);
  stack_push(stack, v1);
}

void cog_pow(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (stack->size < 2) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *v2 = stack_pop(stack);
  if (value_stack(v2)[0]->size != 1) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v2);
    return;
  }
  value_t *w2 = value_stack(v2)[0]->items[0];
  if (w2->type != VWORD) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v2);
    return;
  }
  value_t *v1 = stack_pop(stack);
  if (value_stack(v1)[0]->size != 1) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    stack_push(stack, v2);
    return;
  }
  value_t *w1 = value_stack(v1)[0]->items[0];
  if (w1->type != VWORD) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    stack_push(stack, v2);
    return;
  }
  double y = atof(w1->str_word->value);
  double x = atof(w2->str_word->value);
  double z = pow(x, y);
  int len = snprintf(NULL, 0, "%f", z);
  char *result = malloc(len + 10);
  snprintf(result, len, "%f", z);
  /* string_t *s = calloc(1, sizeof(string_t)); */
  free(w1->str_word->value);
  w1->str_word->bufsize = len + 10;
  w1->str_word->length = len;
  w1->str_word->value = result;
  value_free_safe(v2);
  stack_push(stack, v1);
}
void cog_divide(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (stack->size < 2) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *v2 = stack_pop(stack);
  if (value_stack(v2)[0]->size != 1) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v2);
    return;
  }
  value_t *w2 = value_stack(v2)[0]->items[0];
  if (w2->type != VWORD) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v2);
    return;
  }
  value_t *v1 = stack_pop(stack);
  if (value_stack(v1)[0]->size != 1) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    stack_push(stack, v2);
    return;
  }
  value_t *w1 = value_stack(v1)[0]->items[0];
  if (w1->type != VWORD) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    stack_push(stack, v2);
    return;
  }
  double y = atof(w1->str_word->value);
  double x = atof(w2->str_word->value);
  double z = x / y;
  int len = snprintf(NULL, 0, "%f", z);
  char *result = malloc(len + 10);
  snprintf(result, len, "%f", z);
  /* string_t *s = calloc(1, sizeof(string_t)); */
  free(w1->str_word->value);
  w1->str_word->bufsize = len + 10;
  w1->str_word->length = len;
  w1->str_word->value = result;
  value_free_safe(v2);
  stack_push(stack, v1);
}

void cog_subtract(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (stack->size < 2) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *v2 = stack_pop(stack);
  if (value_stack(v2)[0]->size != 1) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v2);
    return;
  }
  value_t *w2 = value_stack(v2)[0]->items[0];
  if (w2->type != VWORD) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v2);
    return;
  }
  value_t *v1 = stack_pop(stack);
  if (value_stack(v1)[0]->size != 1) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    stack_push(stack, v2);
    return;
  }
  value_t *w1 = value_stack(v1)[0]->items[0];
  if (w1->type != VWORD) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    stack_push(stack, v2);
    return;
  }
  double y = atof(w1->str_word->value);
  double x = atof(w2->str_word->value);
  double z = x - y;
  int len = snprintf(NULL, 0, "%f", z);
  char *result = malloc(len + 10);
  snprintf(result, len, "%f", z);
  /* string_t *s = calloc(1, sizeof(string_t)); */
  free(w1->str_word->value);
  w1->str_word->bufsize = len + 10;
  w1->str_word->length = len;
  w1->str_word->value = result;
  value_free_safe(v2);
  stack_push(stack, v1);
}

void cog_exp(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (stack->size < 1) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *v1 = stack_pop(stack);
  if (value_stack(v1)[0]->size != 1) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    return;
  }
  value_t *w1 = value_stack(v1)[0]->items[0];
  if (w1->type != VWORD) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    return;
  }
  double x = exp(atof(w1->str_word->value));
  int len = snprintf(NULL, 0, "%f", x);
  char *result = malloc(len + 10);
  snprintf(result, len, "%f", x);
  /* string_t *s = calloc(1, sizeof(string_t)); */
  free(w1->str_word->value);
  w1->str_word->bufsize = len + 10;
  w1->str_word->length = len;
  w1->str_word->value = result;
  stack_push(stack, v1);
}

void cog_sin(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (stack->size < 1) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *v1 = stack_pop(stack);
  if (value_stack(v1)[0]->size != 1) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    return;
  }
  value_t *w1 = v1->container->stack->items[0];
  if (w1->type != VWORD) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    return;
  }
  double x = sin(atof(w1->str_word->value));
  int len = snprintf(NULL, 0, "%f", x);
  char *result = malloc(len + 10);
  snprintf(result, len, "%f", x);
  /* string_t *s = calloc(1, sizeof(string_t)); */
  free(w1->str_word->value);
  w1->str_word->bufsize = len + 10;
  w1->str_word->length = len;
  w1->str_word->value = result;
  stack_push(stack, v1);
}

void cog_cos(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (stack->size < 1) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *v1 = stack_pop(stack);
  if (value_stack(v1)[0]->size != 1) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    return;
  }
  value_t *w1 = value_stack(v1)[0]->items[0];
  if (w1->type != VWORD) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    return;
  }
  double x = cos(atof(w1->str_word->value));
  int len = snprintf(NULL, 0, "%f", x);
  char *result = malloc(len + 10);
  snprintf(result, len, "%f", x);
  /* string_t *s = calloc(1, sizeof(string_t)); */
  free(w1->str_word->value);
  w1->str_word->bufsize = len + 10;
  w1->str_word->length = len;
  w1->str_word->value = result;
  stack_push(stack, v1);
}

void cog_floor(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (stack->size < 1) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *v1 = stack_pop(stack);
  if (value_stack(v1)[0]->size != 1) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    return;
  }
  value_t *w1 = value_stack(v1)[0]->items[0];
  if (w1->type != VWORD) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    return;
  }
  double x = floor(atof(w1->str_word->value));
  int len = snprintf(NULL, 0, "%f", x);
  char *result = malloc(len + 10);
  snprintf(result, len, "%f", x);
  /* string_t *s = calloc(1, sizeof(string_t)); */
  free(w1->str_word->value);
  w1->str_word->bufsize = len + 10;
  w1->str_word->length = len;
  w1->str_word->value = result;
  stack_push(stack, v1);
}

void cog_ceil(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (stack->size < 1) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *v1 = stack_pop(stack);
  if (value_stack(v1)[0]->size != 1) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    return;
  }
  value_t *w1 = value_stack(v1)[0]->items[0];
  if (w1->type != VWORD) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    return;
  }
  double x = ceil(atof(w1->str_word->value));
  int len = snprintf(NULL, 0, "%f", x);
  char *result = malloc(len + 10);
  snprintf(result, len, "%f", x);
  /* string_t *s = calloc(1, sizeof(string_t)); */
  free(w1->str_word->value);
  w1->str_word->bufsize = len + 10;
  w1->str_word->length = len;
  w1->str_word->value = result;
  stack_push(stack, v1);
}

void cog_ln(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (stack->size < 1) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *v1 = stack_pop(stack);
  if (value_stack(v1)[0]->size != 1) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    return;
  }
  value_t *w1 = value_stack(v1)[0]->items[0];
  if (w1->type != VWORD) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    return;
  }
  double x = log(atof(w1->str_word->value));
  int len = snprintf(NULL, 0, "%f", x);
  char *result = malloc(len + 10);
  snprintf(result, len, "%f", x);
  /* string_t *s = calloc(1, sizeof(string_t)); */
  free(w1->str_word->value);
  w1->str_word->bufsize = len + 10;
  w1->str_word->length = len;
  w1->str_word->value = result;
  stack_push(stack, v1);
}

void cog_neg(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (stack->size < 1) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *v1 = stack_pop(stack);
  if (value_stack(v1)[0]->size != 1) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    return;
  }
  value_t *w1 = value_stack(v1)[0]->items[0];
  if (w1->type != VWORD) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    return;
  }
  double x = atof(w1->str_word->value) * -1;
  int len = snprintf(NULL, 0, "%f", x);
  char *result = malloc(len + 10);
  snprintf(result, len, "%f", x);
  /* string_t *s = calloc(1, sizeof(string_t)); */
  free(w1->str_word->value);
  w1->str_word->bufsize = len + 10;
  w1->str_word->length = len;
  w1->str_word->value = result;
  stack_push(stack, v1);
}

void cog_int(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (stack->size < 1) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *v1 = stack_pop(stack);
  if (value_stack(v1)[0]->size != 1) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    return;
  }
  value_t *w1 = value_stack(v1)[0]->items[0];
  if (w1->type != VWORD) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    return;
  }
  int x = (int) atof(w1->str_word->value);
  int len = (int)((ceil(log10(x))+1)*sizeof(char));
  char *result = malloc(len + 10);
  snprintf(result, len, "%d", x);
  /* string_t *s = calloc(1, sizeof(string_t)); */
  free(w1->str_word->value);
  w1->str_word->bufsize = len + 10;
  w1->str_word->length = len;
  w1->str_word->value = result;
  stack_push(stack, v1);
}

void cog_equals(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (stack->size < 2) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *v2 = stack_pop(stack);
  if (value_stack(v2)[0]->size != 1) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v2);
    return;
  }
  value_t *w2 = value_stack(v2)[0]->items[0];
  if (w2->type != VWORD) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v2);
    return;
  }
  value_t *v1 = stack_pop(stack);
  if (value_stack(v1)[0]->size != 1) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    stack_push(stack, v2);
    return;
  }
  value_t *w1 = value_stack(v1)[0]->items[0];
  if (w1->type != VWORD) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    stack_push(stack, v2);
    return;
  }
  if (strcmp(w1->str_word->value, w2->str_word->value) == 0) {
    w1->str_word->length = 1;
    w1->str_word->value[0] = 't';
    w1->str_word->value[1] = '\0';
  } else {
    w1->str_word->length = 0;
    w1->str_word->value[0] = '\0';
  }
  value_free_safe(v2);
  stack_push(stack, v1);
}

void cog_nequals(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (stack->size < 2) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *v2 = stack_pop(stack);
  if (value_stack(v2)[0]->size != 1) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v2);
    return;
  }
  value_t *w2 = value_stack(v2)[0]->items[0];
  if (w2->type != VWORD) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v2);
    return;
  }
  value_t *v1 = stack_pop(stack);
  if (value_stack(v1)[0]->size != 1) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    stack_push(stack, v2);
    return;
  }
  value_t *w1 = value_stack(v1)[0]->items[0];
  if (w1->type != VWORD) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    stack_push(stack, v2);
    return;
  }
  if (strcmp(w1->str_word->value, w2->str_word->value) != 0) {
    w1->str_word->length = 1;
    w1->str_word->value[0] = 't';
    w1->str_word->value[1] = '\0';
  } else {
    w1->str_word->length = 0;
    w1->str_word->value[0] = '\0';
  }
  value_free_safe(v2);
  stack_push(stack, v1);
}

void add_funcs_math(ht_t *flit) {
  add_func(flit, cog_equals, "=");
  add_func(flit, cog_nequals, "!=");
  add_func(flit, cog_add, "+");
  add_func(flit, cog_subtract, "-");
  add_func(flit, cog_multiply, "*");
  add_func(flit, cog_divide, "/");
  add_func(flit, cog_neg, "neg");
  add_func(flit, cog_pow, "pow");
  add_func(flit, cog_floor, "floor");
  add_func(flit, cog_ceil, "ceil");
  add_func(flit, cog_cos, "cos");
  add_func(flit, cog_ln, "ln");
  add_func(flit, cog_sin, "sin");
  add_func(flit, cog_exp, "exp");
  add_func(flit, cog_int, "int");
}
