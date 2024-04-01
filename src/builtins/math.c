#include <builtins/math.h>
#include <builtinslib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <strnum.h>

extern stack_t *STACK;

void cog_multiply(value_t *v) {
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
  long double x = string_to_double(w1->str_word);
  long double y = string_to_double(w2->str_word);
  long double z = x * y;
  string_free(w1->str_word);
  w1->str_word = double_to_string(z);
  value_free_safe(v2);
  stack_push(stack, v1);
}
void cog_add(value_t *v) {
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
  string_t *stmp = sum(w1->str_word, w2->str_word, NULL, NULL, NULL, NULL);
  string_free(w1->str_word);
  w1->str_word = stmp;
  value_free_safe(v2);
  stack_push(stack, v1);
}

void cog_pow(value_t *v) {
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
  long double x = string_to_double(w1->str_word);
  long double y = string_to_double(w2->str_word);
  long double z = pow(x, y);
  string_free(w1->str_word);
  w1->str_word = double_to_string(z);
  value_free_safe(v2);
  stack_push(stack, v1);
}
void cog_divide(value_t *v) {
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
  long double x = string_to_double(w1->str_word);
  long double y = string_to_double(w2->str_word);
  long double z = x / y;
  string_free(w1->str_word);
  w1->str_word = double_to_string(z);
  value_free_safe(v2);
  stack_push(stack, v1);
}

void cog_subtract(value_t *v) {
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
  long double x = string_to_double(w1->str_word);
  long double y = string_to_double(w2->str_word);
  long double z = x - y;
  string_free(w1->str_word);
  w1->str_word = double_to_string(z);
  value_free_safe(v2);
  stack_push(stack, v1);
}

void cog_exp(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (stack->size < 1) {
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
  long double x = string_to_double(w1->str_word);
  w1->str_word = double_to_string(exp(x));
  stack_push(stack, v1);
}

void cog_sin(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (stack->size < 1) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *v1 = stack_pop(stack);
  if (value_stack(v1)[0]->size != 1) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    return;
  }
  value_t *w1 = v1->container->stack->items[0];
  if (w1->type != VWORD) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(stack, v1);
    return;
  }
  long double x = string_to_double(w1->str_word);
  w1->str_word = double_to_string(sin(x));
  stack_push(stack, v1);
}

void cog_cos(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (stack->size < 1) {
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
  long double x = string_to_double(w1->str_word);
  w1->str_word = double_to_string(cos(x));
  stack_push(stack, v1);
}

void cog_floor(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (stack->size < 1) {
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
  long double x = string_to_double(w1->str_word);
  w1->str_word = double_to_string(floor(x));
  stack_push(stack, v1);
}

void cog_ceil(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (stack->size < 1) {
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
  long double x = string_to_double(w1->str_word);
  w1->str_word = double_to_string(ceil(x));
  stack_push(stack, v1);
}

void cog_ln(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (stack->size < 1) {
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
  long double x = string_to_double(w1->str_word);
  w1->str_word = double_to_string(log(x));
  stack_push(stack, v1);
}

void cog_neg(value_t *v) {
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
  neg(w1->str_word);
}

void cog_ip(value_t *v) {
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
  ip(w1->str_word);
}

void cog_fp(value_t *v) {
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
  fp(w1->str_word);
}

void cog_equals(value_t *v) {
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
  if (string_comp(w1->str_word, w2->str_word) == 0) {
    w1->str_word->length = 0;
    string_append(w1->str_word, 't');
  } else {
    w1->str_word->length = 0;
  }
  value_free_safe(v2);
  stack_push(stack, v1);
}

void cog_nequals(value_t *v) {
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
  if (string_comp(w1->str_word, w2->str_word) != 0) {
    w1->str_word->length = 0;
    string_append(w1->str_word, 't');
  } else {
    w1->str_word->length = 0;
  }
  value_free_safe(v2);
  stack_push(stack, v1);
}

void add_funcs_math(ht_t *flit) {
  add_func(flit, cog_equals, U"=");
  add_func(flit, cog_nequals, U"!=");
  add_func(flit, cog_add, U"+");
  add_func(flit, cog_subtract, U"-");
  add_func(flit, cog_multiply, U"*");
  add_func(flit, cog_divide, U"/");
  add_func(flit, cog_neg, U"neg");
  add_func(flit, cog_pow, U"pow");
  add_func(flit, cog_floor, U"floor");
  add_func(flit, cog_ceil, U"ceil");
  add_func(flit, cog_cos, U"cos");
  add_func(flit, cog_ln, U"ln");
  add_func(flit, cog_sin, U"sin");
  add_func(flit, cog_exp, U"exp");
  add_func(flit, cog_ip, U"ip");
  add_func(flit, cog_fp, U"fp");
}
