#include <io.h>
#include <stdio.h>
#include <macros.h>

extern stack_t *STACK;

void print_value(value_t *v) {
  custom_t *c;
  switch (v->type) {
    case VWORD:
      printf("W: %s\n", v->str_word->value);
      break;
    case VSTACK:
      printf("Q: [\n");
      for (int i = 0; i < v->container->stack->size; i++) {
        print_value(v->container->stack->items[i]);
      }
      printf("]\n");
      break;
    case VERR:
      printf("%sERROR%s: %s\n", RED, COLOR_RESET, v->str_word->value);
      break;
    case VCUSTOM:
      c = ht_get(ot_get(), v->str_word);
      c->printfunc(v->custom);
      break;
    case VCLIB:
      printf("CLIB_FUNC\n");
  }
}

void cog_questionmark(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  for (int i = 0; i < cur->stack->size; i++) {
    print_value(cur->stack->items[i]);
  }
}

void add_funcs_io(ht_t* flit) {
  add_func(flit, cog_questionmark, "?");
}
