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
#include <hash_table.h>

/* Global variables defined */
stack_t *STACK;
stack_t *EVAL_STACK;
stack_t *CONTAIN_DEF_STACK;
stack_t *MACRO_DEF_STACK;
stack_t *FAMILY;
stack_t *FAMILY_IDX;
stack_t *CONTAINERS;
stack_t *MACROS;
ht_t *OBJ_TABLE;
parser_t *PARSER;
string_t *EXIT_CODE;
bool *EXITED;

// for debugging
void print_crank(char prefix[]) {
  contain_t *cur = stack_peek(STACK);
  int mod = 0;
  int base = 0;
  int(*cr)[2] = NULL;
  if (cur->cranks)
    if (cur->cranks->size)
      cr = cur->cranks->items[0];
  if (cr) {
    mod = cr[0][0];
    base = cr[0][1];
  }
  printf("%s: modcrank %d, crankbase %d\n", prefix, mod, base);
}

void func_free(void *f) {}

void eval_error(char32_t *s, value_t *w) {
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
  if (a == NULL)
    return;
  if (a->size >= a->capacity - 2) {
    a->capacity = a->capacity * 2;
    a->items = realloc(a->items, a->capacity * sizeof(value_t *));
  }
  a->items[a->size] = v;
  a->size++;
}

void stack_add(stack_t *a, value_t *v, int index) {
  if (a == NULL)
    return;
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
  if (a == NULL)
    return NULL;
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
  if (a == NULL)
    return NULL;
  if (a->size > 0) {
    void *v = a->items[a->size - 1];
    a->size--;
    return v;
  }
  return NULL;
}

void *stack_peek(stack_t *s) {
  if (s == NULL)
    return NULL;
  if (s->size > 0) {
    void *v = s->items[s->size - 1];
    return v;
  }
  return NULL;
}

void stack_extend(stack_t *a, stack_t *b) {
  if (a == NULL || b == NULL)
    return;
  for (int i = 0; i < b->size; i++) {
    stack_push(a, b->items[i]);
  }
}

void stack_empty(void *a, void (*freefunc)(void *)) {
  if (a == NULL)
    return;
  stack_t *s = a;
  for (int i = 0; i < s->size; i++) {
    freefunc(s->items[i]);
  }
  s->size = 0;
}

bool stack_exists(stack_t *a, void *e) {
  if (a == NULL || e == NULL)
    return false;
  for (int i = 0; i < a->size; i++) {
    if (a->items[i] == e)
      return true;
  }
  return false;
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
  } else if (v1->type == VMACRO) {
    a->macro = value_stack_copy(v1->macro);
  } else if (v1->type == VCLIB) {
    a->str_word = string_copy(v1->str_word);
    a->custom = v1->custom;
  } else if (v1->type == VCUSTOM) {
    custom_t *c = ht_get(OBJ_TABLE, a->str_word);
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
  if (v->type == VMACRO) {
    value_stack_free(v->macro);
  }
  if (v->type == VERR) {
    error_free(v->error);
  }
  if (v->type == VCUSTOM) {
    custom_t *cstm = ht_get(OBJ_TABLE, v->str_word);
    cstm->freefunc(v->custom);
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

void add_func(ht_t *h, void (*func)(value_t *), char32_t *key) {
  stack_t *macro = init_stack(1);
  value_t *v = init_value(VCLIB);
  v->str_word = init_string(key);
  v->custom = func;
  stack_push(macro, v);
  ht_add(h, init_string(key), macro, value_stack_free);
}

void add_macro(ht_t *h, stack_t *macro, char32_t *key) {
  ht_add(h, init_string(key), macro, value_stack_free);
}

void add_obj(ht_t *h, ht_t *h2, void (*printfunc)(void *),
             void (*freefunc)(void *), void *(*copyfunc)(void *),
             void (*createfunc)(void *), char32_t *key) {

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
  ht_free(c->flit, value_stack_free);
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

parser_t *init_parser(string_t *source) {
  parser_t *p = calloc(1, sizeof(parser_t));
  if (!p)
    die("calloc on parser");
  p->i = 0;
  p->source = source;
  p->c = source->value[0];
  return p;
}

void parser_reset(parser_t *p, string_t *source) {
  p->source = source;
  p->i = 0;
  p->c = source->value[0];
}

void parser_move(parser_t *p) {
  if (p->i < p->source->length) {
    p->i++;
    p->c = p->source->value[p->i];
  }
}

bool parser_on(parser_t *p) {
  if (p->i < p->source->length) {
    return true;
  }
  return false;
}

value_t *parse_word(parser_t *p, bool skipped) {
  string_t *strval = init_string(U"");
  value_t *retval = init_value(VWORD);
  retval->str_word = strval;
  if (issinglet(p->c)) {
    string_append(strval, p->c);
    parser_move(p);
    return retval;
  }
  if (!skipped && parser_on(p)) {
    string_append(strval, p->c);
    parser_move(p);
  }
  while (parser_on(p)) {
    if (isdelim(p->c))
      break;
    string_append(strval, p->c);
    if (issinglet(p->c)) {
      parser_move(p);
      return retval;
    }
    parser_move(p);
  }
  return retval;
}

bool issinglet(char32_t c) {
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

bool isignore(char32_t c) {
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

bool isdelim(char32_t c) {
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
  while (parser_on(p)) {
    if (!isignore(p->c))
      break;
    parser_move(p);
    skipped = true;
  }
  return skipped;
}

value_t *parser_get_next(parser_t *p) {
  bool skipped = parser_skip_ignore(p);
  if (parser_on(p))
    return parse_word(p, skipped);
  return NULL;
}

void inc_crank(contain_t *cur) {
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

void dec_crank(contain_t *cur) {
  if (cur->cranks == NULL)
    return;
  int(*crank)[2];
  for (int i = 0; i < cur->cranks->size; i++) {
    crank = cur->cranks->items[i];
    crank[0][0]--;
    if (crank[0][0] < 0) {
      crank[0][0] = crank[0][1] - 1;
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
      if (string_comp(falias, v->str_word) == 0)
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

/* void expandstack(stack_t *s, stack_t *new, stack_t *family) { */
/*   stack_t *new2; */
/*   for (int i = 0; i < s->size; i++) { */
/*     value_t *newval = s->items[i]; */
/*     s->items[i] = NULL; */
/*     bool evald = false; */
/*     if (newval->type == VWORD) { */
/*       evald = expandword(newval, new, family); */
/*     } */
/*     if (evald) { */
/*       value_free(newval); */
/*     } else { */
/*       stack_push(new, newval); */
/*     } */
/*     s->items[i] = NULL; */
/*   } */
/* } */

/* bool expandword(value_t *v, stack_t *new, stack_t *family) { */
/*   contain_t *expand; */
/*   stack_t *macro; */
/*   bool evald = false; */
/*   for (int i = family->size - 1; i >= 0; i--) { */
/*     contain_t *parent = family->items[i]; */
/*     if ((macro = ht_get(parent->flit, v->str_word))) { */
/*       for (int i = 0; i < macro->size; i++) { */
/*         stack_push(new, value_copy(macro->items[i])); */
/*       } */
/*       evald = true; */
/*       break; */
/*     } else if ((expand = ht_get(parent->word_table, v->str_word))) { */
/*       contain_t *expand2 = contain_value_copy(expand); */
/*       stack_push(family, expand2); */
/*       expandstack(expand2->stack, new, family); */
/*       stack_pop(family); */
/*       contain_free(expand2); */
/*       evald = true; */
/*       break; */
/*     } else if ((isfaliasin(parent, v))) { */
/*       value_t *f = init_value(VCLIB); */
/*       f->str_word = string_copy(v->str_word); */
/*       void (*func)(value_t *) = evalf; */
/*       f->custom = func; */
/*       stack_push(new, f); */
/*       evald = true; */
/*       break; */
/*     } */
/*   } */
/*   return evald; */
/* } */

void evalf() {
  contain_t *cur = stack_peek(STACK);
  value_t *v = stack_pop(cur->stack);
  if (v == NULL) {
    eval_error(U"EMPTY STACK", NULL);
    return;
  }
  stack_push(EVAL_STACK, v);
  if (v->type == VSTACK) {
    stack_push(FAMILY, v->container);
    evalstack(v->container, NULL);
    stack_pop(FAMILY);
  } else if (v->type == VMACRO) {
    evalmacro(v->macro, NULL);
  } else
    die("BAD VALUE TYPE ON STACK");
  inc_crank(cur);
  value_t *vf = stack_pop(EVAL_STACK);
  if (vf) {
    value_free_safe(vf);
  }
  return_function(NULL, false);
}

void *func_copy(void *funcs) { return NULL; }

void *cranks_copy(void *cranks) {
  int(*cr)[2] = cranks;
  int(*arr)[2] = malloc(sizeof(int[2]));
  arr[0][0] = cr[0][0];
  arr[0][1] = cr[0][1];
  return arr;
}

void push_quoted(contain_t *cur, value_t *v) {
  value_t *q = init_value(VSTACK);
  q->container = init_contain(NULL, NULL, NULL);
  stack_push(q->container->stack, v);
  stack_push(cur->stack, q);
}

void eval_value(contain_t *c, contain_t *cur, value_t *val, value_t *callword) {
  switch (val->type) {
  case VWORD:
    evalword(val);
    break;
  case VSTACK:
    stack_push(cur->stack, value_copy(val));
    break;
  case VMACRO:
    stack_push(cur->stack, value_copy(val));
    break;
  case VCLIB:
    ((void (*)(value_t * v))(val->custom))(callword);
    break;
  default:
    push_quoted(cur, value_copy(val));
  }
}

bool return_function(void *stack, bool macro) {
  if (EXITED)
    return true;
  if (macro) {
    if (stack_exists(MACRO_DEF_STACK, stack)) {
      return true;
    }
  } else if (stack_exists(CONTAIN_DEF_STACK, stack)) {
    return true;
  }
  for (int i = 0; i < CONTAIN_DEF_STACK->size; i++) {
    if (stack_exists(CONTAINERS, CONTAIN_DEF_STACK->items[i])) {
      return true;
    }
  }
  for (int i = 0; i < MACRO_DEF_STACK->size; i++) {
    if (stack_exists(MACROS, MACRO_DEF_STACK->items[i])) {
      return true;
    }
  }
  stack_empty(CONTAIN_DEF_STACK, contain_free);
  stack_empty(MACRO_DEF_STACK, value_stack_free);
  return false;
}

void evalstack(contain_t *c, value_t *callword) {
  contain_t *cur = stack_peek(STACK);
  if (!cur)
    return;
  if (c->stack->size) {
    FAMILY->items[0] = cur;
    eval_value(c, cur, c->stack->items[0], callword);
    if (return_function(c, false))
      return;
    int(*cr)[2];
    for (int i = 1; i < c->stack->size; i++) {
      inc_crank(cur);
      cur = stack_peek(STACK);
      FAMILY->items[0] = cur;
      value_t *newval = c->stack->items[i];
      if (cur->cranks) {
        cr = cur->cranks->items[0];
        if (cr[0][0] == 0 && cr[0][1]) {
          eval_value(c, cur, newval, callword);
          if (return_function(c, false))
            return;
          continue;
        }
      }
      if (newval->type != VWORD) {
        push_quoted(cur, value_copy(newval));
        crank();
        if (return_function(c, false))
          return;
        continue;
      }
      bool evald = false;
      int family_idx = FAMILY_IDX->size - 1;
      for (int i = 0; i < FAMILY->size; i++) {
        contain_t *parent = FAMILY->items[i];
        if (family_idx >= 0) {
          contain_t **idx_ptr = FAMILY_IDX->items[family_idx];
          if (parent == *idx_ptr) {
            contain_t **new_ptr = FAMILY_IDX->items[family_idx - 1];
            i += new_ptr - idx_ptr + 1;
            family_idx -= 2;
          }
        }
        if (isfaliasin(parent, newval)) {
          evalf();
          evald = true;
          break;
        }
      }
      if (evald) {
        if (return_function(c, false))
          return;
        continue;
      }
      if (isfaliasin(c, newval)) {
        evalf();
      }
      if (return_function(c, false))
        return;
    }
  }
}

void evalmacro(stack_t *macro, value_t *word) {
  contain_t *old = stack_peek(STACK);
  value_t *v;
  for (int i = 0; i < macro->size; i++) {
    contain_t *cur = stack_peek(STACK);
    FAMILY->items[0] = cur;
    v = macro->items[i];
    switch (v->type) {
    case VCLIB:
      ((void (*)(value_t *))(v->custom))(word);
      break;
    case VSTACK:
      stack_push(cur->stack, value_copy(v));
      break;
    case VMACRO:
      stack_push(cur->stack, value_copy(v));
      break;
    case VWORD:
      evalword(v);
      break;
    default:
      push_quoted(cur, value_copy(v));
      break;
    }
    if (return_function(macro, true))
      return;
  }
}

void evalword(value_t *v) {
  contain_t *old = FAMILY->items[0];
  contain_t *expand;
  stack_t *macro;
  bool evald = false;
  int family_idx = FAMILY_IDX->size - 1;
  for (int i = FAMILY->size - 1; i >= 0; i--) {
    contain_t *parent = FAMILY->items[i];
    if (family_idx >= 0) {
      contain_t **idx_ptr = FAMILY_IDX->items[family_idx];
      if (parent == *idx_ptr) {
        contain_t **new_ptr = FAMILY_IDX->items[family_idx - 1];
        i += new_ptr - idx_ptr + 1;
        family_idx -= 2;
      }
    }

    if ((macro = ht_get(parent->flit, v->str_word))) {
      stack_push(MACROS, macro);
      if (i == 0) {
        stack_push(CONTAINERS, parent);
        evalmacro(macro, v);
        stack_pop(CONTAINERS);
      } else {
        evalmacro(macro, v);
      }
      stack_pop(MACROS);
      evald = true;
      break;
    } else if ((expand = ht_get(parent->word_table, v->str_word))) {
      stack_push(FAMILY_IDX, &FAMILY->items[i]);
      stack_push(FAMILY, expand);
      stack_push(FAMILY_IDX, &FAMILY->items[FAMILY->size - 1]);
      stack_push(CONTAINERS, expand);
      if (i == 0) {
        stack_push(CONTAINERS, parent);
        evalstack(expand, v);
        stack_pop(CONTAINERS);
      } else {
        evalstack(expand, v);
      }
      stack_pop(CONTAINERS);
      stack_pop(FAMILY);
      stack_pop(FAMILY_IDX);
      stack_pop(FAMILY_IDX);
      evald = true;
      break;
    } else if (isfaliasin(parent, v)) {
      if (old->cranks) {
        if (old->cranks->size) {
          int(*cr)[2] = old->cranks->items[0];
          if (cr[0][0] != 1 || cr[0][1] == 0) {
            evalf();
          }
        } else
          evalf();
      } else
        evalf();
      evald = true;
      break;
    }
  }
  if (!evald) {
    push_quoted(old, value_copy(v));
  }
}

void crank() {
  contain_t *cur = stack_peek(STACK);
  if (!cur)
    return;
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
      eval_error(U"CRANK TOO DEEP", NULL);
      inc_crank(cur);
      return;
    }
    stack_push(EVAL_STACK, needseval);
    if (needseval->type == VSTACK) {
      stack_push(FAMILY, needseval->container);
      evalstack(needseval->container, NULL);
      stack_pop(FAMILY);
    } else if (needseval->type == VMACRO) {
      evalmacro(needseval->macro, NULL);
    } else
      die("BAD VALUE ON STACK");
    inc_crank(cur);
    value_t *vf = stack_pop(EVAL_STACK);
    if (vf) {
      value_free_safe(vf);
    }
    return_function(NULL, false);
  } else {
    inc_crank(cur);
  }
}

void eval(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  if (!cur)
    return;
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
      evalf();
      return;
    }
  }
  push_quoted(cur, v);
  crank();
}
