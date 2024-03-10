#include <builtins/io.h>
#include <builtinslib.h>
#include <stdio.h>
#include <macros.h>

extern stack_t *STACK;

void cog_questionmark(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  printf("%sSTACK:%s\n", GRN, COLOR_RESET);
  for (int i = 0; i < cur->stack->size; i++) {
    print_value(cur->stack->items[i], "\n");
  }
  printf("\n");
}

void cog_period(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *elt = stack_pop(cur->stack);
  if (elt == NULL) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  print_value(elt, "\n");
  value_free(elt);
}

void cog_print(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *v1 = stack_pop(cur->stack);
  if (v1 == NULL)
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  if (v1->container->stack->size == 0) {
    stack_push(cur->stack, v1);
    eval_error("TYPE ERROR", v);
    return;
  }
  value_t *word = v1->container->stack->items[0];
  if (word->type != VWORD) {
    stack_push(cur->stack, v1);
    eval_error("TYPE ERROR", v);
    return;
  }
  printf("%s", word->str_word->value);
  value_free(v1);
}

void cog_read(value_t *v) {}

void add_funcs_io(ht_t* flit) {
  add_func(flit, cog_questionmark, "?");
  add_func(flit, cog_period, ".");
  add_func(flit, cog_print, "print");
}
