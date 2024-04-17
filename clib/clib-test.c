#include <cognition.h>
#include <builtinslib.h>
#include <strnum.h>

extern stack_t *STACK;

void iseven(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *v1 = stack_peek(cur->stack);
  if (!v1) {
    eval_error(U"custom error", v);
    return;
  }
  stack_t *v1stack = *value_stack(v1);
  if (v1stack->size != 1) {
    eval_error(U"haha! deleted", v);
    stack_pop(cur->stack);
    value_free_safe(v1);
    return;
  }
  value_t *v1val = v1stack->items[0];
  if (v1val->type != VWORD) {
    eval_error(U"weird number", v);
    return;
  }
  int num = string_to_int(v1val->str_word);
  string_free(v1val->str_word);
  if (num % 2) {
    v1val->str_word = init_string(U"");
    return;
  }
  v1val->str_word = init_string(U"t");
}

void add_funcs(ht_t *flit) {
  add_func(flit, iseven, U"even?");
}

void add_objs(ht_t *object_table) {

}
