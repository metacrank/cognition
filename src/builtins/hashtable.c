#include <builtins/hashtable.h>
#include <builtinslib.h>
#include <macros.h>
#include <string.h>

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
  if (value_stack(wordc)[0]->size != 1) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, wordc);
    stack_push(stack, quot);
    return;
  }
  value_t *word = value_stack(wordc)[0]->items[0];
  if (word->type != VWORD) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, wordc);
    stack_push(stack, quot);
    return;
  }
  if (quot->type == VSTACK) {
    ht_add(cur->word_table, word->str_word, quot->container, contain_free);
    ht_delete(cur->flit, word->str_word, value_stack_free);
    quot->container = NULL;
  } else {
    ht_add(cur->flit, word->str_word, quot->macro, value_stack_free);
    ht_delete(cur->word_table, word->str_word, contain_free);
    quot->macro = NULL;
  }
  word->str_word = NULL;
  value_free(wordc);
  value_free(quot);
}

void cog_undef(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  value_t *wordc = stack_pop(stack);
  if (!wordc) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  if (value_stack(wordc)[0]->size != 1) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, wordc);
    return;
  }
  value_t *word = value_stack(wordc)[0]->items[0];
  if (word->type != VWORD) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(stack, wordc);
    return;
  }
  bool exists = ht_exists(cur->word_table, word->str_word) || ht_exists(cur->flit, word->str_word);
  if (!exists) {
    eval_error("UNDEFINED WORD", v);
    stack_push(stack, wordc);
    return;
  }
  ht_delete(cur->word_table, word->str_word, contain_free);
  ht_delete(cur->flit, word->str_word, value_stack_free);
  value_free(wordc);
}

void cog_unglue(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  value_t *wordc = stack_peek(stack);
  if (!wordc) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  if (value_stack(wordc)[0]->size != 1) {
    eval_error("BAD ARGUMENT TYPE", v);
    return;
  }
  value_t *wordval = value_stack(wordc)[0]->items[0];
  if (wordval->type != VWORD) {
    eval_error("BAD ARGUMENT TYPE", v);
    return;
  }
  contain_t *def = ht_get(cur->word_table, wordval->str_word);;
  if (def == NULL) {
    eval_error("UNDEFINED WORD", v);
    return;
  }
  value_free(wordval);
  value_stack(wordc)[0]->size = 0;
  for (int i = 0; i < def->stack->size; i++) {
    stack_push(value_stack(wordc)[0], value_copy(def->stack->items[i]));
  }
}

void cog_isdef(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  value_t *wordc = stack_peek(stack);
  if (!wordc) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  if (wordc->container->stack->size != 1) {
    eval_error("BAD ARGUMENT TYPE", v);
    return;
  }
  value_t *wordval = wordc->container->stack->items[0];
  if (wordval->type != VWORD) {
    eval_error("BAD ARGUMENT TYPE", v);
    return;
  }
  bool exists = ht_exists(cur->word_table, wordval->str_word) || ht_exists(cur->flit, wordval->str_word);
  wordval->str_word->length = 0;
  wordval->str_word->value[0] = '\0';
  if (exists) {
    string_append(wordval->str_word, 't');
  }
}

/* void cog_alias(value_t *v) { */
/*   contain_t *cur = stack_peek(STACK); */
/*   stack_t *stack = cur->stack; */
/*   value_t *quot = stack_pop(stack); */
/*   if (!quot) { */
/*     eval_error("TOO FEW ARGUMENTS", v); */
/*     return; */
/*   } */
/*   value_t *wordc = stack_pop(stack); */
/*   if (!wordc) { */
/*     eval_error("TOO FEW ARGUMENTS", v); */
/*     stack_push(stack, quot); */
/*     return; */
/*   } */
/*   if (wordc->container->stack->size != 1) { */
/*     eval_error("BAD ARGUMENT TYPE", v); */
/*     stack_push(stack, wordc); */
/*     stack_push(stack, quot); */
/*     return; */
/*   } */
/*   value_t *wordval = wordc->container->stack->items[0]; */
/*   if (wordval->type != VWORD) { */
/*     eval_error("BAD ARGUMENT TYPE", v); */
/*     stack_push(stack, wordc); */
/*     stack_push(stack, quot); */
/*     return; */
/*   } */
/*   stack_t *macro = init_stack(DEFAULT_STACK_SIZE); */
/*   stack_t *family = init_stack(DEFAULT_STACK_SIZE); */
/*   stack_push(family, cur); */
/*   expandstack(quot->container, macro, family); */
/*   free(family->items); */
/*   free(family); */
/*   ht_add(cur->flit, string_copy(wordval->str_word), macro, value_stack_free); */
/*   value_free(wordc); */
/*   value_free(quot); */
/* } */

/* void cog_compile(value_t *v) { */
/*   contain_t *cur = stack_peek(STACK); */
/*   stack_t *stack = cur->stack; */
/*   value_t *quot = stack_peek(stack); */
/*   if (!quot) { */
/*     eval_error("TOO FEW ARGUMENTS", v); */
/*     return; */
/*   } */
/*   stack_t *macro = init_stack(DEFAULT_STACK_SIZE); */
/*   stack_t *family = init_stack(DEFAULT_STACK_SIZE); */
/*   stack_push(family, cur); */
/*   expandstack(quot->container->stack, macro, family); */
/*   free(family->items); */
/*   free(family); */
/*   value_stack_free(quot->container->stack); */
/*   quot->container->stack = macro; */
/* } */

void cog_wordlist(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  ht_t *h = cur->word_table;
  contain_t *listc = init_contain(NULL, NULL, NULL);
  stack_t *list = listc->stack;
  for (int i = 0; i < h->size; i++) {
    sll_t *l = h->buckets[i];
    for (node_t *n = l->head; n != NULL; n = n->next) {
      value_t *word = init_value(VWORD);
      word->str_word = string_copy(n->key);
      stack_push(list, word);
    }
  }
  h = cur->flit;
  for (int i = 0; i < h->size; i++) {
    sll_t *l = h->buckets[i];
    for (node_t *n = l->head; n != NULL; n = n->next) {
      value_t *word = init_value(VWORD);
      word->str_word = string_copy(n->key);
      stack_push(list, word);
    }
  }
  value_t *listval = init_value(VSTACK);
  listval->container = listc;
  stack_push(cur->stack, listval);
}

void cog_bequeath(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  value_t *wordc = stack_pop(stack);
  if (!wordc) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *child = stack_peek(stack);
  if (child->type != VSTACK) {
    stack_push(stack, wordc);
    eval_error("BAD ARGUMENT TYPE", v);
    return;
  }
  int stacksize = value_stack(wordc)[0]->size;
  contain_t *defs[stacksize];
  stack_t *aliases[stacksize];
  for (int i = 0; i < stacksize; i++) {
    value_t *wordval = value_stack(wordc)[0]->items[i];
    if (wordval->type != VWORD) {
      stack_push(stack, wordc);
      eval_error("BAD ARGUMENT TYPE", v);
      return;
    }
    defs[i] = ht_get(cur->word_table, wordval->str_word);
    if (defs[i] == NULL) {
      aliases[i] = ht_get(cur->flit, wordval->str_word);
      if (aliases[i] == NULL) {
        if (isfalias(wordval)) {
          continue;
        }
        stack_push(stack, wordc);
        eval_error("UNDEFINED WORD", v);
        return;
      }
    }
  }
  for (int i = 0; i < stacksize; i++) {
    value_t *wordval = value_stack(wordc)[0]->items[i];
    if (defs[i]) {
      ht_add(child->container->word_table,
             string_copy(wordval->str_word),
             contain_value_copy(defs[i]),
             contain_free);
    } else if (aliases[i]) {
      ht_add(child->container->flit,
             string_copy(wordval->str_word),
             value_stack_copy(aliases[i]),
             contain_free);
    } else {
      if (child->container->faliases == NULL)
        child->container->faliases = init_stack(DEFAULT_STACK_SIZE);
      stack_push(child->container->faliases, wordval->str_word);
      wordval->str_word = NULL;
    }
  }
  value_free(wordc);
}

void add_funcs_hashtable(ht_t *flit) {
  add_func(flit, cog_def, "def");
  add_func(flit, cog_undef, "undef");
  add_func(flit, cog_unglue, "unglue");
  add_func(flit, cog_isdef, "isdef");
  /* add_func(flit, cog_alias, "alias"); */
  /* add_func(flit, cog_bind, "bind"); */
  /* add_func(flit, cog_compile, "compile"); */
  add_func(flit, cog_wordlist, "wordlist");
  add_func(flit, cog_bequeath, "bequeath");
}
