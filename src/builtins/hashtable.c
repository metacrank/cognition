#include <builtins/hashtable.h>
#include <builtinslib.h>
#include <macros.h>
#include <pool.h>
#include <string.h>

extern stack_t *STACK;
extern stack_t *CONTAIN_DEF_STACK;
extern stack_t *MACRO_DEF_STACK;

void cog_def(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (stack->size < 2) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *quot = stack_pop(stack);
  value_t *wordc = stack_pop(stack);
  if (value_stack(wordc)[0]->size != 1) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(stack, wordc);
    stack_push(stack, quot);
    return;
  }
  value_t *word = value_stack(wordc)[0]->items[0];
  if (word->type != VWORD) {
    printf("nop\n");
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(stack, wordc);
    stack_push(stack, quot);
    return;
  }
  if (quot->type == VSTACK) {
    ht_delete(cur->flit, word->str_word, macro_def_stack_push);
    ht_add(cur->word_table, word->str_word, quot->container, contain_def_stack_push);
  } else {
    ht_delete(cur->word_table, word->str_word, contain_def_stack_push);
    ht_add(cur->flit, word->str_word, quot->macro, macro_def_stack_push);
  }
  word->str_word = NULL;
  value_free_safe(wordc);
  free(quot);
}

void cog_undef(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  value_t *wordc = stack_pop(stack);
  if (!wordc) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  if (value_stack(wordc)[0]->size != 1) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(stack, wordc);
    return;
  }
  value_t *word = value_stack(wordc)[0]->items[0];
  if (word->type != VWORD) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(stack, wordc);
    return;
  }
  contain_t *def;
  if ((def = ht_get(cur->word_table, word->str_word))) {
    ht_add(cur->word_table, word->str_word, NULL, nop);
    stack_push(CONTAIN_DEF_STACK, def);
    word->str_word = NULL;
    value_free_safe(wordc);
    return;
  }
  stack_t *macro;
  if ((macro = ht_get(cur->flit, word->str_word))) {
    ht_add(cur->flit, word->str_word, NULL, nop);
    stack_push(MACRO_DEF_STACK, macro);
    word->str_word = NULL;
    value_free_safe(wordc);
    return;
  }
  eval_error(U"UNDEFINED WORD", v);
  stack_push(stack, wordc);
}

void cog_unglue(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  value_t *wordc = stack_peek(stack);
  if (!wordc) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  if (value_stack(wordc)[0]->size != 1) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  value_t *wordval = value_stack(wordc)[0]->items[0];
  if (wordval->type != VWORD) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  contain_t *def = ht_get(cur->word_table, wordval->str_word);
  if (def == NULL) {
    stack_t *macro = ht_get(cur->flit, wordval->str_word);
    if (macro == NULL) {
      eval_error(U"UNDEFINED WORD", v);
      return;
    }
    value_free_safe(wordval);
    if (wordc->type == VMACRO) {
      wordc->macro->size = 0;
      for (int i = 0; i < macro->size; i++) {
        stack_push(wordc->macro, value_copy(macro->items[i]));
      }
      return;
    }
    wordc->type = VMACRO;
    stack_t *temp = wordc->container->stack;
    temp->size = 0;
    wordc->container->stack = NULL;
    contain_def_stack_push(wordc->container);
    wordc->macro = temp;
    for (int i = 0; i < macro->size; i++) {
      stack_push(wordc->macro, value_copy(macro->items[i]));
    }
    return;
  }
  if (wordc->type == VMACRO) {
    wordc->type = VSTACK;
    value_stack_free(wordc->macro);
    wordc->container = contain_copy(def, value_copy);
    return;
  }
  contain_def_stack_push(wordc->container);
  wordc->container = contain_copy(def, value_copy);
}

void cog_isdef(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  value_t *wordc = stack_peek(stack);
  if (!wordc) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  if (value_stack(wordc)[0]->size != 1) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  value_t *wordval = value_stack(wordc)[0]->items[0];
  if (wordval->type != VWORD) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  if (ht_defined(cur->word_table, wordval->str_word) || ht_defined(cur->flit, wordval->str_word)) {
    if (wordval->str_word->length == 0)
      string_append(wordval->str_word, U't');
    return;
  }
  wordval->str_word->length = 0;
}

/* void cog_wordlist(value_t *v) { */
/*   contain_t *cur = stack_peek(STACK); */
/*   ht_t *h = cur->word_table; */
/*   contain_t *listc = init_contain(NULL, NULL, NULL); */
/*   stack_t *list = listc->stack; */
/*   if (h) { */
/*     for (int i = 0; i < h->size; i++) { */
/*       sll_t *l = h->buckets[i]; */
/*       for (node_t *n = l->head; n != NULL; n = n->next) { */
/*         if (n->value) { */
/*           value_t *word = init_value(VWORD); */
/*           word->str_word = string_copy(n->key); */
/*           stack_push(list, word); */
/*         } */
/*       } */
/*     } */
/*   } */
/*   h = cur->flit; */
/*   if (h) { */
/*     for (int i = 0; i < h->size; i++) { */
/*       sll_t *l = h->buckets[i]; */
/*       for (node_t *n = l->head; n != NULL; n = n->next) { */
/*         if (n->value) { */
/*           value_t *word = init_value(VWORD); */
/*           word->str_word = string_copy(n->key); */
/*           stack_push(list, word); */
/*         } */
/*       } */
/*     } */
/*   } */
/*   value_t *listval = init_value(VSTACK); */
/*   listval->container = listc; */
/*   stack_push(cur->stack, listval); */
/* } */

void cog_wordlist(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  ht_t *h = cur->word_table;
  value_t *listval = pool_req(DEFAULT_STACK_SIZE, POOL_VSTACK);
  stack_t *list = listval->container->stack;
  if (h) {
    for (int i = 0; i < h->size; i++) {
      bst_t *bst = h->buckets[i];
      list_bst_keys(bst, list);
    }
  }
  h = cur->flit;
  if (h) {
    for (int i = 0; i < h->size; i++) {
      bst_t *bst = h->buckets[i];
      list_bst_keys(bst, list);
    }
  }
  stack_push(cur->stack, listval);
}

void cog_bequeath(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  value_t *wordc = stack_pop(stack);
  if (!wordc) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *child = stack_peek(stack);
  if (!child) {
    stack_push(stack, wordc);
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  if (child->type != VSTACK) {
    stack_push(stack, wordc);
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  int stacksize = value_stack(wordc)[0]->size;
  contain_t *defs[stacksize];
  stack_t *aliases[stacksize];
  for (int i = 0; i < stacksize; i++) {
    value_t *wordval = value_stack(wordc)[0]->items[i];
    if (wordval->type != VWORD) {
      stack_push(stack, wordc);
      eval_error(U"BAD ARGUMENT TYPE", v);
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
        eval_error(U"UNDEFINED WORD", v);
        return;
      }
    }
  }
  for (int i = 0; i < stacksize; i++) {
    value_t *wordval = value_stack(wordc)[0]->items[i];
    if (defs[i]) {
      if (!child->container->word_table)
        child->container->word_table = pool_req(0, POOL_HT);
      ht_add(child->container->word_table,
             string_copy(wordval->str_word),
             contain_new_clone(defs[i]),
             contain_def_stack_push);
    } else if (aliases[i]) {
      if (!child->container->flit)
        child->container->flit = pool_req(0, POOL_HT);
      ht_add(child->container->flit,
             string_copy(wordval->str_word),
             value_stack_copy(aliases[i]),
             macro_def_stack_push);
    } else {
      if (child->container->faliases == NULL)
        child->container->faliases = pool_req(0, POOL_HT);
      stack_push(child->container->faliases, wordval->str_word);
      wordval->str_word = NULL;
    }
  }
  value_free_safe(wordc);
}

void add_funcs_hashtable(ht_t *flit) {
  add_func(flit, cog_def, U"def");
  add_func(flit, cog_undef, U"undef");
  add_func(flit, cog_unglue, U"unglue");
  add_func(flit, cog_isdef, U"def?");
  add_func(flit, cog_wordlist, U"wordlist");
  add_func(flit, cog_bequeath, U"bequeath");
}
