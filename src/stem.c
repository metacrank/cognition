#include <ctype.h>
#include <macros.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stem.h>
#include <string.h>

/* Global variables defined */
array_t *STACK;
array_t *EVAL_STACK;
ht_t *WORD_TABLE;
parser_t *PARSER;

ht_t *FLIT;
ht_t *OBJ_TABLE;

void func_free(void *f) {}

array_t *init_array(size_t size) {
  array_t *a = calloc(1, sizeof(array_t));
  if (!a)
    die("calloc on array");
  a->size = 0;
  a->capacity = size;
  a->items = calloc(a->capacity, sizeof(value_t *));
  if (!a->items)
    die("calloc on a->items");
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

void array_add(array_t *a, value_t *v, int index) {
  if (a->size >= a->capacity - 3) {
    a->capacity = a->capacity * 2;
    a->items = realloc(a->items, a->capacity * sizeof(value_t *));
  }
  for (int i = a->size - 1; i >= index; i--) {
    a->items[i + 1] = a->items[i];
  }
  a->items[index] = v;
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
  if (!b)
    die("calloc on array");
  b->size = a->size;
  b->capacity = a->capacity;
  b->items = calloc(b->capacity, sizeof(value_t *));
  if (!b->items)
    die("calloc on b->items");
  for (int i = 0; i < a->size; i++) {
    b->items[i] = value_copy(a->items[i]);
  }
  return b;
}

value_t *init_value(int type) {
  value_t *v = calloc(1, sizeof(value_t));
  if (!v)
    die("calloc on value");
  v->type = type;
  v->escaped = false;
  return v;
}

value_t *value_copy(value_t *v) {
  value_t *a = init_value(VINT);
  a->type = v->type;
  if (v->type == VINT || v->type == VFLOAT) {
    a->int_float = v->int_float;
  } else if (v->type == VSTR || v->type == VWORD || v->type == VERR) {
    a->str_word = string_copy(v->str_word);
    a->escaped = v->escaped;
  } else if (v->type == VQUOTE) {
    a->quote = array_copy(v->quote);
  } else if (v->type == VCUSTOM) {
    custom_t *c = ht_get(OBJ_TABLE, a->str_word);
    a->custom = c->copyfunc(v->custom);
    a->str_word = string_copy(v->str_word);
  }
  return a;
}

void value_free(void *vtmp) {
  value_t *v = (value_t *)vtmp;
  if (v == NULL)
    return;
  if (v->type == VSTR || v->type == VWORD || v->type == VERR) {
    string_free(v->str_word);
  }
  if (v->type == VQUOTE) {
    array_free(v->quote);
  }
  if (v->type == VCUSTOM) {
    void (*freefunc)(void *) = ht_get(OBJ_TABLE, v->str_word);
    freefunc(v->custom);
  }
  free(v);
}

custom_t *init_custom(void (*printfunc)(void *), void (*freefunc)(void *),
                      void *(*copyfunc)(void *)) {
  custom_t *c = calloc(1, sizeof(custom_t));
  if (!c)
    die("calloc on custom");
  c->printfunc = printfunc;
  c->freefunc = freefunc;
  c->copyfunc = copyfunc;
  return c;
}

void custom_free(void *c) { free(c); }

void add_func(ht_t *h, void (*func)(value_t *), char *key) {
  ht_add(h, init_string(key), func, value_free);
}

void add_obj(ht_t *h, ht_t *h2, void (*printfunc)(void *),
             void (*freefunc)(void *), void *(*copyfunc)(void *),
             void (*createfunc)(void *), char *key) {

  custom_t *c = init_custom(printfunc, freefunc, copyfunc);
  ht_add(h, init_string(key), c, custom_free);
  ht_add(h2, init_string(key), createfunc, value_free);
}

parser_t *init_parser(char *source) {
  parser_t *p = calloc(1, sizeof(parser_t));
  if (!p)
    die("calloc on parser");
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

parser_t *parser_pp(char *s) {
  parser_t *p = init_parser(s);
  string_t *rstr = init_string(NULL);
  while (p->c != '\0') {
    if (p->c == '#') { /* Comment character is # in stem */
      while (p->c != '\n' && p->c != '\0') {
        parser_move(p);
      }
    } else {
      string_append(rstr, p->c);
      parser_move(p);
    }
  }
  free(p->source);
  parser_reset(p, rstr->value);
  free(rstr);
  return p;
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
  bool escaped = false;
  while (escaped || p->c != '"' && p->c != '\0') {
    if (p->c == '\\') {
      escaped = true;
      parser_move(p);
      continue;
    }
    if (escaped) {
      switch (p->c) {
      case '"':
        string_append(s, '"');
        break;
      case 'n':
        string_append(s, '\n');
        break;
      case 'r':
        string_append(s, '\r');
        break;
      case 't':
        string_append(s, '\t');
        break;
      case '\\':
        string_append(s, '\\');
        break;
      default:
        string_append(s, p->c);
        break;
      }
      parser_move(p);
      escaped = false;
    } else {
      string_append(s, p->c);
      parser_move(p);
      escaped = false;
    }
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
    if (p->c == '\0') {
      value_free(retv);
      parser_error(p);
    }
    array_append(retv->quote, parser_get_next(p));
    parser_skip_whitespace(p);
  }
  parser_move(p);
  return retv;
}

void parser_error(parser_t *p) {
  fprintf(stderr, "PARSER ERROR: unclosed `[` or `\"` \n");
  free(PARSER->source);
  ht_free(WORD_TABLE, value_free);
  ht_free(FLIT, func_free);
  ht_free(OBJ_TABLE, custom_free);
  array_free(STACK);
  free(PARSER);
  array_free(EVAL_STACK);
  exit(1);
}

value_t *parse_word(parser_t *p) {
  value_t *retv = init_value(VWORD);
  string_t *s = init_string(NULL);
  retv->str_word = s;
  if (p->c == '\\') {
    retv->escaped = true;
    parser_move(p);
    if (isspace(p->c) || p->c == '\0') {
      value_free(retv);
      parser_error(p);
    }
  }
  while (!isspace(p->c) && p->c != '\0') {
    string_append(s, p->c);
    parser_move(p);
  }
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

node_t *init_node(string_t *key, void *value) {
  node_t *n = calloc(1, sizeof(node_t));
  if (!n)
    die("calloc on node");
  n->key = key;
  n->value = value;
  n->next = NULL;
  return n;
}

void node_free(node_t *n, void (*freefunc)(void *)) {
  string_free(n->key);
  freefunc(n->value);
  free(n);
}

sll_t *init_sll() {
  sll_t *l = calloc(1, sizeof(sll_t));
  if (!l)
    die("calloc on linked list");
  l->size = 0;
  l->head = NULL;
  return l;
}

void sll_add(sll_t *l, string_t *s, void *v, void (*freefunc)(void *)) {
  if (l->head == NULL) {
    node_t *n = init_node(s, v);
    l->head = n;
    l->size++;
    return;
  }
  node_t *cur = l->head;
  while (cur->next != NULL) {
    if (strcmp(s->value, cur->key->value) == 0) {
      freefunc(cur->value);
      string_free(s);
      cur->value = v;
      return;
    }
    cur = cur->next;
  }
  if (strcmp(s->value, cur->key->value) == 0) {
    value_free(cur->value);
    string_free(s);
    cur->value = v;
    return;
  }
  node_t *n = init_node(s, v);
  cur->next = n;
}

void *sll_get(sll_t *l, string_t *k) {
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

void sll_delete(sll_t *l, string_t *k, void (*freefunc)(void *)) {
  node_t *cur = l->head;
  node_t *tmp;
  if (cur == NULL)
    return;
  if (strcmp(cur->key->value, k->value) == 0) {
    node_free(cur, freefunc);
    l->head = NULL;
    return;
  }
  while (cur->next != NULL) {
    if (strcmp(cur->next->key->value, k->value) == 0) {
      tmp = cur->next->next;
      node_free(cur->next, freefunc);
      cur->next = tmp;
      return;
    }
    cur = cur->next;
  }
}

void sll_free(sll_t *l, void (*func)(void *)) {
  node_t *cur = l->head;
  node_t *tmp;
  while (cur != NULL) {
    tmp = cur;
    cur = cur->next;
    node_free(tmp, func);
  }
  free(l);
}

ht_t *init_ht(size_t size) {
  ht_t *h = calloc(1, sizeof(ht_t));
  if (!h)
    die("calloc on hash table");
  h->size = size;
  h->buckets = calloc(h->size, sizeof(sll_t *));
  if (!h->buckets)
    die("calloc on hash table array");
  for (int i = 0; i < size; i++) {
    h->buckets[i] = init_sll();
  }
  return h;
}

void ht_add(ht_t *h, string_t *key, void *v, void (*freefunc)(void *)) {
  if (key == NULL)
    return;
  sll_add(h->buckets[hash(h, key->value)], key, v, freefunc);
}

void *ht_get(ht_t *h, string_t *key) {
  if (key == NULL)
    return NULL;
  return sll_get(h->buckets[hash(h, key->value)], key);
}

bool ht_exists(ht_t *h, string_t *key) { return ht_get(h, key) != NULL; }

void ht_delete(ht_t *h, string_t *key, void (*freefunc)(void *)) {
  if (key == NULL)
    return;
  sll_delete(h->buckets[hash(h, key->value)], key, freefunc);
}

void ht_free(ht_t *h, void (*func)(void *)) {
  for (int i = 0; i < h->size; i++) {
    sll_free(h->buckets[i], func);
  }
  free(h->buckets);
  free(h);
}

/* DJB2 HASH FUNCTION */
unsigned long hash(ht_t *h, char *key) {
  unsigned long hash = 5381;
  int c;

  while ((c = *key++))
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return hash % h->size;
}

bool eval_ht(value_t *v) {
  value_t *func = ht_get(WORD_TABLE, v->str_word);
  if (func == NULL)
    return false;
  value_free(v);
  if (func->type == VQUOTE) {
    for (int i = 0; i < func->quote->size; i++) {
      eval(value_copy(func->quote->items[i]));
    }
  } else {
    eval(value_copy(func));
  }
  return true;
}

bool eval_builtins(value_t *v) {
  void (*func)(value_t *) = ht_get(FLIT, v->str_word);
  if (func == NULL)
    return false;
  array_append(EVAL_STACK, v);
  func(v);
  array_pop(EVAL_STACK);
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
  case VCUSTOM:
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
  }
}
