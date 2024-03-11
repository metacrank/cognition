#include <builtins/hashtable.h>

extern stack_t *STACK;

void cog_def(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (stack->size < 2) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *quot = stack_pop(stack);
  value_t *wordc = stack_pop(stack);
  if (wordc->container->stack->size != 1) {
    eval_error("TYPE ERROR", v);
    stack_push(stack, wordc);
    stack_push(stack, quot);
    return;
  }
  value_t *word = wordc->container->stack->items[0];
  if (word->type != VWORD) {
    eval_error("TYPE ERROR", v);
    stack_push(stack, wordc);
    stack_push(stack, quot);
    return;
  }
  ht_add(cur->word_table, word->str_word, quot, contain_free);
  free(word);
}

void cog_undef(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  value_t *wordc = stack_pop(stack);
  if (!wordc) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  if (wordc->container->stack->size != 1) {
    eval_error("TYPE ERROR", v);
    stack_push(stack, wordc);
    return;
  }
  value_t *word = wordc->container->stack->items[0];
  if (word->type != VWORD) {
    eval_error("TYPE ERROR", v);
    stack_push(stack, wordc);
    return;
  }
  ht_delete(cur->word_table, word->str_word, contain_free);
  ht_delete(cur->flit, word->str_word, value_stack_free);
  free(word);
}

void add_funcs_hashtable(ht_t *flit) {
}
