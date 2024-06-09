#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#include <better_string.h>
#include <stdlib.h>
/* /\*! @brief This structure is to be used in singly linked lists that hold */
/*  * key-value pairs for a hash table. *\/ */
/* typedef struct NODE_STRUCT { */
/*   /\*! @brief Stores the key *\/ */
/*   string_t *key; */
/*   /\*! @brief Stores the value *\/ */
/*   void *value; */
/*   /\*! @brief Stores the next node in the singly linked list *\/ */
/*   struct NODE_STRUCT *next; */
/* } node_t; */

/* /\*! @brief Singly Linked List used for separate chaining for a hash table *\/ */
/* typedef struct { */
/*   /\*! @brief The first node. Is NULL until value is added to sll. *\/ */
/*   node_t *head; */
/*   /\*! @brief current size of hash table *\/ */
/*   size_t size; */
/* } sll_t; */

/*! @brief binary search tree (BST) implementation */
/*! This BST is used as an improved bucket for a hash table structure,
  thus requiring the storage of a key-value pair.
 */
typedef struct BST_STRUCT {
  /*! @brief stores the left node */
  struct BST_STRUCT *left;
  /*! @brief stores the right node */
  struct BST_STRUCT *right;
  union {
    /*! @brief stores the hash table key */
    string_t *key;
    int ikey;
  };
  /*! @brief stores the hash table value */
  void *value;
} bst_t;

/*! @brief hash table implementation */
/*! this hash table stores a value in a numbered bucket according to what number
 * the key hashes to; each bucket is a singly linked list to handle collisions
 */
typedef struct {
  /*! @brief Stores the key-value pairs */
  bst_t **buckets;
  /*! @brief Total number of buckets in hash table */
  size_t size;
} ht_t;

/*! Allocates memory for new hash table */
ht_t *init_ht(size_t size);

/*! add key value pair to hash table. If the key already exists, the value is
 *replaced and the old value is freed according to freefunc. */
void ht_add(ht_t *h, string_t *key, void *v, void (*freefunc)(void *));

/*! Gets value from hash table by key */
void *ht_get(ht_t *h, string_t *key);

/*! Deletes item from hash table */
void ht_delete(ht_t *h, string_t *key, void (*freefunc)(void *));

/*! returns true if key exists in hash table. false otherwise */
bool ht_exists(ht_t *h, string_t *key);

/* returns true if key exists in hashtable and associated value is not null */
bool ht_defined(ht_t *h, string_t *key);

/*! deep copy of hash table */
ht_t *ht_copy(ht_t *h, void *(*copyfunc)(void *));

/*! Frees hash table */
void ht_free(ht_t *h, void (*freefunc)(void *));

/* frees with 'free' */
void ht_free_free(void *h);

/*! hashes key into integer for hash table */
unsigned long hash(ht_t *h, string_t *key);

/* /\*! Allocates memory for new node struct. *\/ */
/* node_t *init_node(string_t *key, void *v); */

/* /\*! Deep copy of sll node *\/ */
/* node_t *node_copy(node_t *n, void *(*copyfunc)(void *)); */

/* /\*! Frees node *\/ */
/* void node_free(node_t *n, void (*freefunc)(void *)); */

/* /\*! Allocates memory for new singly linked list *\/ */
/* sll_t *init_sll(); */

/* /\*! Adds value to singly linked list. If key already exists, the value is */
/*  * replaced and the old one is freed according to freefunc. *\/ */
/* void sll_add(sll_t *l, string_t *key, void *v, void (*freefunc)(void *)); */

/* /\*! Gets value by key from singly linked list. *\/ */
/* void *sll_get(sll_t *l, string_t *key); */

/* /\*! deletes item from singly linked list *\/ */
/* void sll_delete(sll_t *l, string_t *k, void (*freefunc)(void *)); */

/* /\*! deep copy of singly linked list *\/ */
/* sll_t *sll_copy(sll_t *l, void *(*copyfunc)(void *)); */

/* /\*! Frees singly linked list *\/ */
/* void sll_free(sll_t *l, void (*freefunc)(void *)); */

/*! Allocates memory for new BST node. */
bst_t *init_bst();

/*! inserts key value pair into BST */
void *bst_add(bst_t *bst, string_t *key, void *value, void (*freefunc)(void *));
void *bst_addi(bst_t *bst, int i, void *value, void (*freefunc)(void *));

/*! deletes node from BST by key */
void *bst_del(bst_t *bst, string_t *key, void (*freefunc)(void *));
void *bst_deli(bst_t *bst, int i, void (*freefunc)(void *));

void print_integer_bst(bst_t *bst);

/*! gets value from BST by key */
void *bst_get(bst_t *bst, string_t *key);
void *bst_geti(bst_t *bst, int i);

/*! Copies BST tree recursively */
bst_t *bst_copy(bst_t *bst, void *(*copyfunc)(void *));
bst_t *bst_copy_ikey(bst_t *bst, void *(*copyfunc)(void *));

/*! Frees BST */
void bst_free(bst_t *bst, void (*freefunc)(void *));
void bst_free_ikey(bst_t *bst, void (*freefunc)(void *));

#endif
