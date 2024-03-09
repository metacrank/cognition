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
  print_value(elt, "\n");
  value_free(elt);
}

void add_funcs_io(ht_t* flit) {
  add_func(flit, cog_questionmark, "?");
  add_func(flit, cog_period, ".");
}
