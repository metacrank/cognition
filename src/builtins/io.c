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
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  print_value(elt, "\n");
  value_free_safe(elt);
}

void cog_print(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *v1 = stack_pop(cur->stack);
  if (v1 == NULL) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  if (value_stack(v1)[0]->size == 0) {
    stack_push(cur->stack, v1);
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  value_t *word = value_stack(v1)[0]->items[0];
  if (word->type != VWORD) {
    stack_push(cur->stack, v1);
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  print(word->str_word);
  value_free_safe(v1);
}

void cog_read(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *strval = init_value(VWORD);
  strval->str_word = get_line(stdin);
  push_quoted(cur, strval);
}

void add_funcs_io(ht_t* flit) {
  add_func(flit, cog_questionmark, U"?");
  add_func(flit, cog_period, U".");
  add_func(flit, cog_print, U"print");
  add_func(flit, cog_read, U"read");
}
