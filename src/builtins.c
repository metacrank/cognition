#include <builtins.h>
#include <cognition.h>
#include <ctype.h>
#include <dlfcn.h>
#include <macros.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* macros taken from stackoverflow */
#define MAX 1000
#define JUSTDO(a)                                                              \
  if (!(a)) {                                                                  \
    perror(#a);                                                                \
    exit(1);                                                                   \
  }

extern stack_t *STACK;
extern stack_t *EVAL_STACK;
extern ht_t *WORD_TABLE;
extern parser_t *PARSER;

extern ht_t *FLIT;
extern ht_t *OBJ_TABLE;

/* TODO: switch (for quotes), del (deleting entries from quotes, strings, words)
 */

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

void print_value(value_t *v) {
  custom_t *c;
  switch (v->type) {
  case VINT:
    printf("%.0Lf\n", v->int_float);
    break;
  case VFLOAT:
    printf("%Lf\n", v->int_float);
    break;
  case VSTR:
    printf("%s", v->str_word->value);
    break;
  case VWORD:
    printf("W: %s\n", v->str_word->value);
    break;
  case VQUOTE:
    printf("Q: [\n");
    for (int i = 0; i < v->quote->size; i++) {
      print_value(v->quote->items[i]);
    }
    printf("]\n");
    break;
  case VERR:
    printf("%sERROR%s: %s\n", RED, COLOR_RESET, v->str_word->value);
    break;
  case VCUSTOM:
    c = ht_get(OBJ_TABLE, v->str_word);
    c->printfunc(v->custom);
    break;
  }
}

void eval_error(char *s) {
  value_t *v = init_value(VERR);
  v->str_word = init_string(s);
  stack_push(STACK, v);
}

void stemadd(value_t *v) {
  value_t *v2 = stack_pop(STACK);
  if (v2 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    stack_push(STACK, v2);
    eval_error("EMPTY STACK");
    return;
  }
  if (v1->type != VINT && v1->type != VFLOAT && v2->type != VINT &&
      v2->type != VFLOAT) {
    stack_push(STACK, v1);
    stack_push(STACK, v2);
    eval_error("INCORRECT TYPE ARGUMENT");
    return;
  }

  v1->int_float = v1->int_float + v2->int_float;
  if (v2->type == VFLOAT)
    v1->type = VFLOAT;

  stack_push(STACK, v1);
  value_free(v2);
}

void stemsub(value_t *v) {
  value_t *v2 = stack_pop(STACK);
  if (v2 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    stack_push(STACK, v2);
    eval_error("EMPTY STACK");
    return;
  }
  if (v1->type != VINT && v1->type != VFLOAT && v2->type != VINT &&
      v2->type != VFLOAT) {
    stack_push(STACK, v1);
    stack_push(STACK, v2);
    eval_error("INCORRECT TYPE ARGUMENT");
    return;
  }

  v1->int_float = v1->int_float - v2->int_float;
  if (v2->type == VFLOAT)
    v1->type = VFLOAT;

  stack_push(STACK, v1);
  value_free(v2);
}

void stemmul(value_t *v) {
  value_t *v2 = stack_pop(STACK);
  if (v2 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    stack_push(STACK, v2);
    eval_error("EMPTY STACK");
    return;
  }
  if (v1->type != VINT && v1->type != VFLOAT && v2->type != VINT &&
      v2->type != VFLOAT) {
    stack_push(STACK, v1);
    stack_push(STACK, v2);
    eval_error("INCORRECT TYPE ARGUMENT");
    return;
  }

  v1->int_float = v1->int_float * v2->int_float;
  if (v2->type == VFLOAT)
    v1->type = VFLOAT;

  stack_push(STACK, v1);
  value_free(v2);
}

void stemdiv(value_t *v) {
  value_t *v2 = stack_pop(STACK);
  if (v2 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    stack_push(STACK, v2);
    eval_error("EMPTY STACK");
    return;
  }
  if (v1->type != VINT && v1->type != VFLOAT && v2->type != VINT &&
      v2->type != VFLOAT) {
    stack_push(STACK, v1);
    stack_push(STACK, v2);
    eval_error("INCORRECT TYPE ARGUMENT");
    return;
  }

  v1->int_float = v1->int_float / v2->int_float;
  if (v2->type == VFLOAT)
    v1->type = VFLOAT;

  stack_push(STACK, v1);
  value_free(v2);
}

void stemand(value_t *v) {
  value_t *v2 = stack_pop(STACK);
  if (v2 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    stack_push(STACK, v2);
    eval_error("EMPTY STACK");
    return;
  }
  if (v1->type != VINT && v1->type != VFLOAT && v2->type != VINT &&
      v2->type != VFLOAT) {
    stack_push(STACK, v1);
    stack_push(STACK, v2);
    eval_error("INCORRECT TYPE ARGUMENT");
    return;
  }

  v1->int_float = v1->int_float && v2->int_float;
  if (v2->type == VFLOAT)
    v1->type = VFLOAT;

  stack_push(STACK, v1);
  value_free(v2);
}

void stemor(value_t *v) {
  value_t *v2 = stack_pop(STACK);
  if (v2 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    stack_push(STACK, v2);
    eval_error("EMPTY STACK");
    return;
  }
  if (v1->type != VINT && v1->type != VFLOAT && v2->type != VINT &&
      v2->type != VFLOAT) {
    stack_push(STACK, v1);
    stack_push(STACK, v2);
    eval_error("INCORRECT TYPE ARGUMENT");
    return;
  }

  v1->int_float = v1->int_float || v2->int_float;
  if (v2->type == VFLOAT)
    v1->type = VFLOAT;

  stack_push(STACK, v1);
  value_free(v2);
}

void stemfunc(value_t *v) {
  value_t *v2 = stack_pop(STACK);
  if (v2 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    stack_push(STACK, v2);
    eval_error("EMPTY STACK");
    return;
  }
  if (v1->type != VWORD) {
    eval_error("INCORRECT TYPE ARGUMENT");
    return;
  }
  ht_add(WORD_TABLE, string_copy(v1->str_word), v2, value_free);
  value_free(v1);
}

void nop(value_t *v) {}

void stempow(value_t *v) {
  value_t *v2 = stack_pop(STACK);
  if (v2 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    stack_push(STACK, v2);
    eval_error("EMPTY STACK");
    return;
  }
  if (v1->type != VINT && v1->type != VFLOAT && v2->type != VINT &&
      v2->type != VFLOAT) {
    stack_push(STACK, v1);
    stack_push(STACK, v2);
    eval_error("INCORRECT TYPE ARGUMENT");
    return;
  }

  v1->int_float = powl(v1->int_float, v2->int_float);
  if (v2->type == VFLOAT)
    v1->type = VFLOAT;

  stack_push(STACK, v1);
  value_free(v2);
}

void stemsin(value_t *v) {
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }
  if (v1->type != VINT && v1->type != VFLOAT) {
    stack_push(STACK, v1);
    eval_error("INVALID TYPE ARGUMENT");
  }
  v1->int_float = sinl(v1->int_float);
  stack_push(STACK, v1);
}

void stemcos(value_t *v) {
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }
  if (v1->type != VINT && v1->type != VFLOAT) {
    stack_push(STACK, v1);
    eval_error("INVALID TYPE ARGUMENT");
  }
  v1->int_float = cosl(v1->int_float);
  stack_push(STACK, v1);
}

void stemexp(value_t *v) {
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }
  if (v1->type != VINT && v1->type != VFLOAT) {
    stack_push(STACK, v1);
    eval_error("INVALID TYPE ARGUMENT");
  }
  v1->int_float = expl(v1->int_float);
  stack_push(STACK, v1);
}

void stemln(value_t *v) {
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }
  if (v1->type != VINT && v1->type != VFLOAT) {
    stack_push(STACK, v1);
    eval_error("INVALID TYPE ARGUMENT");
  }
  v1->int_float = logl(v1->int_float);
  stack_push(STACK, v1);
}

void stemceil(value_t *v) {
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }
  if (v1->type != VINT && v1->type != VFLOAT) {
    stack_push(STACK, v1);
    eval_error("INVALID TYPE ARGUMENT");
  }
  v1->int_float = ceil(v1->int_float);
  stack_push(STACK, v1);
}

void stemfloor(value_t *v) {
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }
  if (v1->type != VINT && v1->type != VFLOAT) {
    stack_push(STACK, v1);
    eval_error("INVALID TYPE ARGUMENT");
  }
  v1->int_float = floor(v1->int_float);
  stack_push(STACK, v1);
}

void stemeval(value_t *v) {
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }
  if (v1->type == VQUOTE) {
    stack_push(EVAL_STACK, v1);
    for (int i = 0; i < v1->quote->size; i++) {
      eval(value_copy(v1->quote->items[i]));
    }
    value_t *vf = stack_pop(EVAL_STACK);
    if (vf) {
      value_free(vf);
    }
  } else {
    eval(v1);
  }
}

void unglue(value_t *v) {
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }
  if (v1->type != VWORD) {
    stack_push(STACK, v1);
    eval_error("EMPTY STACK");
    return;
  }
  value_t *value = ht_get(WORD_TABLE, v1->str_word);
  if (!value) {
    stack_push(STACK, v1);
    eval_error("UNBOUND WORD");
    return;
  }
  value = value_copy(value);
  stack_push(STACK, value);
  value_free(v1);
}

void strquote(value_t *v) {
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }
  if (v1->type != VSTR) {
    stack_push(STACK, v1);
    eval_error("INCORRECT TYPE ARGUMENT");
    return;
  }
  value_t *retval = init_value(VQUOTE);
  retval->quote = init_stack(10);
  char *s = malloc(strlen(v1->str_word->value) + 1);
  strcpy(s, v1->str_word->value);
  parser_t *p = parser_pp(s);
  value_t *cur;
  while (1) {
    cur = parser_get_next(p);
    if (cur == NULL)
      break;
    stack_push(retval->quote, cur);
  }
  stack_push(STACK, retval);
  value_free(v1);
  free(p->source);
  free(p);
}

void curry(value_t *v) {
  value_t *v2 = stack_pop(STACK);
  if (v2 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    stack_push(STACK, v2);
    eval_error("EMTPY STACK");
    return;
  }

  if (v2->type != VQUOTE) {
    stack_push(STACK, v1);
    stack_push(STACK, v2);
    eval_error("INCORRECT TYPE ARGUMENT");
    return;
  }

  stack_add(v2->quote, v1, 0);
  stack_push(STACK, v2);
}

void steminsert(value_t *v) {
  value_t *v3 = stack_pop(STACK);
  if (v3 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }
  value_t *v2 = stack_pop(STACK);
  if (v2 == NULL) {
    stack_push(STACK, v3);
    eval_error("EMPTY STACK");
    return;
  }
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    stack_push(STACK, v2);
    stack_push(STACK, v3);
    eval_error("EMTPY STACK");
    return;
  }

  if (v1->type != VINT) {
    stack_push(STACK, v1);
    stack_push(STACK, v2);
    stack_push(STACK, v3);
    eval_error("INCORRECT TYPE ARGUMENT");
    return;
  }

  if (v3->type != VQUOTE) {
    stack_push(STACK, v1);
    stack_push(STACK, v2);
    stack_push(STACK, v3);
    eval_error("INCORRECT TYPE ARGUMENT");
    return;
  }
  if (v1->int_float < 0 || v1->int_float >= v3->quote->size) {
    stack_push(STACK, v1);
    stack_push(STACK, v2);
    stack_push(STACK, v3);
    eval_error("INDEX ERROR");
    return;
  }

  stack_add(v3->quote, v2, (int)v1->int_float);
  stack_push(STACK, v2);
}

void stemfread(value_t *v) {
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }
  if (v1->type != VSTR) {
    stack_push(STACK, v1);
    eval_error("EMPTY STACK");
    return;
  }
  char *val = "";
  size_t len = 0;
  FILE *fp = fopen(v1->str_word->value, "rb");
  if (!fp) {
    stack_push(STACK, v1);
    eval_error("FREAD ERROR");
    return;
  }
  ssize_t bytes_read = getdelim(&val, &len, '\0', fp);
  fclose(fp);
  value_t *retval = init_value(VSTR);
  retval->str_word = init_string(val);
  stack_push(STACK, retval);
  value_free(v1);
  free(val);
}

void stemread(value_t *v) {
  value_t *retval = init_value(VSTR);
  char *a = get_line(stdin);
  retval->str_word = init_string(a);
  stack_push(STACK, retval);
  free(a);
}

void stemexit(value_t *v) {
  ht_free(WORD_TABLE, value_free);
  ht_free(FLIT, func_free);
  stack_free(STACK);
  free(PARSER->source);
  free(PARSER);
  stack_free(EVAL_STACK);
  ht_free(OBJ_TABLE, func_free);
  exit(0);
}

void quote(value_t *v) {
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }

  value_t *retval = init_value(VQUOTE);
  retval->quote = init_stack(10);
  stack_push(retval->quote, v1);
  stack_push(STACK, retval);
}

void stemtype(value_t *v) {
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }

  value_t *retval = init_value(VINT);
  retval->int_float = v1->type;
  stack_push(STACK, v1);
  stack_push(STACK, retval);
}

void dsc(value_t *v) {
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }

  value_free(v1);
}

void swap(value_t *v) {
  value_t *v2 = stack_pop(STACK);
  if (v2 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    stack_push(STACK, v2);
    eval_error("EMPTY STACK");
    return;
  }
  stack_push(STACK, v2);
  stack_push(STACK, v1);
}

void isdef(value_t *v) {
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }

  value_t *retval = init_value(VINT);
  if (v1->type != VWORD) {
    retval->int_float = 0;
  } else {
    retval->int_float = ht_exists(WORD_TABLE, v1->str_word);
  }
  stack_push(STACK, v1);
  stack_push(STACK, retval);
}

void stemdup(value_t *v) {
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }

  value_t *retval = value_copy(v1);
  stack_push(STACK, v1);
  stack_push(STACK, retval);
}

void questionmark(value_t *v) {
  for (int i = 0; i < STACK->size; i++) {
    print_value(STACK->items[i]);
  }
}

void period(value_t *v) {
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }
  print_value(v1);
  value_free(v1);
}

void stemlen(value_t *v) {
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }

  value_t *retval = init_value(VINT);
  if (v1->type == VINT || v1->type == VFLOAT) {
    retval->int_float = 1;
  } else if (v1->type == VSTR || v1->type == VWORD) {
    retval->int_float = strlen(v1->str_word->value);
  } else if (v1->type == VQUOTE) {
    retval->int_float = v1->quote->size;
  }
  stack_push(STACK, v1);
  stack_push(STACK, retval);
}

void dip(value_t *v) {
  value_t *v2 = stack_pop(STACK);
  if (v2 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    stack_push(STACK, v2);
    eval_error("EMPTY STACK");
    return;
  }

  stack_push(EVAL_STACK, v1);
  if (v2->type == VQUOTE) {
    stack_push(EVAL_STACK, v2);
    for (int i = 0; i < v2->quote->size; i++) {
      eval(value_copy(v2->quote->items[i]));
    }
    value_free(stack_pop(EVAL_STACK));
    stack_pop(EVAL_STACK);
  } else {
    eval(v2);
  }
  stack_push(STACK, v1);
}

void del(value_t *v) {
  value_t *v2 = stack_pop(STACK);
  if (v2 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    stack_push(STACK, v2);
    eval_error("EMPTY STACK");
    return;
  }

  if (v2->type != VINT) {
    stack_push(STACK, v1);
    stack_push(STACK, v2);
    eval_error("INCORRECT TYPE ARGUMENT");
    return;
  }
  switch (v1->type) {
  case VQUOTE:
    break;
  case VSTR:
    break;
  case VWORD:
    break;
  default:
    value_free(v1);
    break;
  }
}

void clear(value_t *v) {
  while (STACK->size != 0) {
    value_t *v1 = stack_pop(STACK);
    value_free(v1);
  }
}

void stemif(value_t *v) {
  value_t *v3 = stack_pop(STACK);
  if (v3 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }
  value_t *v2 = stack_pop(STACK);
  if (v2 == NULL) {
    stack_push(STACK, v3);
    eval_error("EMPTY STACK");
    return;
  }

  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    stack_push(STACK, v2);
    stack_push(STACK, v3);
    eval_error("EMPTY STACK");
    return;
  }

  if (v1->type != VINT) {
    stack_push(STACK, v1);
    stack_push(STACK, v2);
    stack_push(STACK, v3);
    eval_error("INCORRECT TYPE ARGUMENT");
    return;
  }

  if (v1->int_float) {
    value_free(v3);
    value_free(v1);
    if (v2->type == VQUOTE) {

      stack_push(EVAL_STACK, v2);
      for (int i = 0; i < v2->quote->size; i++) {
        eval(value_copy(v2->quote->items[i]));
      }
      value_t *vf = stack_pop(EVAL_STACK);
      if (vf) {
        value_free(vf);
      }
    } else {
      eval(v2);
    }
  } else {
    value_free(v2);
    value_free(v1);
    if (v3->type == VQUOTE) {
      stack_push(EVAL_STACK, v3);
      for (int i = 0; i < v3->quote->size; i++) {
        eval(value_copy(v3->quote->items[i]));
      }
      value_t *vf = stack_pop(EVAL_STACK);
      if (vf) {
        value_free(vf);
      }
    } else {
      eval(v3);
    }
  }
}

void gtequals(value_t *v) {
  value_t *v2 = stack_pop(STACK);
  if (v2 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    stack_push(STACK, v2);
    eval_error("EMPTY STACK");
    return;
  }

  value_t *retval = init_value(VINT);
  if (v1->type == VSTR && v2->type == VSTR ||
      v1->type == VWORD && v2->type == VWORD) {
    retval->int_float = strcmp(v1->str_word->value, v2->str_word->value) <= 0;
  } else if ((v1->type == VINT || v1->type == VFLOAT) &&
             (v2->type == VINT || v2->type == VFLOAT)) {
    retval->int_float = v1->int_float >= v2->int_float;
  } else {
    stack_push(STACK, v1);
    stack_push(STACK, v2);
    eval_error("INCORRECT TYPE ARGUMENT");
    return;
  }
  stack_push(STACK, retval);
  value_free(v1);
  value_free(v2);
}

void ltequals(value_t *v) {
  value_t *v2 = stack_pop(STACK);
  if (v2 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    stack_push(STACK, v2);
    eval_error("EMPTY STACK");
    return;
  }

  value_t *retval = init_value(VINT);
  if (v1->type == VSTR && v2->type == VSTR ||
      v1->type == VWORD && v2->type == VWORD) {
    retval->int_float = strcmp(v1->str_word->value, v2->str_word->value) <= 0;
  } else if ((v1->type == VINT || v1->type == VFLOAT) &&
             (v2->type == VINT || v2->type == VFLOAT)) {
    retval->int_float = v1->int_float <= v2->int_float;
  } else {
    stack_push(STACK, v1);
    stack_push(STACK, v2);
    eval_error("INCORRECT TYPE ARGUMENT");
    return;
  }
  stack_push(STACK, retval);
  value_free(v1);
  value_free(v2);
}

void clib(value_t *v) {
  value_t *v1 = stack_pop(STACK);
  void *handle = dlopen(v1->str_word->value, RTLD_LAZY);
  if (!handle) {
    stack_push(STACK, v1);
    eval_error("NOLIB");
    return;
  }
  dlerror();
  void (*af)(void);
  void (*aobjs)(void);
  char *error;
  *(void **)(&af) = dlsym(handle, "add_funcs");
  *(void **)(&aobjs) = dlsym(handle, "add_objs");
  if ((error = dlerror()) != NULL) {
    value_free(v1);
    fprintf(stderr, "%s\n", error);
    eval_error("DLSYM CANNOT FIND FUNCTION");
    return;
  } else {
    (*af)();
    (*aobjs)();
    dlclose(handle);
    value_free(v1);
  }
}

void gthan(value_t *v) {
  value_t *v2 = stack_pop(STACK);
  if (v2 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    stack_push(STACK, v2);
    eval_error("EMPTY STACK");
    return;
  }

  value_t *retval = init_value(VINT);
  if (v1->type == VSTR && v2->type == VSTR ||
      v1->type == VWORD && v2->type == VWORD) {
    retval->int_float = strcmp(v1->str_word->value, v2->str_word->value) > 0;
  } else if ((v1->type == VINT || v1->type == VFLOAT) &&
             (v2->type == VINT || v2->type == VFLOAT)) {
    retval->int_float = v1->int_float > v2->int_float;
  } else {
    stack_push(STACK, v1);
    stack_push(STACK, v2);
    eval_error("INCORRECT TYPE ARGUMENT");
    return;
  }
  stack_push(STACK, retval);
  value_free(v1);
  value_free(v2);
}

void lthan(value_t *v) {
  value_t *v2 = stack_pop(STACK);
  if (v2 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    stack_push(STACK, v2);
    eval_error("EMPTY STACK");
    return;
  }

  value_t *retval = init_value(VINT);
  if (v1->type == VSTR && v2->type == VSTR ||
      v1->type == VWORD && v2->type == VWORD) {
    retval->int_float = strcmp(v1->str_word->value, v2->str_word->value) < 0;
  } else if ((v1->type == VINT || v1->type == VFLOAT) &&
             (v2->type == VINT || v2->type == VFLOAT)) {
    retval->int_float = v1->int_float < v2->int_float;
  } else {
    stack_push(STACK, v1);
    stack_push(STACK, v2);
    eval_error("INCORRECT TYPE ARGUMENT");
    return;
  }
  stack_push(STACK, retval);
  value_free(v1);
  value_free(v2);
}

void equals(value_t *v) {
  value_t *v2 = stack_pop(STACK);
  if (v2 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    stack_push(STACK, v2);
    eval_error("EMPTY STACK");
    return;
  }

  value_t *retval = init_value(VINT);
  if (v1->type == VSTR && v2->type == VSTR ||
      v1->type == VWORD && v2->type == VWORD) {
    retval->int_float = strcmp(v1->str_word->value, v2->str_word->value) == 0;
  } else if ((v1->type == VINT || v1->type == VFLOAT) &&
             (v2->type == VINT || v2->type == VFLOAT)) {
    retval->int_float = v1->int_float == v2->int_float;
  } else {
    stack_push(STACK, v1);
    stack_push(STACK, v2);
    eval_error("INCORRECT TYPE ARGUMENT");
    return;
  }
  stack_push(STACK, retval);
  value_free(v1);
  value_free(v2);
}

void nequals(value_t *v) {
  value_t *v2 = stack_pop(STACK);
  if (v2 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    stack_push(STACK, v2);
    eval_error("EMPTY STACK");
    return;
  }

  value_t *retval = init_value(VINT);
  if (v1->type == VSTR && v2->type == VSTR ||
      v1->type == VWORD && v2->type == VWORD) {
    retval->int_float = strcmp(v1->str_word->value, v2->str_word->value) != 0;
  } else if ((v1->type == VINT || v1->type == VFLOAT) &&
             (v2->type == VINT || v2->type == VFLOAT)) {
    retval->int_float = v1->int_float != v2->int_float;
  } else {
    stack_push(STACK, v1);
    stack_push(STACK, v2);
    eval_error("INCORRECT TYPE ARGUMENT");
    return;
  }
  stack_push(STACK, retval);
  value_free(v1);
  value_free(v2);
}

void compose(value_t *v) {
  value_t *v2 = stack_pop(STACK);
  if (v2 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    stack_push(STACK, v2);
    eval_error("EMPTY STACK");
    return;
  }

  value_t *retval;
  if (v2->type == VSTR && v1->type == VSTR) {
    retval = init_value(VSTR);
    string_concat(v1->str_word, v2->str_word);
    retval->str_word = string_copy(v1->str_word);
    value_free(v1);
    value_free(v2);
  } else if (v2->type == VWORD && v1->type == VWORD) {
    retval = init_value(VWORD);
    string_concat(v1->str_word, v2->str_word);
    retval->str_word = string_copy(v1->str_word);
    value_free(v1);
    value_free(v2);
  } else if (v2->type == VQUOTE && v1->type == VQUOTE) {
    retval = v1;
    stack_extend(v1->quote, v2->quote);
    free(v2->quote->items);
    free(v2->quote);
    free(v2);
  } else {
    stack_push(STACK, v1);
    stack_push(STACK, v2);
    eval_error("INCORRECT TYPE ARGUMENT");
    return;
  }
  stack_push(STACK, retval);
}

void isnum(value_t *v) {
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }

  value_t *retval = init_value(VINT);

  bool isnum = true;
  for (int i = 0; i < v1->str_word->length; i++) {
    if (isspace(v1->str_word->value[i])) {
      continue;
    } else if (!isdigit(v1->str_word->value[i])) {
      isnum = false;
      break;
    }
  }
  retval->int_float = isnum;

  stack_push(STACK, v1);
  stack_push(STACK, retval);
}

void stoi(value_t *v) {
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }

  value_t *retval = init_value(VINT);
  retval->int_float = atoi(v1->str_word->value);
  stack_push(STACK, retval);
  value_free(v1);
}

void ssize(value_t *v) {
  value_t *retval = init_value(VINT);
  retval->int_float = STACK->size;
  stack_push(STACK, retval);
}

void qstack(value_t *v) {
  value_t *retval = init_value(VQUOTE);
  retval->quote = stack_copy(STACK);
  stack_push(STACK, retval);
}

void vat(value_t *v) {
  value_t *retval;
  value_t *v2 = stack_pop(STACK);
  if (v2 == NULL) {
    eval_error("EMTPY STACK");
    return;
  }
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    stack_push(STACK, v2);
    eval_error("EMTPY STACK");
    return;
  }

  if (v1->type != VINT) {
    stack_push(STACK, v1);
    stack_push(STACK, v2);
    eval_error("INCORRECT TYPE ARGUMENT");
    return;
  }
  if (v2->type == VQUOTE) {
    if (v2->quote->size <= v1->int_float || v1->int_float < 0) {
      stack_push(STACK, v1);
      stack_push(STACK, v2);
      eval_error("INDEX ERROR");
      return;
    }
    stack_push(STACK, v2);
    stack_push(STACK, value_copy(v2->quote->items[(int)v1->int_float]));
    value_free(v1);
  } else if (v2->type == VSTR || v2->type == VWORD) {
    if (v2->str_word->length <= v1->int_float || v1->int_float < 0) {
      stack_push(STACK, v1);
      stack_push(STACK, v2);
      eval_error("INDEX ERROR");
      return;
    }
    char *a = (char[]){v2->str_word->value[(int)v1->int_float], '\0'};
    string_t *s = init_string(a);
    if (v2->type == VWORD) {
      retval = init_value(VWORD);
    } else {
      retval = init_value(VSTR);
    }
    retval->str_word = s;
    stack_push(STACK, v2);
    stack_push(STACK, retval);
    value_free(v1);
  } else {
    stack_push(STACK, v1);
    stack_push(STACK, v2);
    eval_error("INCORRECT TYPE ARGUMENT");
    return;
  }
}

void stemfwrite(value_t *v) {
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }
  if (v1->type != VSTR) {
    stack_push(STACK, v1);
    eval_error("EMPTY STACK");
    return;
  }
  char *val;
  size_t len;
  FILE *fp = fopen(v1->str_word->value, "w+");
  if (!fp) {
    stack_push(STACK, v1);
    eval_error("FOPEN ERROR");
    return;
  }
  fprintf(fp, "%s", v1->str_word->value);
  value_free(v1);
  fclose(fp);
}

void stemsleep(value_t *v) {
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }
  if (v1->type != VINT && v1->type != VFLOAT) {
    stack_push(STACK, v1);
    eval_error("INCORRECT TYPE ARGUMENT");
    return;
  }

  sleep(v1->int_float);
  value_free(v1);
}

void stemcut(value_t *v) {
  value_t *v1 = stack_pop(STACK);
  value_t *r1;
  value_t *r2;
  if (v1 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }
  value_t *v2 = stack_pop(STACK);
  if (v2 == NULL) {
    stack_push(STACK, v1);
    eval_error("EMPTY STACK");
    return;
  }
  if (v1->type != VINT) {
    stack_push(STACK, v1);
    stack_push(STACK, v2);
    eval_error("INCORRECT TYPE ARGUMENT");
    return;
  }
  switch (v2->type) {
  case VWORD:
  case VSTR:
    if (v1->int_float >= v2->str_word->length || v1->int_float < 0) {
      stack_push(STACK, v1);
      stack_push(STACK, v2);
      eval_error("INDEX ERROR");
      return;
    }
    if (v2->type == VSTR) {
      r1 = init_value(VSTR);
      r2 = init_value(VSTR);
    } else {
      r1 = init_value(VWORD);
      r2 = init_value(VWORD);
    }
    r1->str_word = init_string("");
    r2->str_word = init_string("");
    for (int i = 0; i < v1->int_float; i++) {
      string_append(r1->str_word, v2->str_word->value[i]);
    }
    for (int i = v1->int_float; i < v2->str_word->length; i++) {
      string_append(r2->str_word, v2->str_word->value[i]);
    }
    value_free(v2);
    break;
  case VQUOTE:
    if (v1->int_float >= v2->quote->size || v1->int_float < 0) {
      stack_push(STACK, v1);
      stack_push(STACK, v2);
      eval_error("INDEX ERROR");
      return;
    }
    r1 = init_value(VQUOTE);
    r1->quote = init_stack(10);
    r2 = init_value(VQUOTE);
    r2->quote = init_stack(10);
    for (int i = 0; i < v1->int_float + 1; i++) {
      stack_push(r1->quote, v2->quote->items[i]);
    }
    for (int i = v1->int_float + 1; i < v2->quote->size; i++) {
      stack_push(r2->quote, v2->quote->items[i]);
    }
    /* [ a b c ] 1 cut => [ a ] [ b c ] */
    free(v2->quote->items);
    free(v2->quote);
    free(v2);
    break;
  default:
    stack_push(STACK, v2);
    stack_push(STACK, v1);
    eval_error("INCORRECT TYPE ARGUMENT");
    return;
  }
  stack_push(STACK, r1);
  stack_push(STACK, r2);
  value_free(v1);
}

void undef(value_t *v) {
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }
  if (v1->type != VWORD) {
    stack_push(STACK, v1);
    eval_error("INCORRECT TYPE ARGUMENT");
    return;
  }
  ht_delete(WORD_TABLE, v1->str_word, value_free);
  ht_delete(FLIT, v1->str_word, func_free);
  value_free(v1);
}

void tostr(value_t *v) {
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }
  if (v1->type == VERR || v1->type == VWORD)
    v1->type = VSTR;
  else {
    stack_push(STACK, v1);
    eval_error("INVALID TYPE ARGUMENT");
  }
  stack_push(STACK, v1);
}

void include(value_t *v) {
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }
  if (v1->type != VSTR) {
    stack_push(STACK, v1);
    eval_error("EMPTY STACK");
    return;
  }
  char *val = "";
  size_t len = 0;
  string_t *strval = init_string("/usr/local/share/stem/stemlib/");
  string_concat(strval, v1->str_word);
  FILE *fp = fopen(strval->value, "rb");
  string_free(strval);
  if (!fp) {
    stack_push(STACK, v1);
    eval_error("FREAD ERROR");
    return;
  }
  ssize_t bytes_read = getdelim(&val, &len, '\0', fp);
  fclose(fp);

  value_t *retval = init_value(VQUOTE);
  retval->quote = init_stack(10);
  parser_t *p = parser_pp(val);
  value_t *cur;
  while (1) {
    cur = parser_get_next(p);
    if (cur == NULL)
      break;
    stack_push(retval->quote, cur);
  }
  stack_push(STACK, retval);
  value_free(v1);
  free(p->source);
  free(p);
}

void uncurry(value_t *v) {
  value_t *v1 = stack_pop(STACK);
  if (v1 == NULL) {
    eval_error("EMPTY STACK");
    return;
  }
  if (v1->type != VQUOTE) {
    stack_push(STACK, v1);
    eval_error("EMPTY STACK");
    return;
  }
  if (v1->quote->size <= 0) {
    stack_push(STACK, v1);
    eval_error("INDEX ERROR");
    return;
  }
  value_t *retval = v1->quote->items[0];
  for (int i = 1; i < v1->quote->size; i++) {
    v1->quote->items[i - 1] = v1->quote->items[i];
  }
  v1->quote->size--;
  stack_push(STACK, v1);
  stack_push(STACK, retval);
}

void add_objs() {}

void add_funcs() {
  add_func(FLIT, period, ".");
  add_func(FLIT, questionmark, "?");
  add_func(FLIT, stemadd, "+");
  add_func(FLIT, stemsub, "-");
  add_func(FLIT, stemdiv, "/");
  add_func(FLIT, stemmul, "*");
  add_func(FLIT, stempow, "pow");
  add_func(FLIT, stemsin, "sin");
  add_func(FLIT, stemcos, "cos");
  add_func(FLIT, stemexp, "exp");
  add_func(FLIT, strquote, "strquote");
  add_func(FLIT, stemeval, "eval");
  add_func(FLIT, stemfunc, "func");
  add_func(FLIT, stemfunc, "def");
  add_func(FLIT, nop, "nop");
  add_func(FLIT, stemln, "ln");
  add_func(FLIT, stemfloor, "floor");
  add_func(FLIT, stemceil, "ceil");
  add_func(FLIT, stemfread, "fread");
  add_func(FLIT, stemread, "read");
  add_func(FLIT, vat, "vat");
  add_func(FLIT, qstack, "qstack");
  add_func(FLIT, ssize, "ssize");
  add_func(FLIT, stoi, "stoi");
  add_func(FLIT, isnum, "isnum");
  add_func(FLIT, compose, "compose");
  add_func(FLIT, lthan, "<");
  add_func(FLIT, ltequals, "<=");
  add_func(FLIT, gthan, ">");
  add_func(FLIT, gtequals, ">=");
  add_func(FLIT, nequals, "!=");
  add_func(FLIT, equals, "=");
  add_func(FLIT, stemif, "if");
  add_func(FLIT, clear, "clear");
  add_func(FLIT, dip, "dip");
  add_func(FLIT, stemlen, "len");
  add_func(FLIT, quote, "quote");
  add_func(FLIT, stemexit, "exit");
  add_func(FLIT, stemtype, "type");
  add_func(FLIT, stemdup, "dup");
  add_func(FLIT, curry, "curry");
  add_func(FLIT, swap, "swap");
  add_func(FLIT, isdef, "isdef");
  add_func(FLIT, dsc, "dsc");
  add_func(FLIT, dsc, "drop");
  add_func(FLIT, clib, "clib");
  add_func(FLIT, stemsleep, "sleep");
  add_func(FLIT, undef, "undef");
  add_func(FLIT, tostr, "tostr");
  add_func(FLIT, stemcut, "cut");
  add_func(FLIT, steminsert, "insert");
  add_func(FLIT, unglue, "unglue");
  add_func(FLIT, stemand, "and");
  add_func(FLIT, stemor, "or");
  add_func(FLIT, include, "include");
  add_func(FLIT, uncurry, "uncurry");
}
