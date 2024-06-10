#include <better_string.h>
#include <builtins.h>
#include <builtinslib.h>
#include <cognition.h>
#include <ctype.h>
#include <hash_table.h>
#include <macros.h>
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
pool_t *OBJ_POOL;
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

pool_t *init_pool() {
  pool_t *pool = calloc(1, sizeof(pool_t));
  return pool;
}

void *paw_alloc(size_t nmemb, size_t size) {
  void *c;
  c = calloc(nmemb, size);
  while (c == NULL) {
    if (!(OBJ_POOL->strstack || OBJ_POOL->vwordstack || OBJ_POOL->vstackstack
           || OBJ_POOL->vmacrostack || OBJ_POOL->vcustomstack || OBJ_POOL->vclibstack
           || OBJ_POOL->stackstack || OBJ_POOL->containstack->size != 0
           || OBJ_POOL->htstack->size != 0 || OBJ_POOL->errstack->size != 0))
      die("paw_alloc out of memory");
    pool_gc(OBJ_POOL);
    c = calloc(nmemb, size);
  }
  return c;
}

bst_t *free_bottom(bst_t *node, void (*freefunc)(void *)) {
  if (!node)
    return NULL;
  if (node->left)
    node->left = free_bottom(node->left, freefunc);
  if (node->right)
    node->right = free_bottom(node->right, freefunc);
  else if (node->left == NULL) {
    stack_free(node->value, freefunc);
    free(node);
    return NULL;
  }
  return node;
}

bst_t *bst_stack_add(bst_t *bst, long i, void *value) {
  if (!bst) {
    bst = init_bst();
    bst->ikey = i;
    bst->value = init_stack(DEFAULT_STACK_SIZE);
    stack_push(bst->value, value);
    return bst;
  }
  bst_t *parent = NULL;
  bst_t *cur = bst;
  bool isleft = true;
  long l;
  while (cur) {
    l = i - cur->ikey;
    if (l < 0) {
      parent = cur;
      cur = cur->left;
      isleft = true;
    } else if (l > 0) {
      parent = cur;
      cur = cur->right;
      isleft = false;
    } else {
      stack_push(cur->value, value);
      return bst;
    }
  }
  if (isleft) {
    parent->left = init_bst();
    parent->left->ikey = i;
    parent->left->value = init_stack(DEFAULT_STACK_SIZE);
    stack_push(parent->left->value, value);
  } else {
    parent->right = init_bst();
    parent->right->ikey = i;
    parent->right->value = init_stack(DEFAULT_STACK_SIZE);
    stack_push(parent->right->value, value);
  }
  return bst;
}

void bst_stack_free(bst_t *bst, void (*freefunc)(void *)) {
  if (!bst)
    return;
  if (bst->left)
    bst_stack_free(bst->left, freefunc);
  if (bst->right)
    bst_stack_free(bst->right, freefunc);
  if (bst->value) {
    stack_t *s = bst->value;
    stack_free(s, freefunc);
  }
  free(bst);
}

void pool_gc(pool_t *pool) {
  pool->strstack = free_bottom(pool->strstack, string_free);
  pool->vwordstack = free_bottom(pool->vwordstack, value_free);
  pool->vstackstack = free_bottom(pool->vstackstack, value_free);
  pool->vmacrostack = free_bottom(pool->vmacrostack, value_free);
  pool->vcustomstack = free_bottom(pool->vcustomstack, value_free);
  pool->vclibstack = free_bottom(pool->vclibstack, value_free);
  pool->stackstack = free_bottom(pool->stackstack, value_stack_free);
}

void pool_gcall(pool_t *pool) {
  bst_stack_free(pool->strstack, string_free);
  bst_stack_free(pool->vwordstack, value_free);
  bst_stack_free(pool->vstackstack, value_free);
  bst_stack_free(pool->vmacrostack, value_free);
  bst_stack_free(pool->vcustomstack, value_free);
  bst_stack_free(pool->vclibstack, value_free);
  bst_stack_free(pool->stackstack, value_stack_free);

  stack_free(pool->vcustomstack, value_free);
  stack_free(pool->htstack, empty_ht_free);
  stack_free(pool->errstack, error_free);
}

void empty_bst(bst_t *bst, pool_t *pool, byte_t type) {
  if (!bst) return;
  empty_bst(bst->left, pool, type);
  pool->strstack = bst_stack_add(pool->strstack, bst->key->bufsize, bst->key);
  pool_add(pool, type, bst->value);
  empty_bst(bst->right, pool, type);
  free(bst);
}

void pool_empty_contain(pool_t *pool, contain_t *c) {
  if (c->err_stack) {
    for (int i = 0; i < c->err_stack->size; i++) {
      pool_add(pool, POOL_ERR, c->err_stack->items[i]);
      c->err_stack->items[i] = NULL;
    }
    c->err_stack->size = 0;
  }
  if (c->delims) {
    pool_add(pool, POOL_STRING, c->delims);
    c->delims = NULL;
  }
  if (c->ignored) {
    pool_add(pool, POOL_STRING, c->ignored);
    c->ignored = NULL;
  }
  if (c->singlets) {
    pool_add(pool, POOL_STRING, c->singlets);
    c->singlets = NULL;
  }
  if (c->flit) {
    pool_add(pool, POOL_FLIT, c->flit);
    c->flit = NULL;
  }
  if (c->word_table) {
    pool_add(pool, POOL_WT, c->word_table);
    c->word_table = NULL;
  }
  if (c->cranks) {
    for (int i = 0; i < c->cranks->size; i++) {
      free(c->cranks->items[i]);
    }
    c->cranks->size = 0;
  }
}

void pool_add(pool_t *pool, byte_t type, void *value) {
  switch (type) {
    case POOL_STRING:;
      string_t *str = value;
      pool->strstack = bst_stack_add(pool->strstack, str->bufsize, str);
      break;
    case POOL_VWORD:;
      value_t *vword = value;
      pool->vwordstack = bst_stack_add(pool->vwordstack, vword->str_word->bufsize, vword);
      break;
    case POOL_VSTACK:;
      value_t *vstack = value;
      pool->vstackstack = bst_stack_add(pool->vstackstack, vstack->container->stack->capacity, vword);
      break;
    case POOL_VMACRO:;
      value_t *vmacro = value;
      pool->vmacrostack = bst_stack_add(pool->vmacrostack, vmacro->macro->capacity, vmacro);
      break;
    case POOL_VCUSTOM:;
      value_t *vcustom = value;
      pool->vcustomstack = bst_stack_add(pool->vcustomstack, vcustom->str_word->bufsize, vcustom);
      break;
    case POOL_VCLIB:;
      value_t *vclib = value;
      long l = 0;
      if (vclib->str_word)
        l = vclib->str_word->bufsize;
      pool->vclibstack = bst_stack_add(pool->vclibstack, l, vclib);
      break;
    case POOL_STACK:;
      stack_t *s = value;
      pool->stackstack = bst_stack_add(pool->stackstack, s->capacity, s);
      break;
    case POOL_CONTAIN:;
      contain_t *c = value;
      pool_empty_contain(pool, c);
      pool->stackstack = bst_stack_add(pool->stackstack, c->stack->capacity, c->stack);
      stack_push(pool->containstack, c);
      break;
    case POOL_WT:;
      ht_t *wt = value;
      for (long int i = 0; i < wt->size; i++) {
        empty_bst(wt->buckets[i], pool, POOL_CONTAIN);
        wt->buckets[i] = NULL;
      }
      stack_push(pool->htstack, wt);
      break;
    case POOL_FLIT:;
      ht_t *flit = value;
      for (long int i = 0; i < flit->size; i++) {
        empty_bst(flit->buckets[i], pool, POOL_STACK);
        flit->buckets[i] = NULL;
      }
      stack_push(pool->htstack, flit);
      break;
    case POOL_ERR:;
      stack_push(pool->errstack, value);
      break;
    case POOL_VALUE:;
      value_t *val = value;
      switch (val->type) {
        case VWORD:
          pool_add(pool, POOL_VWORD, val);
          break;
        case VSTACK:
          pool_add(pool, POOL_VSTACK, val);
          break;
        case VMACRO:
          pool_add(pool, POOL_VMACRO, val);
          break;
        case VERR:
          printf("MYSTERIOUS UCCURENCE OF VERR\n");
          exit(-1);
          break;
        case VCUSTOM:
          pool_add(pool, POOL_VCUSTOM, val);
          break;
        case VCLIB:
          pool_add(pool, POOL_VCLIB, val);
          break;
      }
      break;
  }
}

void *bst_pop_min(bst_t *bst, long i) {
  if (!bst)
    return NULL;
  if (bst->ikey >= i) {
    void *leftmin = bst_pop_min(bst->left, i);
    if (leftmin != NULL)
      return leftmin;
    if (bst->ikey >= i)
      return stack_pop(bst->value);
  }
  return bst_pop_min(bst->right, i);
}

void pool_empty_stack(pool_t *pool, stack_t *stack) {
  for (long i = 0; i < stack->size; i++)
    if (stack->items[i])
      pool_add(pool, POOL_VALUE, stack->items[i]);
  stack->size = 0;
}

void *pool_get(pool_t *pool, long bufsize, byte_t type) {
  switch (type) {
    case POOL_STRING:;
      string_t *str = bst_pop_min(pool->strstack, bufsize);
      str->length = 0;
      return str;
    case POOL_VWORD:;
      value_t *vword = bst_pop_min(pool->vwordstack, bufsize);
      vword->str_word->length = 0;
      return vword;
    case POOL_VSTACK:;
      value_t *vstack = bst_pop_min(pool->vstackstack, bufsize);
      pool_empty_contain(pool, vstack->container);
      pool_empty_stack(pool, vstack->container->stack);
      return vstack;
    case POOL_VMACRO:;
      value_t *vmacro = bst_pop_min(pool->vmacrostack, bufsize);
      pool_empty_stack(pool, vmacro->macro);
      return vmacro;
    case POOL_VCUSTOM:;
      value_t *vcustom = bst_pop_min(pool->vcustomstack, bufsize);
      custom_t *cstm = ht_get(OBJ_TABLE, vcustom->str_word);
      cstm->freefunc(vcustom->custom);
      vcustom->str_word->length = 0;
      return vcustom;
    case POOL_VCLIB:;
      value_t *vclib = bst_pop_min(pool->vclibstack, bufsize);
      if (vclib->str_word)
        vclib->str_word->length = 0;
      return vclib;
    case POOL_STACK:;
      stack_t *stack = bst_pop_min(pool->stackstack, bufsize);
      pool_empty_stack(pool, stack);
      return stack;
    case POOL_CONTAIN:;
      contain_t *contain = stack_pop(pool->containstack);
      pool_empty_contain(pool, contain);
      contain->stack = bst_pop_min(pool->stackstack, bufsize);
      pool_empty_stack(pool, contain->stack);
      return contain;
    case POOL_HT:;
      return stack_pop(pool->htstack);
    case POOL_ERR:;
      return stack_pop(pool->errstack);
  }
}

void *pool_req(long bufsize, byte_t type) {
  void *v = pool_get(OBJ_POOL, bufsize, type);
  if (!v) {
    long buf;
    switch (type) {
      case POOL_STRING:;
        buf = DEFAULT_STRING_LENGTH;
        while (buf < bufsize)
          buf *= 2;
        return alloc_string(buf);
      case POOL_VWORD:;
        value_t *vword = init_value(VWORD);
        vword->str_word = pool_req(bufsize, POOL_STRING);
        return vword;
      case POOL_VSTACK:;
        value_t *vstack = init_value(VSTACK);
        vstack->container = pool_req(bufsize, POOL_CONTAIN);
        return vstack;
      case POOL_VMACRO:;
        value_t *vmacro = init_value(VMACRO);
        vmacro->macro = pool_req(bufsize, POOL_STACK);
        return vstack;
      case POOL_VCUSTOM:;
        value_t *vcustom = init_value(VCUSTOM);
        vcustom->str_word = pool_req(bufsize, POOL_STRING);
        return vcustom;
      case POOL_VCLIB:;
        value_t *vclib = init_value(VCLIB);
        vclib->str_word = pool_req(bufsize, POOL_STRING);
        return vclib;
      case POOL_STACK:;
        buf = DEFAULT_STACK_SIZE;
        while (buf < bufsize)
          buf *= 2;
        return init_stack(buf);
      case POOL_CONTAIN:;
        contain_t *contain = calloc(1, sizeof(contain_t));
        contain->stack = pool_req(bufsize, POOL_STACK);
        return contain;
      case POOL_HT:
        return init_ht(DEFAULT_HT_SIZE);
      case POOL_ERR:
        return calloc(1, sizeof(error_t));
      default:
        return NULL;
    }
  }
  if (type == POOL_CONTAIN) {
    contain_t *c = v;
    if (c->stack == NULL) {
      long buf = DEFAULT_STACK_SIZE;
        while (buf < bufsize)
          buf *= 2;
      c->stack = init_stack(buf);
    }
  }
  return v;
}

void pool_free(pool_t *pool) {
  pool_gcall(pool);
  free(pool);
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
  value_t *v1 = v;
  contain_t *container = stack_peek(STACK);
  value_t *a = init_value(v1->type);
  if (v1->type == VWORD) {
    a->str_word = string_copy(v1->str_word);
  } else if (v1->type == VSTACK) {
    a->container = contain_copy(v1->container, value_copy);
  } else if (v1->type == VMACRO) {
    a->macro = value_stack_copy(v1->macro);
  } else if (v1->type == VERR) {
    a->error = error_copy(v1->error);
  }else if (v1->type == VCLIB) {
    a->str_word = string_copy(v1->str_word);
    a->custom = v1->custom;
  } else if (v1->type == VCUSTOM) {
    custom_t *c = ht_get(OBJ_TABLE, v1->str_word);
    a->custom = c->copyfunc(v1->custom);
    a->str_word = string_copy(v1->str_word);
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
  error_t *en = calloc(1, sizeof(error_t));
  en->str_word = string_copy(e->str_word);
  en->error = string_copy(e->error);
  return en;
}

custom_t *init_custom(void (*printfunc)(FILE *, void *), void (*freefunc)(void *),
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
  stack_t *macro = init_stack(DEFAULT_STACK_SIZE);
  value_t *v = init_value(VCLIB);
  v->str_word = init_string(key);
  v->custom = func;
  stack_push(macro, v);
  ht_add(h, init_string(key), macro, value_stack_free);
}

void add_macro(ht_t *h, stack_t *macro, char32_t *key) {
  ht_add(h, init_string(key), macro, value_stack_free);
}

void add_obj(ht_t *h, void (*printfunc)(FILE *, void *), void (*freefunc)(void *),
             void *(*copyfunc)(void *), char32_t *key) {

  custom_t *c = init_custom(printfunc, freefunc, copyfunc);
  ht_add(h, init_string(key), c, custom_free);
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
        cr = cur->cranks->items[0];
        if (cr[0][0] == 0 && cr[0][1]) {
          eval_value(c, cur, (value_t **)&c->stack->items[i], callword, false, destructive);
          if (return_function(c, false))
            return;
          continue;
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
        contain_free(stack_popdeep(EVAL_CONTAIN_TRASH, i));
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
      value_free(v);
      return;
    }
    if (cur->cranks->size == 0) {
      evalf(v);
      value_free(v);
      return;
    }
    int(*crank)[2] = cur->cranks->items[0];
    if ((crank[0][0] != 1 || crank[0][1] == 0) && crank[0][1] != 1) {
      evalf(v);
      value_free(v);
      return;
    }
    value_free(v);
    return;
  }
  push_quoted(cur, v);
  crank();
}
