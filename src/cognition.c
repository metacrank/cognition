#include <better_string.h>
#include <builtins.h>
#include <builtinslib.h>
#include <cognition.h>
#include <ctype.h>
#include <hash_table.h>
#include <macros.h>
#include <pool.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Global variables defined */
stack_t *STACK;
stack_t *EVAL_CONTAINERS;
stack_t *EVAL_CONTAIN_TRASH;
stack_t *CONTAIN_DEF_STACK;
stack_t *MACRO_DEF_STACK;
stack_t *FAMILY;
string_t *FAMILY_IDX;
int FAMILY_RECURSION_DEPTH;
stack_t *CONTAINERS;
stack_t *MACROS;
stack_t *OBJ_TABLE_STACK;
stack_t *OBJ_TABLE_REF_STACK;
parser_t *PARSER;
string_t *EXIT_CODE;
bool EXITED;
bool RETURNED;
string_t *ROOT;
stack_t *ARGS;

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
  value_t *v = pool_req(0, POOL_VERR);
  if (w)
    v->error->str_word = string_copy(w->str_word);
  else
    v->error->str_word = NULL;
  v->error->error = pool_req(DEFAULT_STRING_LENGTH, POOL_STRING);
  string_append_all(v->error->error, s);
  contain_t *cur = stack_peek(STACK);
  if (cur->err_stack == NULL)
    cur->err_stack = pool_req(DEFAULT_STACK_SIZE, POOL_STACK);
  stack_push(cur->err_stack, v);
}

stack_t *init_stack(size_t size) {
  stack_t *a = paw_alloc(1, sizeof(stack_t));
  a->size = 0;
  a->capacity = size;
  a->items = paw_alloc(a->capacity, sizeof(value_t *));
  return a;
}

void realloc_stack(stack_t *a, size_t capacity) {
  stack_t *s = pool_req(capacity, POOL_STACK);
  memcpy(s->items, a->items, a->size * sizeof(value_t *));
  void *items = a->items;
  a->items = s->items;
  s->items = items;
  size_t cap = a->capacity;
  a->capacity = s->capacity;
  s->capacity = cap;
  pool_addobj(POOL_STACK, s);
}

void stack_push(stack_t *a, void *v) {
  if (a == NULL)
    return;
  if (a->size >= a->capacity - 2)
    realloc_stack(a, a->capacity * 2);
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

void value_stack_pool_add(void *a) { pool_addobj(POOL_STACK, a); }

void *stack_copy(void *a, void *(*copyfunc)(void *)) {
  if (a == NULL) {
    return NULL;
  }
  stack_t *s = a;
  stack_t *b = pool_req(s->size, POOL_STACK);
  b->size = s->size;
  for (int i = 0; i < s->size; i++) {
    b->items[i] = copyfunc(s->items[i]);
  }
  return b;
}

void *value_stack_copy(void *a) { return stack_copy(a, value_copy); }

value_t *init_value(int type) {
  value_t *v = paw_alloc(1, sizeof(value_t));
  v->type = type;
  return v;
}

void *value_copy(void *v) {
  if (v == NULL)
    return NULL;
  value_t *v1 = v;
  value_t *a;
  if (v1->type == VWORD || v1->type == VCLIB || v1->type == VCUSTOM) {
    a = pool_req(v1->str_word->length, val2pool_type(v));
    string_copy_buffer(v1->str_word, a->str_word);
    if (v1->type == VCLIB)
      a->custom = v1->custom;
    else if (v1->type == VCUSTOM) {
      custom_t *c = ht_get(OBJ_TABLE, v1->str_word);
      a->custom = c->copyfunc(v1->custom);
    }
  } else if (v1->type == VSTACK) {
    a = pool_req(v1->container->stack->size, POOL_VSTACK);
    contain_clone(v1->container, a->container);
  } else if (v1->type == VMACRO) {
    a = pool_req(v1->macro->size, POOL_VMACRO);
    for (long i = 0; i < v1->macro->size; i++) {
      a->macro->items[i] = value_copy(v1->macro->items[i]);
    }
    a->macro->size = v1->macro->size;
  } else {
    a = pool_req(0, POOL_VERR);
    a->error->str_word = string_copy(v1->error->str_word);
    a->error->error = string_copy(v1->error->error);
  }
  return a;
}

void value_free(void *vtmp) {
  if (vtmp == NULL)
    return;
  value_t *v = vtmp;
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

error_t *error_copy(void *err) {
  if (err == NULL)
    return NULL;
  error_t *e = err;
  error_t *en = paw_alloc(1, sizeof(error_t));
  en->str_word = string_copy(e->str_word);
  en->error = string_copy(e->error);
  return en;
}

custom_t *init_custom(void (*printfunc)(FILE *, void *), void (*freefunc)(void *),
                      void *(*copyfunc)(void *)) {
  custom_t *c = paw_alloc(1, sizeof(custom_t));
  c->printfunc = printfunc;
  c->freefunc = freefunc;
  c->copyfunc = copyfunc;
  return c;
}

void custom_free(void *c) { free(c); }

void add_func(ht_t *h, void (*func)(value_t *), char32_t *key) {
  stack_t *macro = pool_req(DEFAULT_STACK_SIZE, POOL_STACK);
  value_t *v = pool_req(DEFAULT_STRING_LENGTH, POOL_VCLIB);
  string_append_all(v->str_word, key);
  v->custom = func;
  stack_push(macro, v);
  ht_add(h, init_string(key), macro, value_stack_pool_add);
}

void add_macro(ht_t *h, stack_t *macro, char32_t *key) {
  ht_add(h, init_string(key), macro, value_stack_pool_add);
}

void add_obj(ht_t *h, void (*printfunc)(FILE *, void *), void (*freefunc)(void *),
             void *(*copyfunc)(void *), char32_t *key) {

  custom_t *c = init_custom(printfunc, freefunc, copyfunc);
  ht_add(h, init_string(key), c, custom_free);
}

contain_t *init_contain(ht_t *h, ht_t *flit, stack_t *cranks) {
  contain_t *c = paw_alloc(1, sizeof(contain_t));
  c->stack = pool_req(DEFAULT_STACK_SIZE, POOL_STACK);
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

void contain_pool_add(void *con) {
  if (con == NULL)
    return;
  pool_addobj(POOL_CONTAIN, con);
}

void *contain_value_copy(void *c) { return contain_copy(c, value_copy); }

void *falias_copy(void *f) { return string_copy(f); }

contain_t *contain_copy(contain_t *c, void *(*copyfunc)(void *)) {
  if (c == NULL) {
    return NULL;
  }
  contain_t *contain = paw_alloc(1, sizeof(contain_t));
  contain->word_table = ht_copy(c->word_table, contain_value_copy);
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

void *contain_new_clone(void *c) {
  if (c == NULL)
    return NULL;
  contain_t *con = c;
  contain_t *newc = pool_req(con->stack->size, POOL_CONTAIN);
  contain_clone(con, newc);
  return newc;
}

void contain_clone(contain_t *c, contain_t *newc) {
  if (c == NULL)
    return;
  newc->word_table = ht_copy(c->word_table, contain_new_clone);
  newc->flit = ht_copy(c->flit, value_stack_copy);
  newc->cranks = stack_copy(c->cranks, cranks_copy);
  if (newc->err_stack && c->err_stack)
    for (long i = 0; i < c->err_stack->size; i++)
      stack_push(newc->err_stack, value_copy(c->err_stack->items[i]));
  else if (c->err_stack)
    newc->err_stack = stack_copy(c->err_stack, value_copy);
  for (long i = 0; i < c->stack->size; i++) {
    newc->stack->items[i] = value_copy(c->stack->items[i]);
  }
  newc->stack->size = c->stack->size;
  if (newc->faliases && c->faliases) {
    for (long i = 0; i < c->faliases->size; i++)
      stack_push(newc->faliases, string_copy(c->faliases->items[i]));
  }
  newc->delims = string_copy(c->delims);
  newc->singlets = string_copy(c->singlets);
  newc->ignored = string_copy(c->ignored);
  newc->dflag = c->dflag;
  newc->iflag = c->iflag;
  newc->sflag = c->sflag;
}

parser_t *init_parser(string_t *source) {
  parser_t *p = paw_alloc(1, sizeof(parser_t));
  p->i = 0;
  p->source = source;
  if (source)
    if (source->length)
      p->c = source->value[0];
  return p;
}

void parser_reset(parser_t *p, string_t *source) {
  p->source = source;
  p->i = 0;
  p->c = source->value[0];
}

void parser_move(parser_t *p) {
  p->i++;
  if (p->i < p->source->length) {
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
  value_t *retval = pool_req(DEFAULT_STRING_LENGTH, POOL_VWORD);
  string_t *strval = retval->str_word;
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

void evalf(value_t *alias) {
  contain_t *cur = stack_peek(STACK);
  value_t *v = stack_pop(cur->stack);
  if (v == NULL) {
    eval_error(U"EMPTY STACK", alias);
    return;
  }
  stack_push(EVAL_CONTAINERS, cur);
  FAMILY_RECURSION_DEPTH++;
  if (v->type == VSTACK) {
    stack_push(FAMILY, v->container);
    evalstack(v->container, NULL, true);
    stack_pop(FAMILY);
  } else if (v->type == VMACRO) {
    evalmacro(v->macro, NULL, true);
  } else
    die("BAD VALUE TYPE ON STACK");
  inc_crank(cur);
  FAMILY_RECURSION_DEPTH--;
  stack_pop(EVAL_CONTAINERS);
  for (int i = 0; i < EVAL_CONTAIN_TRASH->size; i++) {
    if (!stack_exists(EVAL_CONTAINERS, EVAL_CONTAIN_TRASH->items[i])) {
      contain_free(stack_popdeep(EVAL_CONTAIN_TRASH, i));
      i--;
    }
  }
  value_free_safe(v);
  return_function(cur, false);
}

void *func_copy(void *funcs) { return NULL; }

void *cranks_copy(void *cranks) {
  int(*cr)[2] = cranks;
  int(*arr)[2] = paw_alloc(1, sizeof(int[2]));
  arr[0][0] = cr[0][0];
  arr[0][1] = cr[0][1];
  return arr;
}

void push_quoted(contain_t *cur, value_t *v) {
  value_t *q = pool_req(DEFAULT_STACK_SIZE, POOL_VSTACK);
  stack_push(q->container->stack, v);
  stack_push(cur->stack, q);
}

void eval_value(contain_t *c, contain_t *cur, value_t **val, value_t *callword, bool always_evalf, bool destructive) {
  value_t *vl = *val;
  switch (vl->type) {
  case VWORD:
    evalword(vl, always_evalf);
    break;
  case VSTACK:
    if (destructive) {
      stack_push(cur->stack, vl);
      *val = NULL;
    } else {
      stack_push(cur->stack, value_copy(vl));
    }
    break;
  case VMACRO:
    if (destructive) {
      stack_push(cur->stack, vl);
      *val = NULL;
    } else {
      stack_push(cur->stack, value_copy(vl));
    }
    break;
  case VCLIB:
    ((void (*)(value_t * v))(vl->custom))(callword);
    break;
  default:
    if (destructive) {
      push_quoted(cur, vl);
      *val = NULL;
    } else {
      push_quoted(cur, value_copy(vl));
    }
  }
}

bool return_function(void *stack, bool macro) {
  if (EXITED)
    return true;
  for (int i = 0; i < CONTAIN_DEF_STACK->size; i++) {
    if (stack_exists(EVAL_CONTAINERS, CONTAIN_DEF_STACK->items[i])) {
      stack_push(EVAL_CONTAIN_TRASH, stack_popdeep(CONTAIN_DEF_STACK, i));
      i--;
    }
  }
  if (macro) {
    if (stack_exists(MACRO_DEF_STACK, stack)) {
      return true;
    }
  } else if (stack_exists(CONTAIN_DEF_STACK, stack)) { // && stack->def == true
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
  //TODO: successfully implement contain_pool_add here
  stack_empty(CONTAIN_DEF_STACK, contain_free);
  stack_empty(MACRO_DEF_STACK, value_stack_pool_add);
  return false;
}

void evalstack(contain_t *c, value_t *callword, bool destructive) {
  contain_t *cur = stack_peek(STACK);
  value_t *newval;
  bool evald = false;
  if (!cur)
    return;
  if (c->stack->size) {
    FAMILY->items[0] = cur;
    eval_value(c, cur, (value_t **)&c->stack->items[0], callword, true, destructive);
    if (return_function(c, false))
      return;
    int(*cr)[2];
    for (int i = 1; i < c->stack->size; i++) {
      if (RETURNED) {
        return;
      }
      inc_crank(cur);
      cur = stack_peek(STACK);
      FAMILY->items[0] = cur;
      newval = c->stack->items[i];
      if (cur->cranks) {
        if (cur->cranks->size) {
          cr = cur->cranks->items[0];
          if (cr[0][0] == 0 && cr[0][1]) {
            eval_value(c, cur, (value_t **)&c->stack->items[i], callword, false, destructive);
            if (return_function(c, false))
              return;
            continue;
          }
        }
      }
      if (newval->type != VWORD) {
        if (destructive) {
          push_quoted(cur, newval);
          c->stack->items[i] = NULL;
        } else {
          push_quoted(cur, value_copy(newval));
        }
        crank();
        dec_crank(cur);
        if (return_function(c, false))
          return;
        continue;
      }
      evald = false;
      stack_push(FAMILY, c);
      int family_idx = FAMILY_IDX->length - 1;
      for (int i = FAMILY->size - 1; i >= 0; i--) {
        contain_t *parent = FAMILY->items[i];
        if (family_idx >= 0) {
          if (i == FAMILY_IDX->value[family_idx]) {
            i = FAMILY_IDX->value[family_idx - 1] + 1;
            family_idx -= 2;
          }
        }
        if (parent == NULL) continue;
        if (isfaliasin(parent, newval)) {
          if (cur->cranks) {
            if (cur->cranks->size) {
              int(*cr)[2] = cur->cranks->items[0];
              if ((cr[0][0] != 1 || cr[0][1] == 0) && cr[0][1] != 1) {
                evalf(newval);
              }
            } else
              evalf(newval);
          } else
            evalf(newval);
          evald = true;
          break;
        }
      }
      stack_pop(FAMILY);
      if (!evald) {
        if (destructive) {
          push_quoted(cur, newval);
          c->stack->items[i] = NULL;
        } else {
          push_quoted(cur, value_copy(newval));
        }
        crank();
        dec_crank(cur);
      }
      if (return_function(c, false))
        return;
    }
  }
}

void evalmacro(stack_t *macro, value_t *word, bool destructive) {
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
      if (destructive) {
        stack_push(cur->stack, v);
        macro->items[i] = NULL;
      } else {
        stack_push(cur->stack, value_copy(v));
      }
      break;
    case VMACRO:
      if (destructive) {
        stack_push(cur->stack, v);
        macro->items[i] = NULL;
      } else {
        stack_push(cur->stack, value_copy(v));
      }
      break;
    case VWORD:
      //evalword(v, i == 0);
      evalword(v, true);
      break;
    default:
      if (destructive) {
        push_quoted(cur, v);
        macro->items[i] = NULL;
      } else {
        push_quoted(cur, value_copy(v));
      }
      break;
    }
    if (return_function(macro, true))
      return;
    if (RETURNED)
      return;
  }
}

void evalword(value_t *v, bool always_evalf) {
  contain_t *old = FAMILY->items[0];
  contain_t *expand;
  stack_t *macro;
  bool evald = false;
  int family_idx = FAMILY_IDX->length - 1;
  for (int i = FAMILY->size - 1; i >= 0; i--) {
    contain_t *parent = FAMILY->items[i];
    if (family_idx >= 0) {
      if (i == FAMILY_IDX->value[family_idx]) {
        i = FAMILY_IDX->value[family_idx - 1] + 1;
        family_idx -= 2;
      }
    }
    if (parent == NULL) continue;
    if ((macro = ht_get(parent->flit, v->str_word))) {
      if (FAMILY_RECURSION_DEPTH >= 13824) {
        eval_error(U"MAXIMUM RECURSION DEPTH REACHED", v);
        return;
      }
      stack_push(MACROS, macro);
      FAMILY_RECURSION_DEPTH++;
      if (i == 0) {
        stack_push(CONTAINERS, parent);
        evalmacro(macro, v, false);
        stack_pop(CONTAINERS);
      } else {
        evalmacro(macro, v, false);
      }
      FAMILY_RECURSION_DEPTH--;
      stack_pop(MACROS);
      evald = true;
      if (RETURNED)
        RETURNED = false;
      break;
    } else if ((expand = ht_get(parent->word_table, v->str_word))) {
      if (FAMILY_RECURSION_DEPTH >= 13824) {
        eval_error(U"MAXIMUM RECURSION DEPTH REACHED", v);
        return;
      }
      string_append(FAMILY_IDX, i);
      stack_push(FAMILY, expand);
      string_append(FAMILY_IDX, FAMILY->size - 1);
      stack_push(CONTAINERS, expand);
      FAMILY_RECURSION_DEPTH++;
      if (i == 0) {
        stack_push(CONTAINERS, parent);
        evalstack(expand, v, false);
        stack_pop(CONTAINERS);
      } else {
        evalstack(expand, v, false);
      }
      FAMILY_RECURSION_DEPTH--;
      stack_pop(CONTAINERS);
      stack_pop(FAMILY);
      FAMILY_IDX->length -= 2;
      evald = true;
      if (RETURNED)
        RETURNED = false;
      break;
    } else if (isfaliasin(parent, v)) {
      if (always_evalf)
        evalf(v);
      else {
        if (old->cranks) {
          if (old->cranks->size) {
            int(*cr)[2] = old->cranks->items[0];
            if ((cr[0][0] != 1 || cr[0][1] == 0) && cr[0][1] != 1) {
              evalf(v);
            }
          } else
            evalf(v);
        } else
          evalf(v);
      }
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
    stack_push(EVAL_CONTAINERS, cur);
    FAMILY_RECURSION_DEPTH++;
    if (needseval->type == VSTACK) {
      stack_push(FAMILY, needseval->container);
      evalstack(needseval->container, NULL, true);
      stack_pop(FAMILY);
    } else if (needseval->type == VMACRO) {
      evalmacro(needseval->macro, NULL, true);
    } else
      die("BAD VALUE ON STACK");
    inc_crank(cur);
    FAMILY_RECURSION_DEPTH--;
    stack_pop(EVAL_CONTAINERS);
    for (int i = 0; i < EVAL_CONTAIN_TRASH->size; i++) {
      if (!stack_exists(EVAL_CONTAINERS, EVAL_CONTAIN_TRASH->items[i])) {
        contain_pool_add(stack_popdeep(EVAL_CONTAIN_TRASH, i));
        i--;
      }
    }
    value_free_safe(needseval);
    return_function(cur, false);
  } else {
    inc_crank(cur);
  }
}

void eval(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  if (!cur)
    return;
  if (isfalias(v)) {
    if (!cur->cranks) {
      evalf(v);
      pool_addobj(POOL_VWORD, v);
      return;
    }
    if (cur->cranks->size == 0) {
      evalf(v);
      pool_addobj(POOL_VWORD, v);
      return;
    }
    int(*crank)[2] = cur->cranks->items[0];
    if ((crank[0][0] != 1 || crank[0][1] == 0) && crank[0][1] != 1) {
      evalf(v);
      pool_addobj(POOL_VWORD, v);
      return;
    }
    pool_addobj(POOL_VWORD, v);
    return;
  }
  push_quoted(cur, v);
  crank();
}
