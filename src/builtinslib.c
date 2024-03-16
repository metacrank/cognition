#include <builtinslib.h>
#include <macros.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

/* macros taken from stackoverflow */
#define MAX 1000
#define JUSTDO(a)                                                              \
  if (!(a)) {                                                                  \
    perror(#a);                                                                \
    exit(1);                                                                   \
  }

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
  newc->singlets = string_copy(c->singlets);
  newc->dflag = c->dflag;
  newc->iflag = c->iflag;
  newc->sflag = c->sflag;
}

void print_str_formatted(string_t *string) {
  if (!string) return;
  for (int i = 0; i < string->length; i++) {
    if (string->value[i] == '\n') {
      printf("\\n");
    } else if (string->value[i] == '\t') {
      printf("\\t");
    } else {
      printf("%c", string->value[i]);
    }
  }
}

void print_value(value_t *v, const char *end) {
  custom_t *c;
  switch (v->type) {
    case VWORD:
      printf("'");
      print_str_formatted(v->str_word);
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

/* taken from stackoverflow */
char *get_line(FILE *f) {
  int len = MAX;
  char buf[MAX], *e = NULL, *ret;
  JUSTDO(ret = calloc(MAX, 1));
  while (fgets(buf, MAX, f)) {
    if (len - strlen(ret) < MAX)
      JUSTDO(ret = realloc(ret, len *= 2));
    strcat(ret, buf);
    if ((e = strrchr(ret, '\n')))
      break;
  }
  /* stackoverflow code patch: clearerr */
  clearerr(f);
  if (e)
    *e = '\0';
  return ret;
}
