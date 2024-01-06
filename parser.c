#include "parser.h"
#include "macros.h"
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50
#define JUSTDO(a)                                                              \
  if (!(a)) {                                                                  \
    perror(#a);                                                                \
    exit(1);                                                                   \
  }

array_t *STACK;
ht_t *WORD_TABLE;
char *INBUF;
parser_t *PARSER;

array_t *init_array(size_t size) {
  array_t *a = calloc(1, sizeof(array_t));
  a->size = 0;
  a->capacity = size;
  a->items = calloc(a->capacity, sizeof(value_t *));
  return a;
}

void array_append(array_t *a, value_t *v) {
  if (a->size >= a->capacity - 2) {
    a->capacity = a->capacity * 2;
    a->items = realloc(a->items, a->capacity * sizeof(value_t *));
  }
  a->items[a->size] = v;
  a->size++;
}

value_t *array_pop(array_t *a) {
  if (a->size > 0) {
    value_t *v = a->items[a->size - 1];
    a->size--;
    return v;
  }
  return NULL;
}

void array_extend(array_t *a, array_t *b) {
  for (int i = 0; i < b->size; i++) {
    array_append(a, b->items[i]);
  }
}

void array_free(array_t *a) {
  for (int i = 0; i < a->size; i++) {
    value_free(a->items[i]);
  }
  free(a->items);
  free(a);
}

array_t *array_copy(array_t *a) {
  array_t *b = calloc(1, sizeof(array_t));
  b->size = a->size;
  b->capacity = a->capacity;
  b->items = calloc(b->capacity, sizeof(value_t *));
  for (int i = 0; i < a->size; i++) {
    b->items[i] = value_copy(a->items[i]);
  }
  return b;
}

value_t *init_value(int type) {
  value_t *v = calloc(1, sizeof(value_t));
  v->type = type;
  v->escaped = false;
  return v;
}

value_t *value_copy(value_t *v) {
  value_t *a = init_value(VINT);
  a->type = v->type;
  if (v->type == VINT || v->type == VFLOAT) {
    a->int_float = v->int_float;
  } else if (v->type == VSTR || v->type == VWORD) {
    a->str_word = string_copy(v->str_word);
  } else if (v->type == VQUOTE) {
    a->quote = array_copy(v->quote);
  }
  return a;
}

void value_free(value_t *v) {
  if (v->type == VSTR || v->type == VWORD) {
    string_free(v->str_word);
  }
  if (v->type == VQUOTE) {
    array_free(v->quote);
  }
  free(v);
}

parser_t *init_parser(char *source) {
  parser_t *p = calloc(1, sizeof(parser_t));
  p->i = 0;
  p->source = source;
  p->c = source[0];
  return p;
}

void parser_reset(parser_t *p, char *source) {
  p->source = source;
  p->i = 0;
  p->c = source[0];
}

void parser_move(parser_t *p) {
  if (p->i < strlen(p->source) && p->c != '\0') {
    p->i++;
    p->c = p->source[p->i];
  }
}

void parser_skip_whitespace(parser_t *p) {
  while (isspace(p->c)) {
    parser_move(p);
  }
}

value_t *parse_string(parser_t *p) {
  value_t *retv = init_value(VSTR);
  parser_move(p);
  string_t *s = init_string(NULL);
  while (p->c != '"' && p->c != '\0') {
    string_append(s, p->c);
    parser_move(p);
  }
  parser_move(p);
  retv->str_word = s;
  return retv;
}

value_t *parse_quote(parser_t *p) {
  value_t *retv = init_value(VQUOTE);
  retv->quote = init_array(10);
  parser_move(p);
  parser_skip_whitespace(p);
  while (p->c != ']') {
    array_append(retv->quote, parser_get_next(p));
    parser_skip_whitespace(p);
  }
  parser_move(p);
  return retv;
}

void parser_error(parser_t *p) { exit(1); }

value_t *parse_word(parser_t *p) {
  value_t *retv = init_value(VWORD);
  string_t *s = init_string(NULL);
  if (p->c == '\\') {
    retv->escaped = true;
    parser_move(p);
    if (isspace(p->c) || p->c == '\0') {
      parser_error(p);
    }
  }
  while (!isspace(p->c) && p->c != '\0') {
    string_append(s, p->c);
    parser_move(p);
  }
  retv->str_word = s;
  return retv;
}

value_t *parse_num(parser_t *p) {
  value_t *retv;
  string_t *s = init_string(NULL);
  bool is_float = false;
  while (isdigit(p->c) || (p->c == '.') && !is_float) {
    if (p->c == '.')
      is_float = true;
    string_append(s, p->c);
    parser_move(p);
  }
  if (is_float)
    retv = init_value(VFLOAT);
  else
    retv = init_value(VINT);
  retv->int_float = atof(s->value);
  string_free(s);
  return retv;
}

value_t *parser_get_next(parser_t *p) {
  parser_skip_whitespace(p);
  if (isdigit(p->c)) {
    return parse_num(p);
  }
  switch (p->c) {
  case '"':
    return parse_string(p);
  case '[':
    return parse_quote(p);
  case '\0':
    return NULL;
  default:
    return parse_word(p);
  }
}

node_t *init_node(string_t *key, value_t *value) {
  node_t *n = calloc(1, sizeof(node_t));
  n->key = key;
  n->value = value;
  n->next = NULL;
  return n;
}

void node_free(node_t *n) {
  string_free(n->key);
  value_free(n->value);
  free(n);
}

sll_t *init_sll() {
  sll_t *l = calloc(1, sizeof(sll_t));
  l->size = 0;
  l->head = NULL;
  return l;
}

void sll_add(sll_t *l, string_t *s, value_t *v) {
  if (l->head == NULL) {
    node_t *n = init_node(s, v);
    l->head = n;
    l->size++;
    return;
  }
  node_t *cur = l->head;
  while (cur->next != NULL) {
    if (strcmp(s->value, cur->value->str_word->value) == 0) {
      value_free(cur->value);
      string_free(s);
      cur->value = v;
      return;
    }
    cur = cur->next;
  }
  if (strcmp(s->value, cur->value->str_word->value) == 0) {
    value_free(cur->value);
    string_free(s);
    cur->value = v;
    return;
  }
  node_t *n = init_node(s, v);
  cur->next = n;
}

value_t *sll_get(sll_t *l, string_t *k) {
  if (l->head == NULL)
    return NULL;
  node_t *cur = l->head;
  while (cur != NULL) {
    if (strcmp(k->value, cur->key->value) == 0)
      return cur->value;
    cur = cur->next;
  }
  return NULL;
}

void sll_free(sll_t *l) {
  node_t *cur = l->head;
  node_t *tmp;
  while (cur != NULL) {
    tmp = cur;
    cur = cur->next;
    node_free(tmp);
  }
  free(l);
}

ht_t *init_ht(size_t size) {
  ht_t *h = calloc(1, sizeof(ht_t));
  h->size = size;
  h->buckets = calloc(h->size, sizeof(sll_t *));
  for (int i = 0; i < size; i++) {
    h->buckets[i] = init_sll();
  }
  return h;
}

void ht_add(ht_t *h, string_t *key, value_t *v) {
  sll_add(h->buckets[hash(h, key->value)], key, v);
}

value_t *ht_get(ht_t *h, string_t *key) {
  return sll_get(h->buckets[hash(h, key->value)], key);
}

void ht_free(ht_t *h) {
  for (int i = 0; i < h->size; i++) {
    sll_free(h->buckets[i]);
  }
  free(h->buckets);
  free(h);
}

/* DJB2 HASH FUNCTION */
unsigned long hash(ht_t *h, char *key) {
  unsigned long hash = 5381;
  int c;

  while (c = *key++)
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return hash % h->size;
}

void print_value(value_t *v) {
  switch (v->type) {
  case VINT:
  case VFLOAT:
    printf("%Lf\n", v->int_float);
    break;
  case VSTR:
    printf("%s\n", v->str_word->value);
    break;
  case VWORD:
    printf("W: %s\n", v->str_word->value);
    break;
  case VQUOTE:
    printf("Q:\n");
    for (int i = 0; i < v->quote->size; i++) {
      print_value(v->quote->items[i]);
    }
    break;
  case VERR:
    printf("STACK ERR\n");
    break;
  }
}

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
  if (e)
    *e = '\0';
  return ret;
}

bool eval_error() {
  value_t *v = init_value(VERR);
  array_append(STACK, v);
  return true;
}

bool eval_builtins(value_t *v) {
  char *str = v->str_word->value;
  value_t *v1;
  value_t *v2;
  value_t *v3;
  value_t *retval;
  if (strcmp(str, "func") == 0) {
    v2 = array_pop(STACK);
    if (v2 == NULL) {
      value_free(v);
      return eval_error();
    }
    v1 = array_pop(STACK);
    if (v1 == NULL) {
      value_free(v);
      array_append(STACK, v2);
      return eval_error();
    }
    if (v1->type != VWORD) {
      value_free(v);
      return eval_error();
    }
    ht_add(WORD_TABLE, string_copy(v1->str_word), v2);
    value_free(v1);
  } else if (strcmp(str, "+") == 0) {
    v2 = array_pop(STACK);
    if (v2 == NULL) {
      value_free(v);
      return eval_error();
    }
    v1 = array_pop(STACK);
    if (v1 == NULL) {
      value_free(v);
      array_append(STACK, v2);
      return eval_error();
    }
    retval = init_value(VFLOAT);
    if (v1->type == VINT && v2->type == VINT) {
      retval->type = VINT;
    }
    if (v1->type != VINT && v1->type != VFLOAT ||
        v2->type != VINT && v2->type != VFLOAT) {
      array_append(STACK, v1);
      array_append(STACK, v2);
      value_free(v);
      return eval_error();
    }
    retval->int_float = v1->int_float + v2->int_float;
    array_append(STACK, retval);
    value_free(v1);
    value_free(v2);
  } else if (strcmp(str, "-") == 0) {
    v2 = array_pop(STACK);
    if (v2 == NULL) {
      value_free(v);
      return eval_error();
    }
    v1 = array_pop(STACK);
    if (v1 == NULL) {
      value_free(v);
      array_append(STACK, v2);
      return eval_error();
    }
    retval = init_value(VFLOAT);
    if (v1->type == VINT && v2->type == VINT) {
      retval->type = VINT;
    }
    retval->int_float = v1->int_float - v2->int_float;
    array_append(STACK, retval);
    value_free(v1);
    value_free(v2);
  } else if (strcmp(str, "/") == 0) {
    v2 = array_pop(STACK);
    if (v2 == NULL) {
      value_free(v);
      return eval_error();
    }
    v1 = array_pop(STACK);
    if (v1 == NULL) {
      value_free(v);
      array_append(STACK, v2);
      return eval_error();
    }
    retval = init_value(VFLOAT);
    if (v1->type == VINT && v2->type == VINT) {
      retval->type = VINT;
    }
    retval->int_float = v1->int_float / v2->int_float;
    array_append(STACK, retval);
    value_free(v1);
    value_free(v2);
  } else if (strcmp(str, "*") == 0) {
    v2 = array_pop(STACK);
    if (v2 == NULL) {
      value_free(v);
      return eval_error();
    }
    v1 = array_pop(STACK);
    if (v1 == NULL) {
      value_free(v);
      array_append(STACK, v2);
      return eval_error();
    }
    retval = init_value(VFLOAT);
    if (v1->type == VINT && v2->type == VINT) {
      retval->type = VINT;
    }
    retval->int_float = v1->int_float * v2->int_float;
    array_append(STACK, retval);
    value_free(v1);
    value_free(v2);
  } else if (strcmp(str, "pow") == 0) {
    v2 = array_pop(STACK);
    if (v2 == NULL) {
      value_free(v);
      return eval_error();
    }
    v1 = array_pop(STACK);
    if (v1 == NULL) {
      value_free(v);
      array_append(STACK, v2);
      return eval_error();
    }
    retval = init_value(VFLOAT);
    if (v1->type == VINT && v2->type == VINT) {
      retval->type = VINT;
    }
    retval->int_float = powl(v1->int_float, v2->int_float);
    array_append(STACK, retval);
    value_free(v1);
    value_free(v2);
  } else if (strcmp(str, "eval") == 0) {
    v1 = array_pop(STACK);
    if (v1 == NULL) {
      value_free(v);
      array_append(STACK, v1);
      return eval_error();
    }
    if (v1->type == VQUOTE) {
      for (int i = 0; i < v1->quote->size; i++) {
        eval(value_copy(v1->quote->items[i]));
      }
      value_free(v1);
    } else {
      eval(v1);
    }
  } else if (strcmp(str, "evalstr") == 0) {
    v1 = array_pop(STACK);
    if (v1 == NULL) {
      value_free(v);
      return eval_error();
    }
    if (v1->type != VSTR) {
      array_append(STACK, v1);
      value_free(v);
      return eval_error();
    }
    parser_t *tmp_p = init_parser(v1->str_word->value);
    value_t *cur;
    while (1) {
      cur = parser_get_next(tmp_p);
      if (cur == NULL)
        break;
      eval(cur);
    }
    free(tmp_p);
    value_free(v1);

  } else if (strcmp(str, ".") == 0) {
    v1 = array_pop(STACK);
    if (v1 == NULL) {
      value_free(v);
      return eval_error();
    }
    print_value(v1);
    value_free(v1);
  } else if (strcmp(str, "qstack") == 0) {
    retval = init_value(VQUOTE);
    retval->quote = array_copy(STACK);
    array_append(STACK, retval);
  } else if (strcmp(str, "?") == 0) {
    for (int i = 0; i < STACK->size; i++) {
      print_value(STACK->items[i]);
    }
  } else if (strcmp(str, "sin") == 0) {
    v1 = array_pop(STACK);
    if (v1 == NULL) {
      value_free(v);
      return eval_error();
    }
    retval = init_value(VFLOAT);
    retval->int_float = sinhl(v1->int_float);
    array_append(STACK, retval);
    value_free(v1);
  } else if (strcmp(str, "cos") == 0) {
    v1 = array_pop(STACK);
    if (v1 == NULL) {
      value_free(v);
      return eval_error();
    }
    retval = init_value(VFLOAT);
    retval->int_float = coshl(v1->int_float);
    array_append(STACK, retval);
    value_free(v1);
  } else if (strcmp(str, "exp") == 0) {
    v1 = array_pop(STACK);
    if (v1 == NULL) {
      value_free(v);
      return eval_error();
    }
    retval = init_value(VFLOAT);
    retval->int_float = expl(v1->int_float);
    array_append(STACK, retval);
    value_free(v1);
  } else if (strcmp(str, "floor") == 0) {
    v1 = array_pop(STACK);
    if (v1 == NULL) {
      value_free(v);
      return eval_error();
    }
    retval = init_value(VFLOAT);
    retval->int_float = floor(v1->int_float);
    array_append(STACK, retval);
    value_free(v1);
  } else if (strcmp(str, "ceil") == 0) {
    v1 = array_pop(STACK);
    if (v1 == NULL) {
      value_free(v);
      return eval_error();
    }
    retval = init_value(VFLOAT);
    retval->int_float = ceil(v1->int_float);
    array_append(STACK, retval);
    value_free(v1);
  } else if (strcmp(str, "ln") == 0) {
    v1 = array_pop(STACK);
    if (v1 == NULL) {
      value_free(v);
      return eval_error();
    }
    retval = init_value(VFLOAT);
    retval->int_float = logl(v1->int_float);
    array_append(STACK, retval);
    value_free(v1);
  } else if (strcmp(str, "ssize") == 0) {
    retval = init_value(VINT);
    retval->int_float = STACK->size;
    array_append(STACK, retval);
  } else if (strcmp(str, "compose") == 0) {
    v2 = array_pop(STACK);
    if (v2 == NULL) {
      value_free(v);
      return eval_error();
    }
    v1 = array_pop(STACK);
    if (v1 == NULL) {
      value_free(v);
      array_append(STACK, v2);
      return eval_error();
    }

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
      array_extend(v1->quote, v2->quote);
      free(v2->quote);
      free(v2);
    } else {
      value_free(v);
      array_append(STACK, v1);
      array_append(STACK, v2);
      return eval_error();
    }
    array_append(STACK, retval);
  } else if (strcmp(str, "wtostr") == 0) {
    v1 = array_pop(STACK);
    if (v1 == NULL) {
      value_free(v);
      return eval_error();
    }

    if (v1->type != VWORD) {
      value_free(v);
      array_append(STACK, v1);
      return eval_error();
    }
    v1->type = VSTR;
    array_append(STACK, v1);
  } else if (strcmp(str, "=") == 0) {
    v2 = array_pop(STACK);
    if (v2 == NULL) {
      value_free(v);
      return eval_error();
    }
    v1 = array_pop(STACK);
    if (v1 == NULL) {
      value_free(v);
      array_append(STACK, v2);
      return eval_error();
    }

    retval = init_value(VINT);
    if (v1->type == VSTR && v2->type == VSTR ||
        v1->type == VWORD && v2->type == VWORD) {
      retval->int_float = strcmp(v1->str_word->value, v2->str_word->value) == 0;
    } else if ((v1->type == VINT || v1->type == VFLOAT) &&
               (v2->type == VINT || v2->type == VFLOAT)) {
      retval->int_float = v1->int_float == v2->int_float;
    } else {
      value_free(v);
      array_append(STACK, v1);
      array_append(STACK, v2);
      return eval_error();
    }
    array_append(STACK, retval);
    value_free(v1);
    value_free(v2);
  } else if (strcmp(str, "!=") == 0) {
    v2 = array_pop(STACK);
    if (v2 == NULL) {
      value_free(v);
      return eval_error();
    }
    v1 = array_pop(STACK);
    if (v1 == NULL) {
      value_free(v);
      array_append(STACK, v2);
      return eval_error();
    }

    retval = init_value(VINT);
    if (v1->type == VSTR && v2->type == VSTR ||
        v1->type == VWORD && v2->type == VWORD) {
      retval->int_float = strcmp(v1->str_word->value, v2->str_word->value) != 0;
    } else if ((v1->type == VINT || v1->type == VFLOAT) &&
               (v2->type == VINT || v2->type == VFLOAT)) {
      retval->int_float = v1->int_float != v2->int_float;
    } else {
      value_free(v);
      array_append(STACK, v1);
      array_append(STACK, v2);
      return eval_error();
    }
    array_append(STACK, retval);
    value_free(v1);
    value_free(v2);
  } else if (strcmp(str, "<=") == 0) {
    v2 = array_pop(STACK);
    if (v2 == NULL) {
      value_free(v);
      return eval_error();
    }
    v1 = array_pop(STACK);
    if (v1 == NULL) {
      value_free(v);
      array_append(STACK, v2);
      return eval_error();
    }

    retval = init_value(VINT);
    if (v1->type == VSTR && v2->type == VSTR ||
        v1->type == VWORD && v2->type == VWORD) {
      retval->int_float = strcmp(v1->str_word->value, v2->str_word->value) <= 0;
    } else if ((v1->type == VINT || v1->type == VFLOAT) &&
               (v2->type == VINT || v2->type == VFLOAT)) {
      retval->int_float = v1->int_float <= v2->int_float;
    } else {
      value_free(v);
      array_append(STACK, v1);
      array_append(STACK, v2);
      return eval_error();
    }
    array_append(STACK, retval);
    value_free(v1);
    value_free(v2);
  } else if (strcmp(str, "<") == 0) {
    v2 = array_pop(STACK);
    if (v2 == NULL) {
      value_free(v);
      return eval_error();
    }
    v1 = array_pop(STACK);
    if (v1 == NULL) {
      value_free(v);
      array_append(STACK, v2);
      return eval_error();
    }

    retval = init_value(VINT);
    if (v1->type == VSTR && v2->type == VSTR ||
        v1->type == VWORD && v2->type == VWORD) {
      retval->int_float = strcmp(v1->str_word->value, v2->str_word->value) < 0;
    } else if ((v1->type == VINT || v1->type == VFLOAT) &&
               (v2->type == VINT || v2->type == VFLOAT)) {
      retval->int_float = v1->int_float < v2->int_float;
    } else {
      value_free(v);
      array_append(STACK, v1);
      array_append(STACK, v2);
      return eval_error();
    }
    array_append(STACK, retval);
    value_free(v1);
    value_free(v2);
  } else if (strcmp(str, ">") == 0) {
    v2 = array_pop(STACK);
    if (v2 == NULL) {
      value_free(v);
      return eval_error();
    }
    v1 = array_pop(STACK);
    if (v1 == NULL) {
      value_free(v);
      array_append(STACK, v2);
      return eval_error();
    }

    retval = init_value(VINT);
    if (v1->type == VSTR && v2->type == VSTR ||
        v1->type == VWORD && v2->type == VWORD) {
      retval->int_float = strcmp(v1->str_word->value, v2->str_word->value) > 0;
    } else if ((v1->type == VINT || v1->type == VFLOAT) &&
               (v2->type == VINT || v2->type == VFLOAT)) {
      retval->int_float = v1->int_float > v2->int_float;
    } else {
      value_free(v);
      array_append(STACK, v1);
      array_append(STACK, v2);
      return eval_error();
    }
    array_append(STACK, retval);
    value_free(v1);
    value_free(v2);
  } else if (strcmp(str, ">=") == 0) {
    v2 = array_pop(STACK);
    if (v2 == NULL) {
      value_free(v);
      return eval_error();
    }
    v1 = array_pop(STACK);
    if (v1 == NULL) {
      value_free(v);
      array_append(STACK, v2);
      return eval_error();
    }

    retval = init_value(VINT);
    if (v1->type == VSTR && v2->type == VSTR ||
        v1->type == VWORD && v2->type == VWORD) {
      retval->int_float = strcmp(v1->str_word->value, v2->str_word->value) >= 0;
    } else if ((v1->type == VINT || v1->type == VFLOAT) &&
               (v2->type == VINT || v2->type == VFLOAT)) {
      retval->int_float = v1->int_float >= v2->int_float;
    } else {
      value_free(v);
      array_append(STACK, v1);
      array_append(STACK, v2);
      return eval_error();
    }
    array_append(STACK, retval);
    value_free(v1);
    value_free(v2);
  } else if (strcmp(str, "if") == 0) {
    v3 = array_pop(STACK);
    if (v3 == NULL) {
      value_free(v);
      return eval_error();
    }
    v2 = array_pop(STACK);
    if (v2 == NULL) {
      value_free(v);
      array_append(STACK, v3);
      return eval_error();
    }
    v1 = array_pop(STACK);
    if (v1 == NULL) {
      value_free(v);
      array_append(STACK, v2);
      array_append(STACK, v3);
      return eval_error();
    }

    if (v3->type != VINT) {
      value_free(v);
      return eval_error();
    }

    if (v3->int_float) {
      if (v1->type == VQUOTE) {
        for (int i = 0; i < v1->quote->size; i++) {
          eval(value_copy(v1->quote->items[i]));
        }
        value_free(v1);
      } else {
        eval(v1);
      }
      value_free(v2);
    } else {
      if (v2->type == VQUOTE) {
        for (int i = 0; i < v2->quote->size; i++) {
          eval(value_copy(v2->quote->items[i]));
        }
        value_free(v2);
      } else {
        eval(v2);
      }
      value_free(v1);
    }
    value_free(v3);
  } else if (strcmp(str, "clear") == 0) {
    for (int i = 0; i < STACK->size; i++) {
      value_free(array_pop(STACK));
    }
  } else if (strcmp(str, "curry") == 0) {
    v2 = array_pop(STACK);
    if (v2 == NULL) {
      value_free(v);
      return eval_error();
    }
    v1 = array_pop(STACK);
    if (v1 == NULL) {
      value_free(v);
      array_append(STACK, v2);
      return eval_error();
    }

    if (v2->type != VQUOTE) {
      value_free(v);
      array_append(STACK, v1);
      array_append(STACK, v2);
      return eval_error();
    }

    array_append(v2->quote, v1);
    array_append(STACK, v2);
  } else if (strcmp(str, "len") == 0) {
    v1 = array_pop(STACK);
    if (v1 == NULL) {
      value_free(v);
      return eval_error();
    }

    retval = init_value(VINT);
    if (v1->type == VINT || v1->type == VFLOAT) {
      retval->int_float = 1;
    } else if (v1->type == VSTR || v1->type == VWORD) {
      retval->int_float = strlen(v1->str_word->value);
    } else if (v1->type == VQUOTE) {
      retval->int_float = v1->quote->size;
    }
    array_append(STACK, v1);
    array_append(STACK, retval);
  } else if (strcmp(str, "dup") == 0) {
    v1 = array_pop(STACK);
    if (v1 == NULL) {
      value_free(v);
      return eval_error();
    }

    retval = value_copy(v1);
    array_append(STACK, v1);
    array_append(STACK, retval);
  } else if (strcmp(str, "discard") == 0) {
    v1 = array_pop(STACK);
    if (v1 == NULL) {
      value_free(v);
      return eval_error();
    }

    value_free(v1);
  } else if (strcmp(str, "type") == 0) {
    v1 = array_pop(STACK);
    if (v1 == NULL) {
      value_free(v);
      return eval_error();
    }

    retval = init_value(VINT);
    retval->int_float = v1->type;
    array_append(STACK, v1);
    array_append(STACK, retval);
  } else if (strcmp(str, "quote") == 0) {
    v1 = array_pop(STACK);
    if (v1 == NULL) {
      value_free(v);
      return eval_error();
    }

    retval = init_value(VQUOTE);
    retval->quote = init_array(10);
    array_append(retval->quote, v1);
    array_append(STACK, retval);
  } else if (strcmp(str, "exit") == 0) {
    ht_free(WORD_TABLE);
    array_free(STACK);
    free(INBUF);
    free(PARSER);
    value_free(v);
    exit(0);
  } else if (strcmp(str, "read") == 0) {
    v1 = array_pop(STACK);
    if (v1 == NULL) {
      value_free(v);
      return eval_error();
    }

    if (v1->type != VSTR) {
      value_free(v);
      array_append(STACK, v1);
      return eval_error();
    }
    printf("%s", v1->str_word->value);
    retval = init_value(VSTR);
    retval->str_word = init_string(get_line(stdin));
    array_append(STACK, retval);
    value_free(v1);
  } else if (strcmp(str, "vat") == 0) {
    v2 = array_pop(STACK);
    if (v2 == NULL) {
      value_free(v);
      return eval_error();
    }
    v1 = array_pop(STACK);
    if (v1 == NULL) {
      value_free(v);
      array_append(STACK, v2);
      return eval_error();
    }

    if (v1->type != VINT) {
      value_free(v);
      array_append(STACK, v1);
      array_append(STACK, v2);
      return eval_error();
    }
    if (v2->type == VINT || v2->type == VFLOAT) {
      value_free(v);
      array_append(STACK, v1);
      array_append(STACK, v2);
      return eval_error();
    }

    if (v2->type == VQUOTE) {
      if (v2->quote->size <= v1->int_float) {
        value_free(v);
        array_append(STACK, v1);
        array_append(STACK, v2);
        return eval_error();
      }
      array_append(STACK, v2);
      array_append(STACK, value_copy(v2->quote->items[(int)v1->int_float]));
      value_free(v1);
    } else {
      value_free(v);
      return eval_error();
    }
  } else {
    return false;
  }
  value_free(v);
  return true;
}

bool eval_ht(value_t *v) {
  value_t *func = ht_get(WORD_TABLE, v->str_word);
  if (func == NULL)
    return false;
  if (func->type == VQUOTE) {
    for (int i = 0; i < func->quote->size; i++) {
      eval(value_copy(func->quote->items[i]));
    }
  } else {
    eval(value_copy(func));
  }
  value_free(v);
  return true;
}

void eval(value_t *v) {
  switch (v->type) {
  case VINT:
  case VFLOAT:
  case VSTR:
  case VQUOTE:
  case VERR:
    array_append(STACK, v);
    break;
  case VWORD:
    if (v->escaped) {
      v->escaped = false;
      array_append(STACK, v);
    } else {
      if (!eval_builtins(v)) {
        if (!eval_ht(v)) {
          array_append(STACK, v);
        }
      }
    }
    break;
  }
}
