#include <hash_table.h>
#include <cognition.h>
#include <strnum.h>
#include <builtinslib.h>

extern stack_t *STACK;

void iseven(value_t *v) {
  printf("iseven\n");
  int stacksize = DEFAULT_STACK_SIZE;
  stack_t *st = init_stack(DEFAULT_STACK_SIZE);
}

void add_funcs(ht_t *flit) {
  iseven(NULL);
  //add_func(flit, iseven, U"even?");
}

void add_objs(ht_t *object_table) {}
