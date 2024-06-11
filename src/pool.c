#include <pool.h>
#include <hash_table.h>
#include <cognition.h>
#include <macros.h>

pool_t *OBJ_POOL;
extern stack_t *OBJ_TABLE_STACK;
extern string_t *ROOT;

pool_t *init_pool() {
  pool_t *pool = calloc(1, sizeof(pool_t));
  pool->containstack = init_stack(DEFAULT_STACK_SIZE);
  pool->htstack = init_stack(DEFAULT_STACK_SIZE);
  pool->verrstack = init_stack(DEFAULT_STACK_SIZE);
  return pool;
}

void *paw_alloc(size_t nmemb, size_t size) {
  void *c;
  c = calloc(nmemb, size);
  while (c == NULL) {
    if (!(OBJ_POOL->strstack || OBJ_POOL->vwordstack || OBJ_POOL->vstackstack
           || OBJ_POOL->vmacrostack || OBJ_POOL->vcustomstack || OBJ_POOL->vclibstack
           || OBJ_POOL->stackstack || OBJ_POOL->containstack->size != 0
           || OBJ_POOL->htstack->size != 0 || OBJ_POOL->verrstack->size != 0))
      die("paw_alloc out of memory");
    pool_gc(OBJ_POOL);
    c = calloc(nmemb, size);
  }
  return c;
}

bst_t *free_bottom(bst_t *node, void (*freefunc)(void *)) {
  if (!node)
    return NULL;
  bst_t *left = node->left;
  if (node->left)
    node->left = free_bottom(node->left, freefunc);
  if (node->right)
    node->right = free_bottom(node->right, freefunc);
  else if (left == NULL) {
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
    bst->value = pool_req(DEFAULT_STACK_SIZE, POOL_STACK);
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
  stack_t *s = pool_req(DEFAULT_STACK_SIZE, POOL_STACK);
  if (isleft) {
    parent->left = init_bst();
    parent->left->ikey = i;
    parent->left->value = s;
    stack_push(parent->left->value, value);
  } else {
    parent->right = init_bst();
    parent->right->ikey = i;
    parent->right->value = s;
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

void pool_contain_free(void *con) {
  if (con == NULL)
    return;
  contain_t *c = con;
  ht_free(c->word_table, contain_free);
  ht_free(c->flit, value_stack_free);
  stack_free(c->err_stack, value_free);
  stack_free(c->cranks, free);
  stack_free(c->faliases, (void (*)(void *))string_free);
  string_free(c->delims);
  string_free(c->ignored);
  string_free(c->singlets);
  free(c);
}

void pool_gcall(pool_t *pool) {
  bst_stack_free(pool->strstack, string_free);
  bst_stack_free(pool->vwordstack, value_free);
  bst_stack_free(pool->vstackstack, value_free);
  bst_stack_free(pool->vmacrostack, value_free);
  bst_stack_free(pool->vcustomstack, value_free);
  bst_stack_free(pool->vclibstack, value_free);
  bst_stack_free(pool->stackstack, value_stack_free);

  stack_free(pool->containstack, pool_contain_free);
  stack_free(pool->htstack, empty_ht_free);
  stack_free(pool->verrstack, value_free);
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
      pool_add(pool, POOL_VERR, c->err_stack->items[i]);
      c->err_stack->items[i] = NULL;
    }
    c->err_stack->size = 0;
  }
  if (c->faliases) {
    for (int i = 0; i < c->faliases->size; i++) {
      pool_add(pool, POOL_STRING, c->faliases->items[i]);
      c->faliases->items[i] = NULL;
    }
    c->faliases->size = 0;
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
  c->dflag = false;
  c->iflag = true;
  c->sflag = true;
}

void pool_add(pool_t *pool, byte_t type, void *value) {
  switch (type) {
    case POOL_STRING:;
      string_t *str = value;
      pool->strstack = bst_stack_add(pool->strstack, str->bufsize, str);
      break;
    case POOL_VWORD:;
      value_t *vword = value;
      if (vword->str_word)
        pool->vwordstack = bst_stack_add(pool->vwordstack, vword->str_word->bufsize, vword);
      else
        pool->vwordstack = bst_stack_add(pool->vwordstack, DEFAULT_STRING_LENGTH, vword);
      break;
    case POOL_VSTACK:;
      value_t *vstack = value;
      pool->vstackstack = bst_stack_add(pool->vstackstack, vstack->container->stack->capacity, vstack);
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
      if (c->stack)
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
    case POOL_VERR:;
      stack_push(pool->verrstack, value);
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
          pool_add(pool, POOL_VERR, val);
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

void pool_addobj(byte_t type, void *value) {
  pool_add(OBJ_POOL, type, value);
}

void *bst_pop_min(pool_t *pool, bst_t **bst, long i) {
  bst_t *b = *bst;
  if (!b)
    return NULL;
  if (b->ikey >= i) {
    void *leftmin = bst_pop_min(pool, &b->left, i);
    if (leftmin != NULL)
      return leftmin;
    stack_t *s = b->value;
    if (b->ikey >= i) {
      if (s->size == 1 && false) {
        void *retval = stack_pop(s);
        *bst = bst_deli(b, b->ikey, func_free);
        pool_add(pool, POOL_STACK, s);
        return retval;
      }
      if (s->size != 0) {
        return stack_pop(s);
      }
    }
  }
  return bst_pop_min(pool, &b->right, i);
}

void pool_empty_stack(pool_t *pool, stack_t *stack) {
  for (long i = 0; i < stack->size; i++) {
    if (stack->items[i])
      pool_add(pool, POOL_VALUE, stack->items[i]);
  }
  stack->size = 0;
}

void *pool_get(pool_t *pool, long bufsize, byte_t type) {
  switch (type) {
    case POOL_STRING:;
      string_t *str = bst_pop_min(pool, &pool->strstack, bufsize);
      if (str) str->length = 0;
      return str;
    case POOL_VWORD:;
      value_t *vword = bst_pop_min(pool, &pool->vwordstack, bufsize);
      if (vword) {
        if (vword->str_word)
          vword->str_word->length = 0;
        else
          vword->str_word = pool_req(bufsize, POOL_STRING);
      }
      return vword;
    case POOL_VSTACK:;
      value_t *vstack = bst_pop_min(pool, &pool->vstackstack, bufsize);
      if (vstack) {
        pool_empty_contain(pool, vstack->container);
        pool_empty_stack(pool, vstack->container->stack);
      }
      return vstack;
    case POOL_VMACRO:;
      return NULL;
      value_t *vmacro = bst_pop_min(pool, &pool->vmacrostack, bufsize);
      if (vmacro) pool_empty_stack(pool, vmacro->macro);
      return vmacro;
    case POOL_VCUSTOM:;
      value_t *vcustom = bst_pop_min(pool, &pool->vcustomstack, bufsize);
      if (vcustom) {
        custom_t *cstm = ht_get(OBJ_TABLE, vcustom->str_word);
        cstm->freefunc(vcustom->custom);
        vcustom->str_word->length = 0;
      }
      return vcustom;
    case POOL_VCLIB:;
      value_t *vclib = bst_pop_min(pool, &pool->vclibstack, bufsize);
      if (vclib)
        if (vclib->str_word)
          vclib->str_word->length = 0;
      return vclib;
    case POOL_STACK:;
      stack_t *stack = bst_pop_min(pool, &pool->stackstack, bufsize);
      if (stack) pool_empty_stack(pool, stack);
      return stack;
    case POOL_CONTAIN:;
      contain_t *contain = stack_pop(pool->containstack);
      if (contain) {
        pool_empty_contain(pool, contain);
        contain->stack = bst_pop_min(pool, &pool->stackstack, bufsize);
        if (contain->stack) pool_empty_stack(pool, contain->stack);
      }
      return contain;
    case POOL_HT:
      return stack_pop(pool->htstack);
    case POOL_VERR:;
      value_t *verr = stack_pop(pool->verrstack);
      if (verr) {
        if (verr->error->error)
          pool->strstack = bst_stack_add(pool->strstack, verr->error->error->bufsize, verr->error->error);
        if (verr->error->str_word)
          pool->strstack = bst_stack_add(pool->strstack, verr->error->str_word->bufsize, verr->error->str_word);
      }
      return verr;
    default:
      return NULL;
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
        return vmacro;
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
        contain_t *contain = paw_alloc(1, sizeof(contain_t));
        contain->stack = pool_req(bufsize, POOL_STACK);
        contain->iflag = true;
        contain->sflag = true;
        return contain;
      case POOL_HT:
        return init_ht(DEFAULT_HT_SIZE);
      case POOL_VERR:;
        value_t *verr = init_value(VERR);
        verr->error = paw_alloc(1, sizeof(error_t));
        return verr;
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

byte_t val2pool_type(value_t *v) {
  switch (v->type) {
    case VWORD:
      return POOL_VWORD;
    case VSTACK:
      return POOL_VSTACK;
    case VMACRO:
      return POOL_VMACRO;
    case VERR:
      return POOL_VERR;
    case VCUSTOM:
      return POOL_VCUSTOM;
    case VCLIB:
      return POOL_VCLIB;
  }
}

void print_pool_bst(bst_t *bst, bool f) {
  if (!bst) {
    if (f)
      printf("()");
    return;
  }
  printf("(");
  print_pool_bst(bst->left, false);
  printf(" %ld ", bst->ikey);
  print_pool_bst(bst->right, false);
  printf(")");
}

void show_pool() {
  printf("strstack:    ");
  print_pool_bst(OBJ_POOL->strstack, true);
  printf("\nvwordstack:  ");
  print_pool_bst(OBJ_POOL->vwordstack, true);
  printf("\nvstackstack: ");
  print_pool_bst(OBJ_POOL->vstackstack, true);
  printf("\nvmacrostack: ");
  print_pool_bst(OBJ_POOL->vmacrostack, true);
  printf("\nvcustomstack:");
  print_pool_bst(OBJ_POOL->vcustomstack, true);
  printf("\nvclibstack:  ");
  print_pool_bst(OBJ_POOL->vclibstack, true);
  printf("\nstackstack:  ");
  print_pool_bst(OBJ_POOL->stackstack, true);

  long i;
  printf("\n\ncontainstack:[");
  if (OBJ_POOL->containstack->size) {
    for (i = 0; i < OBJ_POOL->containstack->size - 1; i++) {
      printf("CONTAIN,");
    } printf("CONTAIN]\n");
  } else printf("]\n");
  printf("htstack:     [");
  if (OBJ_POOL->htstack->size) {
    for (i = 0; i < OBJ_POOL->htstack->size - 1; i++) {
      printf("HT,");
    } printf("HT]\n");
  } else printf("]\n");
  printf("verrstack:   [");
  if (OBJ_POOL->verrstack->size) {
    for (i = 0; i < OBJ_POOL->verrstack->size - 1; i++) {
      printf("VERR");
    } printf("VERR]\n");
  } else printf("]\n");
}
