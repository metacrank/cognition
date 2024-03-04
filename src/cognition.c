#include <better_string.h>
#include <builtins.h>
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

void eval_error(char *s) {
  value_t *v = init_value(VERR);
  v->str_word = init_string(s);
  contain_t *cur = stack_peek(STACK);
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
  if (index >= a->size)
    return NULL;
  void *retval = a->items[index];
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
  stack_t *s = a;
  for (int i = 0; i < s->size; i++) {
    freefunc(s->items[i]);
  }
  free(s->items);
  free(s);
}

void value_stack_free(void *a) {
  stack_free(a, value_free);
}

void *stack_copy(void *a, void *(*copyfunc)(void *)) {
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

void *value_stack_copy(void *a) {
  return stack_copy(a, value_copy);
}

value_t *init_value(int type) {
  value_t *v = calloc(1, sizeof(value_t));
  if (!v)
    die("calloc on value");
  v->type = type;
  v->escaped = false;
  return v;
}

void *value_copy(void *v) {
  value_t *a = init_value(VINT);
  value_t *v1 = v;
  contain_t *container = stack_peek(STACK);
  a->type = v1->type;
  if (v1->type == VINT || v1->type == VFLOAT) {
    a->int_float = v1->int_float;
  } else if (v1->type == VWORD || v1->type == VERR) {
    a->str_word = string_copy(v1->str_word);
    a->escaped = v1->escaped;
  } else if (v1->type == VSTACK) {
    a->container = contain_copy(v1->container, value_copy);
  } else if (v1->type == VCUSTOM) {
    ht_t *ot = ot_get();
    custom_t *c = ht_get(ot, a->str_word);
    a->custom = c->copyfunc(v1->custom);
    a->str_word = string_copy(v1->str_word);
  }
  return a;
}

void value_free(void *vtmp) {
  value_t *v = (value_t *)vtmp;
  contain_t *c = stack_peek(STACK);
  if (v == NULL)
    return;
  if (v->type == VWORD || v->type == VERR) {
    string_free(v->str_word);
  }
  if (v->type == VSTACK) {
    contain_free(v->container);
  }
  if (v->type == VCUSTOM) {
    void (*freefunc)(void *) = ht_get(ot_get(), v->str_word);
    freefunc(v->custom);
  }
  free(v);
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
  value_t *v = init_value(VCLIB);
  v->custom = func;
  stack_push(macro, v);
  ht_add(h, init_string(key), macro, value_stack_free);
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
  c->stack = init_stack(10);
  c->err_stack = init_stack(10);
  c->flit = flit;
  c->word_table = h;
  c->cranks = cranks;
  c->faliases = init_stack(10);
  c->delims = init_string(NULL);
  c->ignored = init_string(NULL);
  c->dflag = false;
  c->iflag = true;
  return c;
}

void contain_free(void *con) {
  contain_t *c = con;
  ht_free(c->word_table, value_free);
  ht_free(c->flit, value_stack_free);
  stack_free(c->stack, value_free);
  stack_free(c->err_stack, value_free);
}

void *contain_value_copy(void *c) {
  return contain_copy(c, value_copy);
}

void *falias_copy(void *f) {
  return string_copy(f);
}

contain_t *contain_copy(contain_t *c, void *(*copyfunc)(void *)) {
  contain_t *contain = calloc(1, sizeof(contain_t));
  if (c->word_table->size) {
    contain->word_table = ht_copy(c->word_table, contain_value_copy);
  } else {
    contain->word_table = init_ht(10);
  }
  if (c->flit->size) {
    contain->flit = ht_copy(c->flit, value_stack_copy);
  } else {
    contain->flit = init_ht(10);
  }
  contain->cranks = stack_copy(c->cranks, cranks_copy);
  contain->err_stack = stack_copy(c->err_stack, value_copy);
  contain->stack = stack_copy(c->stack, value_copy);
  contain->faliases = stack_copy(c->faliases, falias_copy);
  contain->delims = string_copy(c->delims);
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

/* parser_t *parser_pp(char *s) { */
/*   parser_t *p = init_parser(s); */
/*   string_t *rstr = init_string(NULL); */
/*   while (p->c != '\0') { */
/*     if (p->c == '#') { /\* Comment character is # in stem *\/ */
/*       while (p->c != '\n' && p->c != '\0') { */
/*         parser_move(p); */
/*       } */
/*     } else { */
/*       string_append(rstr, p->c); */
/*       parser_move(p); */
/*     } */
/*   } */
/*   free(p->source); */
/*   parser_reset(p, rstr->value); */
/*   free(rstr); */
/*   return p; */
/* } */

void parser_move(parser_t *p) {
  if (p->i < strlen(p->source) && p->c != '\0') {
    p->i++;
    p->c = p->source[p->i];
  }
}

bool isdelim(parser_t *p) { contain_t *c = stack_peek(STACK); }

value_t *parse_word(parser_t *p) {
  string_t *strval = init_string(NULL);
  value_t *retval = init_value(VWORD);
  retval->str_word = strval;
  do {
    string_append(strval, p->c);
    parser_move(p);
  } while (!isdelim(p) && p->c);
  return retval;
}

bool isignore(parser_t *p) {
  contain_t *c = stack_peek(STACK);
  if (c->iflag) {
    for (int i = 0; i < c->ignored->length; i++) {
      if (c->ignored->value[i] == p->c) {
        return true;
      }
      return false;
    }
  } else {
    for (int i = 0; i < c->ignored->length; i++) {
      if (c->ignored->value[i] != p->c) {
        return false;
      }
    }
    return true;
  }
  return false;
}

void parser_skip_ignore(parser_t *p) {
  while (isignore(p) && p->c != '\0') {
    parser_move(p);
  }
}

value_t *parser_get_next(parser_t *p) {
  parser_skip_ignore(p);
  switch (p->c) {
  case '\0':
    return NULL;
  case EOF:
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

sll_t *sll_copy(sll_t *l, void *(*copyfunc)(void *)) {
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

void inc_crank() {
  contain_t *cur = stack_peek(STACK);
  void **defcranks = cur->cranks->items;
  int(*crank)[2];
  for (int i = 0; i < cur->cranks->size; i++) {
    crank = defcranks[i];
    crank[0][0]++;
    if (crank[0][0] >= crank[0][1]) {
      crank[0][0] = 0;
    }
  }
}

bool isfaliasin(contain_t *c, value_t *v) {
  string_t *falias;
  for (int i = 0; i < c->faliases->size; i++) {
    falias = c->faliases->items[i];
    if (strcmp(falias->value, v->str_word->value) == 0)
      return true;
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
    switch (newval->type) {
      case VWORD:
        stack_push(family, c);
        expandword(newval, new, family);
        break;
      default:
        stack_push(new->stack, newval);
    }
  }
}

void expandword(value_t *v, contain_t *new, stack_t *family) {
  contain_t *expand;
  stack_t *macro;
  bool evald = false;
  for (int i = family->size - 1; i >= 0; i--) {
    contain_t *parent = family->items[i];
    if ((macro = ht_get(parent->flit, v->str_word))) {
      stack_push(new->stack, v); // change
      evald = true;
      break;
    } else if ((expand = ht_get(parent->word_table, v->str_word))) {
      expandstack(expand, new, family);
      evald = true;
      break;
    } else if ((isfaliasin(parent, v))) {
      value_t *f = init_value(VCLIB);
      void(*func)(value_t *v) = cog_eval;
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
  if (cur->stack->size == 0) {
    value_t *err = init_value(VERR);
    eval_error("EMPTY STACK");
    return;
  }
  value_t *v = stack_pop(cur->stack);
  stack_push(EVAL_STACK, v);
  stack_t *family = init_stack(10);
  stack_push(family, cur);
  evalstack(v->container, family);
  value_t *vf = stack_pop(EVAL_STACK);
  if(vf) {
    value_free(vf);
  }
}

void *func_copy(void *funcs) { return NULL; }

void *cranks_copy(void *cranks) {}

void contain_push(contain_t *c, value_t *v) {
  contain_t *contain = init_contain(ht_copy(c->word_table, value_copy),
                                    ht_copy(c->flit, func_copy),
                                    stack_copy(c->cranks, cranks_copy));
  value_t *container = init_value(VSTACK);
  container->container = contain;
  stack_push(contain->stack, v);
  contain_t *otherc = stack_peek(STACK);
  stack_push(otherc->stack, container);
}

void push_quoted(contain_t *cur, value_t *v) {
  value_t *q = init_value(VSTACK);
  q->container = init_contain(init_ht(1), init_ht(1), init_stack(1));
  stack_push(q->container->stack, v);
  stack_push(cur->stack, q);
}

void evalstack(contain_t *c, stack_t *family) {
  contain_t *cur = stack_peek(STACK);
  for (int i = 0; i < c->stack->size; i++) {
    value_t *newval = c->stack->items[i];
    switch (newval->type) {
      case VWORD:
        stack_push(family, c);
        evalword(newval, family);
        break;
      case VSTACK:
        stack_push(cur->stack, newval);
        crank();
        break;
      case VCLIB:
        ((void(*)(value_t *v))(newval->custom))(newval);
        crank();
        break;
      default:
        push_quoted(cur, newval);
        crank();
    }
  }
}

void evalmacro(stack_t *macro, stack_t *family) {
  contain_t *cur = stack_peek(STACK);
  value_t *v;
  value_t *q;
  for (int i = 0; i < macro->size; i++) {
    v = macro->items[i];
    switch (v->type) {
      case VCLIB:
        ((void(*)(value_t *v))(v->custom))(v);
        break;
      case VSTACK:
        stack_push(cur->stack, v);
        break;
      case VWORD:
        evalword(v, family);
        break;
      default:
        push_quoted(cur, v);
    }
  }
}

void evalword(value_t *v, stack_t *family) {
  contain_t *expand;
  stack_t *macro;
  bool evald = false;
  for (int i = family->size - 1; i >= 0; i--) {
    contain_t *parent = family->items[i];
    if ((macro = ht_get(parent->flit, v->str_word))) {
      evalmacro(macro, family);
      crank();
      evald = true;
      break;
    } else if ((expand = ht_get(parent->word_table, v->str_word))) {
      evalstack(expand, family);
      evald = true;
      break;
    } else if ((isfaliasin(parent, v))) {
      evalf();
      evald = true;
      break;
    }
  }
  if (!evald) {
    push_quoted(stack_peek(STACK), v);
    crank();
  }
}

void crank() {
  contain_t *cur = stack_peek(STACK);
  int cindex = -1;
  int(*crank)[2];
  for (int i = 0; i < cur->cranks->size; i++) {
    crank = cur->cranks->items[i];
    if (*crank[0] == 0 && *crank[1]) {
      cindex = i;
      break;
    }
  }
  if (cindex >= 0) {
    int fixedindex = cur->stack->size - 1 - cindex;
    value_t *needseval = stack_popdeep(cur->stack, fixedindex);
    stack_t *family = init_stack(10);
    stack_push(family, cur);
    evalstack(needseval->container, family);
  }
  inc_crank();
}

void eval(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  if (isfalias(v)) {
    if (cur->cranks->size == 0) {
      evalf();
      return;
    }
    int(*crank)[2] = cur->cranks->items[0];
    if (*crank[0] || !(*crank[1])) {
      evalf();
      return;
    }
  }
  push_quoted(cur, v);
  crank();
}
