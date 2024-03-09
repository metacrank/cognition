#include <builtinslib.h>
#include <macros.h>
#include <ctype.h>
#include <stdio.h>

bool strisint(string_t *s) {
  for (int i = 0; i < s->length; i++) {
    if (!isdigit(s->value[i]))
      return false;
  }
  return true;
}

bool word_truth(value_t *v) {
  string_t *str = v->str_word;
  return str->length;
}

void contain_copy_attributes(contain_t *c, contain_t *newc) {
  if (c == NULL) {
    return NULL;
  }
  newc->flit = ht_copy(c->flit, value_stack_copy);
  newc->word_table = ht_copy(c->word_table, contain_value_copy);
  newc->cranks = stack_copy(c->cranks, cranks_copy);
  newc->faliases = stack_copy(c->faliases, falias_copy);
  newc->delims = string_copy(c->delims);
  newc->ignored = string_copy(c->ignored);
  newc->dflag = c->dflag;
  newc->iflag = c->iflag;
}

void print_value(value_t *v, const char *end) {
  custom_t *c;
  switch (v->type) {
    case VWORD:
      printf("'");
      for (int i = 0; i < v->str_word->length; i++) {
        if (v->str_word->value[i] == '\n') {
          printf("\\n");
        }
        else printf("%c", v->str_word->value[i]);
      }
      printf("'");
      break;
    case VSTACK:
      printf("[ ");
      for (int i = 0; i < v->container->stack->size; i++) {
        print_value(v->container->stack->items[i], " ");
      }
      printf("]");
      break;
    case VERR:
      printf("'%s':%s%s%s", v->error->str_word->value, RED, v->error->error->value, COLOR_RESET);
      break;
    case VCUSTOM:
      c = ht_get(ot_get(), v->str_word);
      c->printfunc(v->custom);
      break;
    case VCLIB:
      printf("CLIB_FUNC");
  }
  printf("%s", end);
}
