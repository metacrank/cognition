#include <better_string.h>
#include <builtins.h>
#include <builtinslib.h>
#include <cognition.h>
#include <ctype.h>
#include <macros.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Global variables defined */
stack_t *STACK;
stack_t *EVAL_STACK;
// stack of tuples of: pointer to 'root' stack, ht_t*
stack_t *OBJ_STACK;
parser_t *PARSER;

void func_free(void *f) {}

void eval_error(char *s, value_t *w) {
  value_t *v = init_value(VERR);
  v->error = calloc(1, sizeof(error_t));
  if (w)
    v->error->str_word = string_copy(w->str_word);
  else
    v->error->str_word = NULL;
  v->error->error = init_string(s);
  contain_t *cur = stack_peek(STACK);
  if (cur->err_stack == NULL)
    cur->err_stack = init_stack(DEFAULT_STACK_SIZE);
  stack_push(cur->err_stack, v);
}

stack_t *init_stack(size_t size) {
  stack_t *a = calloc(1, sizeof(stack_t));
  if (!a)
    die("calloc on stack");
  a->size = 0;
  a->capacity = size;
  a->items = calloc(a->capacity, sizeof(value_t *));
  if (!a->items)
    die("calloc on a->items");
  return a;
}

void stack_push(stack_t *a, void *v) {
  if (a->size >= a->capacity - 2) {
    a->capacity = a->capacity * 2;
    a->items = realloc(a->items, a->capacity * sizeof(value_t *));
  }
  a->items[a->size] = v;
  a->size++;
}

void stack_add(stack_t *a, value_t *v, int index) {
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

void *stack_popdeep(stack_t *a, int index) {
  if (index >= a->size || index < 0)
    return NULL;
  void *retval = a->items[index];
  a->size--;
  for (int i = index; i < a->size; i++) {
    a->items[i] = a->items[i + 1];
  }
  return retval;
}

void *stack_pop(stack_t *a) {
  if (a->size > 0) {
    void *v = a->items[a->size - 1];
    a->size--;
    return v;
  }
  return NULL;
}

void *stack_peek(stack_t *s) {
  if (s->size > 0) {
    void *v = s->items[s->size - 1];
    return v;
  }
  return NULL;
}

void stack_extend(stack_t *a, stack_t *b) {
  for (int i = 0; i < b->size; i++) {
    stack_push(a, b->items[i]);
  }
}

void stack_free(void *a, void (*freefunc)(void *)) {
  if (a == NULL)
    return;
  stack_t *s = a;
  for (int i = 0; i < s->size; i++) {
    freefunc(s->items[i]);
  }
  free(s->items);
  free(s);
}

void value_stack_free(void *a) { stack_free(a, value_free); }

void *stack_copy(void *a, void *(*copyfunc)(void *)) {
  if (a == NULL) {
    return NULL;
  }
  stack_t *b = calloc(1, sizeof(stack_t));
  if (!b)
    die("calloc on stack");
  stack_t *s = a;
  b->size = s->size;
  b->capacity = s->capacity;
  b->items = calloc(b->capacity, sizeof(value_t *));
  if (!b->items)
    die("calloc on b->items");
  for (int i = 0; i < s->size; i++) {
    b->items[i] = copyfunc(s->items[i]);
  }
  return b;
}

void *value_stack_copy(void *a) { return stack_copy(a, value_copy); }

value_t *init_value(int type) {
  value_t *v = calloc(1, sizeof(value_t));
  if (!v)
    die("calloc on value");
  v->type = type;
  return v;
}

void *value_copy(void *v) {
  if (v == NULL)
    return NULL;
  value_t *a = init_value(VWORD);
  value_t *v1 = v;
  contain_t *container = stack_peek(STACK);
  a->type = v1->type;
  if (v1->type == VWORD || v1->type == VERR) {
    a->str_word = string_copy(v1->str_word);
  } else if (v1->type == VSTACK) {
    a->container = contain_copy(v1->container, value_copy);
  } else if (v1->type == VCLIB) {
    a->str_word = v1->str_word;
    a->custom = v1->custom;
  } else if (v1->type == VCUSTOM) {
    ht_t *ot = ot_get();
    custom_t *c = ht_get(ot, a->str_word);
    a->custom = c->copyfunc(v1->custom);
    a->str_word = string_copy(v1->str_word);
  }
  return a;
}

void value_free(void *vtmp) {
  if (vtmp == NULL)
    return;
  value_t *v = (value_t *)vtmp;
  if (v == NULL)
    return;
  if (v->type == VWORD || v->type == VCLIB || v->type == VCUSTOM) {
    string_free(v->str_word);
  }
  if (v->type == VSTACK) {
    contain_free(v->container);
  }
  if (v->type == VERR) {
    error_free(v->error);
  }
  if (v->type == VCUSTOM) {
    void (*freefunc)(void *) = ht_get(ot_get(), v->str_word);
    freefunc(v->custom);
  }
  free(v);
}

void error_free(void *err) {
  if (err == NULL)
    return;
  error_t *e = err;
  string_free(e->error);
  string_free(e->str_word);
  free(e);
}

ht_t *ot_get() {
  if (STACK->size > 0) {
    contain_t *root = STACK->items[0];
    contain_t *ref;
    ht_t *ot;
    void *(*entry)[2];
    for (int i = 0; i < OBJ_STACK->size; i++) {
      entry = OBJ_STACK->items[i];
      ref = *entry[0];
      if (ref == root) {
        ot = *entry[1];
        return ot;
      }
    }
  }
  return NULL;
}

void obj_free(void *v) {
  void **entry = v;
  ht_t *ot = entry[1];
  ht_free(ot, free);
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
  stack_t *macro = init_stack(1);
  contain_t *c = calloc(1, sizeof(contain_t));
  c->stack = macro;
  value_t *v = init_value(VCLIB);
  v->str_word = init_string(key);
  v->custom = func;
  stack_push(macro, v);
  ht_add(h, init_string(key), c, value_stack_free);
}

void add_macro(ht_t *h, stack_t *macro, char *key) {
  ht_add(h, init_string(key), macro, value_stack_free);
}

void add_obj(ht_t *h, ht_t *h2, void (*printfunc)(void *),
             void (*freefunc)(void *), void *(*copyfunc)(void *),
             void (*createfunc)(void *), char *key) {

  custom_t *c = init_custom(printfunc, freefunc, copyfunc);
  ht_add(h, init_string(key), c, custom_free);
  ht_add(h2, init_string(key), createfunc, value_free);
}

contain_t *init_contain(ht_t *h, ht_t *flit, stack_t *cranks) {
  contain_t *c = calloc(1, sizeof(contain_t));
  c->stack = init_stack(DEFAULT_STACK_SIZE);
  c->err_stack = NULL;
  c->flit = flit;
  c->word_table = h;
  c->cranks = cranks;
  c->faliases = NULL;
  c->delims = NULL;
  c->ignored = NULL;
  c->singlets = NULL;
  c->dflag = false;
  c->iflag = true;
  c->sflag = true;
  return c;
}

void contain_free(void *con) {
  if (con == NULL)
    return;
  contain_t *c = con;
  ht_free(c->word_table, contain_free);
  ht_free(c->flit, contain_free);
  stack_free(c->stack, value_free);
  stack_free(c->err_stack, value_free);
  stack_free(c->cranks, free);
  stack_free(c->faliases, (void (*)(void *))string_free);
  string_free(c->delims);
  string_free(c->ignored);
  string_free(c->singlets);
  free(c);
}

void *contain_value_copy(void *c) { return contain_copy(c, value_copy); }

void *falias_copy(void *f) { return string_copy(f); }

contain_t *contain_copy(contain_t *c, void *(*copyfunc)(void *)) {
  if (c == NULL) {
    return NULL;
  }
  contain_t *contain = calloc(1, sizeof(contain_t));
  contain->word_table = ht_copy(c->word_table, contain_value_copy);
  contain->word_table = NULL;
  contain->flit = ht_copy(c->flit, value_stack_copy);
  contain->cranks = stack_copy(c->cranks, cranks_copy);
  contain->err_stack = stack_copy(c->err_stack, value_copy);
  contain->stack = stack_copy(c->stack, copyfunc);
  contain->faliases = stack_copy(c->faliases, falias_copy);
  contain->delims = string_copy(c->delims);
  contain->singlets = string_copy(c->singlets);
  contain->ignored = string_copy(c->ignored);
  contain->dflag = c->dflag;
  contain->iflag = c->iflag;
  contain->sflag = c->sflag;
  return contain;
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

void parser_move(parser_t *p) {
  if (p->i < strlen(p->source) && p->c != '\0') {
    p->i++;
    p->c = p->source[p->i];
  }
}

value_t *parse_word(parser_t *p, bool skipped) {
  string_t *strval = init_string(NULL);
  value_t *retval = init_value(VWORD);
  retval->str_word = strval;
  if (issinglet(p->c)) {
    string_append(strval, p->c);
    parser_move(p);
    return retval;
  }
  if (!skipped && p->c) {
    string_append(strval, p->c);
    parser_move(p);
  }
  while (!isdelim(p->c) && p->c) {
    string_append(strval, p->c);
    if (issinglet(p->c)) {
      parser_move(p);
      return retval;
    }
    parser_move(p);
  }
  return retval;
}

bool issinglet(char c) {
  contain_t *cur = stack_peek(STACK);
  if (cur->sflag) {
    if (cur->singlets == NULL)
      return false;
    for (int i = 0; i < cur->singlets->length; i++) {
      if (cur->singlets->value[i] == c) {
        return true;
      }
    }
    return false;
  }
  if (cur->singlets == NULL)
    return true;
  for (int i = 0; i < cur->singlets->length; i++) {
    if (cur->singlets->value[i] == c) {
      return false;
    }
  }
  return true;
}

bool isignore(char c) {
  contain_t *cur = stack_peek(STACK);
  if (cur->iflag) {
    if (cur->ignored == NULL)
      return false;
    for (int i = 0; i < cur->ignored->length; i++) {
      if (cur->ignored->value[i] == c) {
        return true;
      }
    }
    return false;
  }
  if (cur->ignored == NULL)
    return true;
  for (int i = 0; i < cur->ignored->length; i++) {
    if (cur->ignored->value[i] == c) {
      return false;
    }
  }
  return true;
}

bool isdelim(char c) {
  contain_t *cur = stack_peek(STACK);
  if (cur->dflag) {
    if (cur->delims == NULL)
      return false;
    for (int i = 0; i < cur->delims->length; i++) {
      if (cur->delims->value[i] == c) {
        return true;
      }
    }
    return false;
  }
  if (cur->delims == NULL)
    return true;
  for (int i = 0; i < cur->delims->length; i++) {
    if (cur->delims->value[i] == c) {
      return false;
    }
  }
  return true;
}

bool parser_skip_ignore(parser_t *p) {
  bool skipped = false;
  while (isignore(p->c) && p->c != '\0') {
    parser_move(p);
    skipped = true;
  }
  return skipped;
}

value_t *parser_get_next(parser_t *p) {
  bool skipped = parser_skip_ignore(p);
  switch (p->c) {
  case '\0':
    return NULL;
  case EOF:
    return NULL;
  default:
    return parse_word(p, skipped);
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

node_t *node_copy(node_t *n, void *(*copyfunc)(void *)) {
  if (!n)
    return NULL;
  node_t *nn = init_node(string_copy(n->key), copyfunc(n->value));
  return nn;
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
    freefunc(cur->value);
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

sll_t *sll_copy(sll_t *l, void *(*copyfunc)(void *)) {
  if (!l) return NULL;
  sll_t *list = init_sll();
  list->head = node_copy(l->head, copyfunc);
  node_t *nn = list->head;
  node_t *n = l->head;
  while (n) {
    nn = node_copy(n->next, copyfunc);
    n = n->next;
    nn = nn->next;
  }
}

void sll_free(sll_t *l, void (*func)(void *)) {
  if (!l) return;
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
    die("calloc on hash table stack");
  for (int i = 0; i < size; i++) {
    h->buckets[i] = init_sll();
  }
  return h;
}

ht_t *ht_copy(ht_t *h, void *(*copyfunc)(void *)) {
  if (h == NULL)
    return NULL;
  ht_t *ht = calloc(1, sizeof(ht_t));
  ht->buckets = calloc(h->size, sizeof(sll_t *));
  for (int i = 0; i < h->size; i++) {
    ht->buckets[i] = sll_copy(h->buckets[i], copyfunc);
  }
  ht->size = h->size;
  return ht;
}

void ht_add(ht_t *h, string_t *key, void *v, void (*freefunc)(void *)) {
  if (key == NULL)
    return;
  sll_add(h->buckets[hash(h, key->value)], key, v, freefunc);
}

void *ht_get(ht_t *h, string_t *key) {
  if (key == NULL || h == NULL)
    return NULL;
  return sll_get(h->buckets[hash(h, key->value)], key);
}

bool ht_exists(ht_t *h, string_t *key) { return ht_get(h, key) != NULL; }

void ht_delete(ht_t *h, string_t *key, void (*freefunc)(void *)) {
  if (h == NULL)
    return;
  if (key == NULL)
    return;
  sll_delete(h->buckets[hash(h, key->value)], key, freefunc);
}

void ht_free(ht_t *h, void (*func)(void *)) {
  if (h == NULL)
    return;
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

void inc_crank() {
  contain_t *cur = stack_peek(STACK);
  if (cur->cranks == NULL)
    return;
  int(*crank)[2];
  for (int i = 0; i < cur->cranks->size; i++) {
    crank = cur->cranks->items[i];
    crank[0][0]++;
    if (crank[0][0] >= crank[0][1]) {
      crank[0][0] = 0;
    }
  }
}

void dec_crank() {
  contain_t *cur = stack_peek(STACK);
  if (cur->cranks == NULL)
    return;
  int(*crank)[2];
  for (int i = 0; i < cur->cranks->size; i++) {
    crank = cur->cranks->items[i];
    crank[0][0]--;
    if (crank[0][0] < 0) {
      crank[0][0] = crank[0][1];
    }
  }
}

bool isfaliasin(contain_t *c, value_t *v) {
  if (c->faliases == NULL)
    return false;
  string_t *falias;
  if (c->faliases) {
    for (int i = 0; i < c->faliases->size; i++) {
      falias = c->faliases->items[i];
      if (strcmp(falias->value, v->str_word->value) == 0)
        return true;
    }
    return false;
  }
  return false;
}

bool isfalias(value_t *v) {
  contain_t *c = stack_peek(STACK);
  return isfaliasin(c, v);
}

void expandstack(contain_t *c, contain_t *new, stack_t *family) {
  for (int i = 0; i < c->stack->size; i++) {
    value_t *newval = c->stack->items[i];
    c->stack->items[i] = NULL;
    switch (newval->type) {
      case VWORD:
        stack_push(family, c);
        expandword(newval, new, family);
        stack_pop(family);
        value_free(newval);
        break;
      default:
        stack_push(new->stack, newval);
    }
  }
}

void expandword(value_t *v, contain_t *new, stack_t *family) {
  contain_t *expand;
  bool evald = false;
  for (int i = family->size - 1; i >= 0; i--) {
    contain_t *parent = family->items[i];
    if ((expand = ht_get(parent->flit, v->str_word))) {
      for (int i = 0; i < expand->stack->size; i++) {
        stack_push(new->stack, value_copy(expand->stack->items[i]));
      }
      evald = true;
      break;
    } else if ((expand = ht_get(parent->word_table, v->str_word))) {
      expandstack(expand, new, family);
      evald = true;
      break;
    } else if ((isfaliasin(parent, v))) {
      value_t *f = init_value(VCLIB);
      void (*func)(value_t *) = cog_eval;
      f->custom = func;
      stack_push(new->stack, f);
      evald = true;
      break;
    }
  }
  if (!evald) {
    stack_push(new->stack, v);
  }
}

void evalf() {
  contain_t *cur = stack_peek(STACK);
  value_t *v = stack_pop(cur->stack);
  if (v == NULL) {
    eval_error("EMPTY STACK", NULL);
    return;
  }
  stack_push(EVAL_STACK, v);
  stack_t *family = init_stack(10);
  stack_push(family, cur);
  evalstack(v->container, family);
  free(family->items);
  free(family);
  value_t *vf = stack_pop(EVAL_STACK);
  if (vf) {
    value_free(vf);
  }
}

void *func_copy(void *funcs) { return NULL; }

void *cranks_copy(void *cranks) {}

void push_quoted(contain_t *cur, value_t *v) {
  value_t *q = init_value(VSTACK);
  q->container = init_contain(NULL, NULL, NULL);
  stack_push(q->container->stack, v);
  stack_push(cur->stack, q);
}

void eval_value(contain_t *c, stack_t *family, contain_t *cur, value_t *val) {
  switch (val->type) {
  case VWORD:
    stack_push(family, c);
    evalword(val, family, false);
    stack_pop(family);
    break;
  case VSTACK:
    stack_push(cur->stack, value_copy(val));
    inc_crank();
    break;
  case VCLIB:
    ((void (*)(value_t * v))(val->custom))(NULL);
    inc_crank();
    break;
  default:
    push_quoted(cur, value_copy(val));
    inc_crank();
  }
}

void evalstack(contain_t *c, stack_t *family) {
  contain_t *cur = stack_peek(STACK);
  if (c->stack->size) {
    eval_value(c, family, cur, c->stack->items[0]);
    int(*cr)[2];
    for (int i = 1; i < c->stack->size; i++) {
      value_t *newval = c->stack->items[i];
      if (cur->cranks) {
        cr = cur->cranks->items[0];
        if (cr[0][0] == 0 && cr[0][1]) {
          eval_value(c, family, cur, newval);
          continue;
        }
      }
      if (newval->type != VWORD) {
        push_quoted(cur, value_copy(newval));
        crank();
        continue;
      }
      bool evald = false;
      for (int i = 0; i < family->size; i++) {
        contain_t *parent = family->items[i];
        if (isfaliasin(parent, newval)) {
          evalf();
          evald = true;
          break;
        }
      }
      if (evald) continue;
      if (isfaliasin(c, newval)) {
        evalf();
      }
    }
    return;
  }
  inc_crank();
}

void evalmacro(contain_t *c, value_t *word, stack_t *family) {
  contain_t *cur = stack_peek(STACK);
  stack_t *macro = c->stack;
  value_t *v;
  for (int i = 0; i < macro->size; i++) {
    v = macro->items[i];
    switch (v->type) {
    case VCLIB:
      ((void (*)(value_t *))(v->custom))(word);
      break;
    case VSTACK:
      stack_push(cur->stack, value_copy(v));
      break;
    case VWORD:
      if (isfaliasin(c, v)) {
        evalf();
      }
      else {
        stack_push(family, c);
        evalword(v, family, true);
        stack_pop(family);
      }
      break;
    default:
      push_quoted(cur, value_copy(v));
    }
  }
}

void evalword(value_t *v, stack_t *family, bool m) {
  contain_t *expand;
  stack_t *macro;
  bool evald = false;
  for (int i = family->size - 1; i >= 0; i--) {
    contain_t *parent = family->items[i];
    if ((expand = ht_get(parent->flit, v->str_word))) {
      evalmacro(expand, v, family);
      inc_crank();
      evald = true;
      break;
    } else if ((expand = ht_get(parent->word_table, v->str_word))) {
      evalstack(expand, family);
      evald = true;
      break;
    } else if (m && isfaliasin(parent, v)) {
      evalf();
      evald = true;
      break;
    }
  }
  if (!evald) {
    push_quoted(stack_peek(STACK), value_copy(v));
  }
}

void crank() {
  contain_t *cur = stack_peek(STACK);
  if (cur->cranks == NULL)
    return;
  int cindex = -1;
  int(*crank)[2];
  for (int i = 0; i < cur->cranks->size; i++) {
    crank = cur->cranks->items[i];
    if (crank[0][0] == 0 && crank[0][1]) {
      cindex = i;
      break;
    }
  }
  if (cindex >= 0) {
    int fixedindex = cur->stack->size - 1 - cindex;
    value_t *needseval = stack_popdeep(cur->stack, fixedindex);
    if (!needseval) {
      eval_error("CRANK TOO DEEP", NULL);
      inc_crank();
      return;
    }
    stack_push(EVAL_STACK, needseval);
    stack_t *family = init_stack(10);
    stack_push(family, cur);
    evalstack(needseval->container, family);
    free(family->items);
    free(family);
    value_t *vf = stack_pop(EVAL_STACK);
    if (vf) {
      value_free(vf);
    }
  } else {
    inc_crank();
  }
}

void eval(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  if (isfalias(v)) {
    value_free(v);
    if (!cur->cranks) {
      evalf();
      return;
    }
    if (cur->cranks->size == 0) {
      evalf();
      return;
    }
    int(*crank)[2] = cur->cranks->items[0];
    if (crank[0][0] != 1 || crank[0][1] == 0) {
      printf("evaling\n");
      evalf();
      return;
    }
  }
  push_quoted(cur, v);
  crank();
}
