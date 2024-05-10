#include <hash_table.h>
#include <macros.h>
#include <better_string.h>
#include <string.h>

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
    if (string_comp(s, cur->key) == 0) {
      freefunc(cur->value);
      string_free(s);
      cur->value = v;
      return;
    }
    cur = cur->next;
  }
  if (string_comp(s, cur->key) == 0) {
    freefunc(cur->value);
    string_free(s);
    cur->value = v;
    return;
  }
  node_t *n = init_node(s, v);
  cur->next = n;
  l->size++;
}

void *sll_get(sll_t *l, string_t *k) {
  if (l->head == NULL)
    return NULL;
  node_t *cur = l->head;
  while (cur != NULL) {
    if (string_comp(k, cur->key) == 0)
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
  if (string_comp(cur->key, k) == 0) {
    tmp = cur->next;
    node_free(cur, freefunc);
    l->head = tmp;
    return;
  }
  while (cur->next != NULL) {
    if (string_comp(cur->next->key, k) == 0) {
      tmp = cur->next->next;
      node_free(cur->next, freefunc);
      cur->next = tmp;
      return;
    }
    cur = cur->next;
  }
}

sll_t *sll_copy(sll_t *l, void *(*copyfunc)(void *)) {
  if (!l)
    return NULL;
  sll_t *list = init_sll();
  node_t *n = l->head;
  while (n) {
    if (n->value) {
      list->head = node_copy(n, copyfunc);
      n = n->next;
      break;
    }
    n = n->next;
  }
  node_t *nn = list->head;
  while (n) {
    if (n->value) {
      nn->next = node_copy(n, copyfunc);
      nn = nn->next;
      nn->next = NULL;
    }
    n = n->next;
  }
  return list;
}

void sll_free(sll_t *l, void (*func)(void *)) {
  if (!l)
    return;
  node_t *cur = l->head;
  node_t *tmp;
  while (cur != NULL) {
    tmp = cur;
    cur = cur->next;
    node_free(tmp, func);
  }
  free(l);
}

bst_t *init_bst() { return calloc(1, sizeof(bst_t)); }

void bst_add(bst_t *bst, string_t *key, void *value) {
  if (!key)
    return;
  bst_t *parent = NULL;
  bst_t *cur = bst;
  bool isleft = true;
  long l;
  while (cur) {
    l = string_comp(cur->key, key);
    if (l < 0) {
      parent = cur;
      cur = cur->left;
      isleft = true;
    } else if (l > 0) {
      parent = cur;
      cur = cur->right;
      isleft = false;
    } else {
      return;
    }
  }
  if (parent) {
    if (isleft) {
      parent->left = init_bst();
      parent->left->key = key;
      parent->left->value = value;
    } else {
      parent->right = init_bst();
      parent->right->key = key;
      parent->right->value = value;
    }
  } else {
    bst_t *b = init_bst();
    b->key = key;
    b->value = value;
  }
}

void bst_del(bst_t *bst, string_t *key, void (*freefunc)(void *)) {
  if (!bst)
    return;
  bst_t *parent = NULL;
  bst_t *cur = bst;
  long l;
  bool isleft;
  while (cur) {
    l = string_comp(cur->key, key);
    if (l < 0) {
      parent = cur;
      cur = cur->left;
      isleft = true;
    } else if (l > 0) {
      parent = cur;
      cur = cur->right;
      isleft = false;
    } else
      break;
  }
  bst_t *left = cur->left;
  bst_t *right = cur->right;
  if (parent) {
    if (!left && !right) {
      string_free(cur->key);
      freefunc(cur->value);
      free(cur);
    } else if (left && !right) {
      string_free(cur->key);
      freefunc(cur->value);
      free(cur);
      if (isleft)
        parent->left = left;
      else
        parent->right = left;
    } else if (!left && right) {
      string_free(cur->key);
      freefunc(cur->value);
      free(cur);
      if (isleft)
        parent->left = right;
      else
        parent->right = right;
    } else {
      bst_t *ioparent = cur;
      bst_t *inorder = right;
      while (inorder->left) {
        ioparent = inorder;
        inorder = inorder->left;
      }
      string_free(cur->key);
      freefunc(cur->value);
      cur->key = inorder->key;
      cur->value = inorder->value;
      if (inorder->right)
        ioparent->left = inorder->right;
      free(inorder);
    }
  }
}

void *bst_get(bst_t *bst, string_t *key) {
  if (!bst)
    return NULL;
  long l = string_comp(key, bst->key);
  if (l < 0)
    return bst_get(bst->left, key);
  else if (l > 0)
    return bst_get(bst->right, key);
  else
    return bst->value;
}

void *bst_geti(bst_t *bst, int i) {
  if (!bst)
    return NULL;
  long l = i - bst->ikey;
  if (l < 0)
    return bst_geti(bst->left, i);
  else if (l > 0)
    return bst_geti(bst->right, i);
  else
    return bst->value;
}

bst_t *bst_copy(bst_t *bst, void *(*copyfunc)(void *)) {
  if (!bst)
    return NULL;
  bst_t *b = init_bst();
  b->key = string_copy(bst->key);
  b->value = copyfunc(b->value);
  b->ikey = bst->ikey;
  if (bst->left)
    b->left = bst_copy(bst->left, copyfunc);
  if (bst->right)
    b->right = bst_copy(bst->right, copyfunc);
  return b;
}

void bst_free(bst_t *bst, void (*freefunc)(void *)) {
  if (!bst)
    return;
  if (bst->left)
    bst_free(bst->left, freefunc);
  if (bst->right)
    bst_free(bst->right, freefunc);
  if (bst) {
    string_free(bst->key);
    if (bst->value)
      freefunc(bst->value);
    free(bst);
  }
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
  sll_add(h->buckets[hash(h, key)], key, v, freefunc);
}

void *ht_get(ht_t *h, string_t *key) {
  if (key == NULL || h == NULL)
    return NULL;
  return sll_get(h->buckets[hash(h, key)], key);
}

// bool ht_exists(ht_t *h, string_t *key) { return ht_get(h, key) != NULL; }
bool ht_defined(ht_t *h, string_t *key) { return ht_get(h, key) != NULL; }

bool ht_exists(ht_t *h, string_t *key) {
  if (key == NULL || h == NULL)
    return false;
  sll_t *l = h->buckets[hash(h, key)];
  if (l->head == NULL)
    return false;
  node_t *cur = l->head;
  while (cur != NULL) {
    if (string_comp(key, cur->key) == 0)
      return true;
    cur = cur->next;
  }
  return false;
}

void ht_delete(ht_t *h, string_t *key, void (*freefunc)(void *)) {
  if (h == NULL)
    return;
  if (key == NULL)
    return;
  sll_delete(h->buckets[hash(h, key)], key, freefunc);
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

void ht_free_free(void *h) {
  ht_free(h, free);
}

/* DJB2 HASH FUNCTION */
unsigned long hash(ht_t *h, string_t *key) {
  unsigned long hash = 5381;
  char32_t *keyend = key->value + key->length;
  char32_t *c = key->value;
  while (c < keyend) {
    hash = ((hash << 5) + hash) + *c++; /* hash * 33 + c */
  }
  return hash % h->size;
}
