#ifndef POOL_H_
#define POOL_H_

#include <cognition.h>
#include <hash_table.h>

typedef struct {
  bst_t *strstack;
  bst_t *vwordstack;
  bst_t *vstackstack;
  bst_t *vmacrostack;
  bst_t *vcustomstack;
  bst_t *vclibstack;
  bst_t *stackstack;

  stack_t *containstack;
  stack_t *htstack;
  stack_t *verrstack;
} pool_t;

enum {
  POOL_STRING,
  POOL_VWORD,
  POOL_VSTACK,
  POOL_VMACRO,
  POOL_VCUSTOM,
  POOL_VCLIB,
  POOL_STACK,
  POOL_CONTAIN,
  POOL_HT,
  POOL_WT,
  POOL_FLIT,
  POOL_VERR,
  POOL_VALUE
};

/* initialize pool */
pool_t *init_pool();

/*! trims all bottom nodes of all data structures once */
void pool_gc(pool_t *pool);

/*! pool-aware allocation -- collects garbage if fails */
void *paw_alloc(size_t nmemb, size_t size);

/*! adds value to stack in bst */
bst_t *bst_stack_add(bst_t *bst, long i, void *value);

/*! adds value to pool */
void pool_add(pool_t *pool, byte_t type, void *value);

/*! adds value to OBJ_POOL */
void pool_addobj(byte_t type, void *value);

/*! gets value from pool */
void *pool_get(pool_t *pool, long bufsize, byte_t type);

/*! gets value from OBJ_POOL, allocating on failure */
void *pool_req(long bufsize, byte_t type);

/*! frees pool */
void pool_free(pool_t *pool);

/*! diplays a bst, printing '()' if bst == NULL && f */
void print_pool_bst(bst_t *bst, bool f);

/*! print state of current object pool */
void show_pool();

#endif // POOL_H_
