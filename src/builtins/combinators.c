#include <builtins/combinators.h>
#include <builtinslib.h>
#include <macros.h>
#include <string.h>
#include <stdio.h>

extern stack_t *STACK;
extern stack_t *EVAL_STACK;

extern void add_funcs(ht_t *flit);

void cog_quote(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *v1 = stack_pop(cur->stack);
  if (v1 == NULL) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  push_quoted(cur, v1);
}

void cog_eval(value_t *v) {
  contain_t *old = stack_peek(STACK);
  evalf();
  old = stack_peek(STACK);
  if (!old) return;
  dec_crank(old);
}

void cog_child(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *v1 = init_value(VSTACK);
  v1->container = calloc(1, sizeof(contain_t));
  contain_copy_attributes(cur, v1->container);
  v1->container->err_stack = stack_copy(cur->err_stack, value_copy);
  v1->container->stack = init_stack(DEFAULT_STACK_SIZE);
  stack_push(cur->stack, v1);
}

void cog_stack(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *v1 = init_value(VSTACK);
  v1->container = calloc(1, sizeof(contain_t));
  v1->container->err_stack = init_stack(DEFAULT_STACK_SIZE);
  v1->container->stack = init_stack(DEFAULT_STACK_SIZE);
  v1->container->faliases = init_stack(DEFAULT_STACK_SIZE);
  v1->container->delims = string_copy(cur->delims);
  v1->container->singlets = string_copy(cur->singlets);
  v1->container->ignored = string_copy(cur->ignored);
  v1->container->dflag = cur->dflag;
  v1->container->iflag = cur->iflag;
  v1->container->sflag = cur->sflag;
  stack_push(cur->stack, v1);
}

/* void cog_wstack(value_t *v) { */
/*   contain_t *cur = stack_peek(STACK); */
/*   value_t *list = stack_pop(STACK); */
/*   if (list == NULL) { */
/*     eval_error("TOO FEW ARGUMENTS", v); */
/*     return; */
/*   } */
/*   value_t *retval = init_value(VSTACK); */
/*   retval->container = calloc(1, sizeof(contain_t)); */
/*   retval->container->err_stack = init_stack(DEFAULT_STACK_SIZE); */
/*   retval->container->stack = init_stack(DEFAULT_STACK_SIZE); */
/*   retval->container->faliases = init_stack(DEFAULT_STACK_SIZE); */
/*   retval->container->delims = string_copy(cur->delims); */
/*   retval->container->singlets = string_copy(cur->singlets); */
/*   retval->container->ignored = string_copy(cur->ignored); */
/*   retval->container->dflag = cur->dflag; */
/*   retval->container->iflag = cur->iflag; */
/*   retval->container->sflag = cur->sflag; */
/*   contain_t *expand; */
/*   stack_t *macro; */
/*   for (int i = 0; i < list->container->stack->size; i++) { */
/*     value_t *v1 = list->container->stack->items[i]; */
/*     if ((macro = ht_get(cur->flit, v1->str_word))) { */
/*       ht_add(retval->container->flit, v1->str_word, value_stack_copy(macro), value_stack_free); */
/*     } else if ((expand = ht_get(cur->word_table, v1->str_word))) { */
/*       ht_add(retval->container->word_table, v1->str_word, contain_value_copy(expand), contain_free); */
/*     } else if (isfalias(v1)) { */
/*       stack_push(retval->container->faliases, string_copy(v1->str_word)); */
/*     } else { */
/*       eval_error("UNDEFINED WORD", v); */
/*       return; */
/*     } */
/*   } */
/*   stack_push(cur->stack, retval); */
/* } */

/* //needs to not rely on cog_child(), specifically because cranks should not be copied */
/* void cog_bstack(value_t *v) { */
/*   contain_t *cur = stack_peek(STACK); */
/*   value_t *list = stack_pop(STACK); */
/*   if (list == NULL) { */
/*     eval_error("TOO FEW ARGUMENTS", v); */
/*     return; */
/*   } */
/*   cog_child(v); */
/*   value_t *v1 = stack_peek(cur->stack); */
/*   for (int i = 0; i < list->container->stack->size; i++) { */
/*     value_t *v2 = list->container->stack->items[i]; */
/*     if (ht_exists(v1->container->flit, v2->str_word)) { */
/*       ht_delete(v1->container->flit, v2->str_word, value_stack_free); */
/*     } else if (ht_exists(v1->container->word_table, v2->str_word)) { */
/*       ht_delete(v1->container->word_table, v2->str_word, contain_free); */
/*     } else if (isfaliasin(v1->container, v2)) { */
/*       for (int i = 0; i < v1->container->faliases->size; i++) { */
/*         string_t *f = v1->container->faliases->items[i]; */
/*         if (strcmp(f->value, v2->str_word->value) == 0) { */
/*           stack_popdeep(v1->container->faliases, i); */
/*         } */
/*       } */
/*     } else { */
/*       eval_error("UNDEFINED WORD", v); */
/*       return; */
/*     } */
/*   } */
/* } */

void cog_macro(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *v1 = init_value(VMACRO);
  v1->macro = init_stack(DEFAULT_STACK_SIZE);
  stack_push(cur->stack, v1);
}

void cog_expand(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *v1 = stack_peek(cur->stack);
  if (!v1) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  stack_t *family = init_stack(DEFAULT_STACK_SIZE);
  stack_t *new = init_stack(DEFAULT_STACK_SIZE);
  stack_push(family, cur);
  if (v1->type == VSTACK) {
    stack_push(family, v1->container);
    expandstack(v1->container->stack, new, family);
    value_stack_free(v1->container->stack);
    v1->container->stack = new;
  }
  else {
    expandstack(v1->macro, new, family);
    value_stack_free(v1->macro);
    v1->macro = new;
  }
  free(family->items);
  free(family);
}

void cog_cast(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *v1 = stack_pop(cur->stack);
  if (!v1) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *quot = stack_peek(cur->stack);
  stack_t *v1stack = *value_stack(v1);
  if (v1stack->size != 1) {
    stack_push(cur->stack, v1);
    eval_error("BAD ARGUMENT TYPE", v);
    return;
  }
  value_t *num = v1stack->items[0];
  if (num->type != VWORD) {
    stack_push(cur->stack, v1);
    eval_error("BAD ARGUMENT TYPE", v);
    return;
  }
  if (strcmp(num->str_word->value, "0") == 0 || strcmp(num->str_word->value, "VSTACK") == 0) {
    if (quot->type == VSTACK) {
      value_free_safe(v1);
      return;
    }
    quot->type = VSTACK;
    contain_t *c = init_contain(NULL, NULL, NULL);
    c->stack = quot->macro;
    quot->container = c;
    value_free_safe(v1);
    return;
  }
  if (strcmp(num->str_word->value, "1") == 0 || strcmp(num->str_word->value, "VMACRO") == 0) {
    if (quot->type == VMACRO) {
      value_free_safe(v1);
      return;
    }
    quot->type = VMACRO;
    stack_t *s = quot->container->stack;
    quot->container->stack = NULL;
    contain_free(quot->container);
    quot->macro = s;
    value_free_safe(v1);
    return;
  }
  eval_error("INDEX OUT OF RANGE", v);
  stack_push(cur->stack, v1);
}

void cog_sub(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *v1 = init_value(VSTACK);
  v1->container = init_contain(NULL, NULL, NULL);
  stack_push(v1->container->faliases, init_string("f"));
  add_funcs(v1->container->flit);
  stack_push(cur->stack, v1);
}

void cog_compose(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  if (cur->stack->size < 2) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *v2 = stack_pop(cur->stack);
  stack_t **v2stack = value_stack(v2);
  if ((*v2stack)->size == 0) {
    value_free_safe(v2);
    return;
  }
  value_t *v1 = stack_peek(cur->stack);
  stack_t **v1stack = value_stack(v1);
  stack_extend(*v1stack, *v2stack);
  (*v2stack)->size = 0;
  value_free_safe(v2);
}

void cog_prepose(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  if (cur->stack->size < 2) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *v2 = stack_pop(cur->stack);
  stack_t **v2stackp = value_stack(v2);
  stack_t *v2stack = *v2stackp;
  if (v2stack->size == 0) {
    value_free_safe(v2);
    return;
  }
  value_t *v1 = stack_peek(cur->stack);
  stack_t **v1stackp = value_stack(v1);
  stack_t *v1stack = *v1stackp;
  stack_t *stack = v2stack;
  stack_extend(stack, v1stack);
  v1stack->size = 0;
  *v2stackp = *v1stackp;
  *v1stackp = stack;
  value_free_safe(v2);
}

void cog_put(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  if (cur->stack->size < 3) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *index = stack_pop(cur->stack);
  if (value_stack(index)[0]->size != 1) {
    stack_push(cur->stack, index);
    eval_error("BAD ARGUMENT TYPE", v);
    return;
  }
  value_t *idxval = value_stack(index)[0]->items[0];
  if (idxval->type != VWORD) {
    stack_push(cur->stack, index);
    eval_error("BAD ARGUMENT TYPE", v);
    return;
  }
  if (!strisint(idxval->str_word)) {
    stack_push(cur->stack, index);
    eval_error("BAD ARGUMENT TYPE", v);
    return;
  }
  value_t *v1 = stack_pop(cur->stack);
  value_t *stack = stack_peek(cur->stack);
  int idx = atoi(idxval->str_word->value);
  stack_t *stackstack = *value_stack(stack);
  if (idx < 0 || idx > stackstack->size) {
    stack_push(cur->stack, v1);
    stack_push(cur->stack, index);
    eval_error("OUT OF RANGE", v);
    return;
  }
  value_free_safe(index);
  stack_push(stackstack, NULL);
  for (int i = idx + 1; i < stackstack->size; i++) {
    stackstack->items[i] = stackstack->items[i-1];
  }
  stackstack->items[idx] = v1;
}

void cog_dip(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (cur->stack->size < 2) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *quot = stack_pop(stack);
  value_t *v1 = stack_pop(stack);
  stack_push(stack, quot);
  evalf();
  if (!cur) return;
  stack_push(stack, v1);
  dec_crank(cur);
}

void cog_if(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  value_t *v3 = stack_pop(stack);
  if (!v3) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *v2 = stack_pop(stack);
  if (!v2) {
    stack_push(cur->stack, v3);
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *v1 = stack_pop(stack);
  if (!v1) {
    stack_push(cur->stack, v2);
    stack_push(cur->stack, v3);
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *w1 = value_stack(v1)[0]->items[0];
  if (w1->type != VWORD) {
    stack_push(stack, v1);
    stack_push(stack, v2);
    stack_push(stack, v3);
    eval_error("BAD ARGUMENT TYPE", v);
    return;
  }
  bool v1_fixed = word_truth(w1);
  value_free_safe(v1);
  if (v1_fixed) {
    stack_push(stack, v2);
    value_free_safe(v3);
    evalf();
  } else {
    stack_push(stack, v3);
    value_free_safe(v2);
    evalf();
  }
  if (!cur) return;
  dec_crank(cur);
}

/* // currenty equivalent to evalf'ing the loop body every repetition */
/* // you could truly 'loop' the code by applying eval() to each member of the quote instead */
/* void cog_loop(value_t *v) { */
/*   contain_t *cur = stack_peek(STACK); */
/*   if (cur->stack->size == 0) { */
/*     eval_error("TOO FEW ARGUMENTS", v); */
/*     return; */
/*   } */
/*   value_t *body = stack_pop(cur->stack); */
/*   stack_push(EVAL_STACK, body); */
/*   bool cont = true; */
/*   stack_t *family = init_stack(DEFAULT_STACK_SIZE); */
/*   stack_push(family, cur); */
/*   do { */
/*     evalstack(body->container, family); */
/*     value_t *cont_val = stack_peek(STACK); */
/*     if (cont_val == NULL) { */
/*       eval_error("TOO FEW ARGUMENTS", v); */
/*       break; */
/*     } */
/*     if (cont_val->container->stack->size != 1) { */
/*       eval_error("BAD ARGUMENT TYPE", v); */
/*       break; */
/*     } */
/*     value_t *cont_val_value = cont_val->container->stack->items[0]; */
/*     if (cont_val_value->type != VWORD) { */
/*       eval_error("BAD ARGUMENT TYPE", v); */
/*       break; */
/*     } */
/*     cont = word_truth(cont_val_value); */
/*   } while (cont); */
/*   value_t *b = stack_pop(EVAL_STACK); */
/*   value_free_safe(b); */
/*   free(family->items); */
/*   free(family); */
/* } */

/* void cog_times(value_t *v) { */
/*   contain_t *cur = stack_peek(STACK); */
/*   if (cur->stack->size < 2) { */
/*     eval_error("TOO FEW ARGUMENTS", v); */
/*     return; */
/*   } */
/*   value_t *q1 = stack_pop(cur->stack); */
/*   if (q1->container->stack->size != 1) { */
/*     stack_push(cur->stack, q1); */
/*     eval_error("BAD ARGUMENT TYPE", v); */
/*     return; */
/*   } */
/*   value_t *w1 = q1->container->stack->items[0]; */
/*   if (w1->type != VWORD) { */
/*     stack_push(cur->stack, q1); */
/*     eval_error("BAD ARGUMENT TYPE", v); */
/*     return; */
/*   } */
/*   if (!strisint(w1->str_word)) { */
/*     stack_push(cur->stack, q1); */
/*     eval_error("BAD ARGUMENT TYPE", v); */
/*     return; */
/*   } */
/*   int n = atoi(w1->str_word->value); */
/*   value_t *body = stack_pop(STACK); */
/*   stack_push(EVAL_STACK, body); */
/*   stack_t *family = init_stack(DEFAULT_STACK_SIZE); */
/*   stack_push(family, cur); */
/*   for (int i = 0; i < n; i++) { */
/*     evalstack(body->container, family); */
/*   } */
/*   free(family->items); */
/*   free(family); */
/*   value_free_safe(body); */
/*   value_free_safe(q1); */
/* } */

void cog_split(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  if (cur->stack->size < 2) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *num = stack_pop(cur->stack);
  if (num->container->stack->size != 1) {
    stack_push(cur->stack, num);
    eval_error("BAD ARGUMENT TYPE", v);
    return;
  }
  value_t *numval = num->container->stack->items[0];
  if (numval->type != VWORD) {
    stack_push(cur->stack, num);
    eval_error("BAD ARGUMENT TYPE", v);
    return;
  }
  if (!strisint(numval->str_word)) {
    stack_push(cur->stack, num);
    eval_error("BAD ARGUMENT TYPE", v);
    return;
  }
  int n = atoi(numval->str_word->value);
  value_t *q = stack_peek(STACK);
  if (n < 0 || n > q->container->stack->size) {
    stack_push(cur->stack, num);
    eval_error("OUT OF RANGE", v);
    return;
  }
  value_t *q2 = init_value(VSTACK);

  q2->container = calloc(1, sizeof(contain_t));
  q2->container->stack = init_stack(DEFAULT_STACK_SIZE);
  q2->container->err_stack = stack_copy(q->container->err_stack, value_copy);
  contain_copy_attributes(q->container, q2->container);

  for (int i = n; i < q->container->stack->size; i++) {
    stack_push(q2->container->stack, q->container->stack->items[i]);
  }
  q->container->stack->size = n;
  stack_push(cur->stack, q2);
}

void cog_vat(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (stack->size < 2) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *index = stack_pop(stack);
  if (index->container->stack->size != 1) {
    stack_push(stack, index);
    eval_error("BAD ARGUMENT TYPE", v);
    return;
  }
  value_t *idxval = index->container->stack->items[0];
  if (idxval->type != VWORD) {
    stack_push(stack, index);
    eval_error("BAD ARGUMENT TYPE", v);
    return;
  }
  if (!strisint(idxval->str_word)) {
    stack_push(stack, index);
    eval_error("BAD ARGUMENT TYPE", v);
    return;
  }
  int n = atoi(idxval->str_word->value);
  value_t *quot = stack_peek(stack);
  if (n < 0 || n >= quot->container->stack->size) {
    stack_push(stack, index);
    eval_error("OUT OF RANGE", v);
    return;
  }
  value_free_safe(index);
  value_t *v1 = value_copy(quot->container->stack->items[n]);
  stack_push(stack, v1);
}

/* void cog_nth(value_t *v) { */
/*   contain_t *cur = stack_peek(STACK); */
/*   stack_t *stack = cur->stack; */
/*   if (stack->size < 2) { */
/*     eval_error("TOO FEW ARGUMENTS", v); */
/*     return; */
/*   } */
/*   value_t *v1 = stack_pop(stack); */
/*   if (v1->container->stack->size != 1) { */
/*     eval_error("BAD ARGUMENT TYPE", v); */
/*     stack_push(stack, v1); */
/*     return; */
/*   } */
/*   value_t *w1 = v1->container->stack->items[0]; */
/*   if (w1->type != VWORD) { */
/*     eval_error("BAD ARGUMENT TYPE", v); */
/*     stack_push(stack, v1); */
/*     return; */
/*   } */
/*   if (!strisint(w1->str_word)) { */
/*     eval_error("BAD ARGUMENT TYPE", v); */
/*     stack_push(stack, v1); */
/*     return; */
/*   } */
/*   int n = atoi(w1->str_word->value); */
/*   value_t *quot = stack_peek(STACK); */
/*   if (quot->container->stack->size <= n || n < 0) { */
/*     eval_error("INDEX OUT OF RANGE", v); */
/*     stack_push(stack, v1); */
/*     return; */
/*   } */
/*   value_free_safe(v1); */
/*   for (int i = 0; i < n; i++) { */
/*     value_free_safe(quot->container->stack->items[i]); */
/*   } */
/*   for (int i = n + 1; i < quot->container->stack->size; i++) { */
/*     value_free_safe(quot->container->stack->items[i]); */
/*   } */
/*   quot->container->stack->items[0] = quot->container->stack->items[n]; */
/*   quot->container->stack->size = 0; */
/* } */

void cog_substack(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (stack->size < 3) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *index2 = stack_pop(stack);
  value_t *index1 = stack_pop(stack);
  if (index1->container->stack->size != 1 || index2->container->stack->size != 1) {
    stack_push(stack, index1);
    stack_push(stack, index2);
    eval_error("BAD ARGUMENT TYPE", v);
    return;
  }
  value_t *idxval1 = index1->container->stack->items[0];
  value_t *idxval2 = index2->container->stack->items[0];
  if (idxval1->type != VWORD || idxval2->type != VWORD) {
    stack_push(stack, index1);
    stack_push(stack, index2);
    eval_error("BAD ARGUMENT TYPE", v);
    return;
  }
  if (!strisint(idxval1->str_word) || !strisint(idxval2->str_word)) {
    stack_push(stack, index1);
    stack_push(stack, index2);
    eval_error("BAD ARGUMENT TYPE", v);
    return;
  }
  int n1 = atoi(idxval1->str_word->value);
  int n2 = atoi(idxval2->str_word->value);
  value_t *quot = stack_peek(stack);
  if (n1 < 0 || n2 < 0 || n1 >= quot->container->stack->size || n2 >= quot->container->stack->size || n2 < n1) {
    stack_push(stack, index1);
    stack_push(stack, index2);
    eval_error("OUT OF RANGE", v);
    return;
  }
  value_free_safe(index1);
  value_free_safe(index2);
  stack_t *qstack = quot->container->stack;
  for (int i = 0; i < n1; i++) {
    value_free_safe(qstack->items[i]);
  }
  for (int i = n2 + 1; i < qstack->size; i++) {
    value_free_safe(qstack->items[i]);
  }
  for (int i = 0; i <= n2 - n1; i++) {
    qstack->items[i] = qstack->items[i + n1];
  }
  qstack->size = n2 - n1 + 1;
}

void cog_del(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (stack->size < 2) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *index = stack_pop(stack);
  if (index->container->stack->size != 1) {
    stack_push(stack, index);
    eval_error("BAD ARGUMENT TYPE", v);
    return;
  }
  value_t *idxval = index->container->stack->items[0];
  if (idxval->type != VWORD) {
    stack_push(stack, index);
    eval_error("BAD ARGUMENT TYPE", v);
    return;
  }
  if (!strisint(idxval->str_word)) {
    stack_push(stack, index);
    eval_error("BAD ARGUMENT TYPE", v);
    return;
  }
  int n = atoi(idxval->str_word->value);
  value_t *quot = stack_peek(stack);
  if (n < 0 || n >= quot->container->stack->size) {
    stack_push(stack, index);
    eval_error("OUT OF RANGE", v);
    return;
  }
  value_free_safe(index);
  value_free_safe(stack_popdeep(quot->container->stack, n));
}

void cog_uncompose(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  value_t *quot = stack_pop(STACK);
  if (quot == NULL) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  for (int i = 0; i < quot->container->stack->size; i++) {
    value_t *element = init_value(VSTACK);
    element->container = calloc(1, sizeof(contain_t));
    element->container->stack = init_stack(DEFAULT_STACK_SIZE);
    element->container->err_stack = stack_copy(quot->container->err_stack, value_copy);
    contain_copy_attributes(quot->container, element->container);
    stack_push(element->container->stack, quot->container->stack->items[i]);
    stack_push(stack, element);
  }
  quot->container->stack->size = 0;
  value_free_safe(quot);
}

void cog_size(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  value_t *v1 = stack_peek(stack);
  if (v1 == NULL) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  int size = value_stack(v1)[0]->size;
  char buffer[11];
  snprintf(buffer, 11, "%d", size);
  value_t *sizeval = init_value(VWORD);
  sizeval->str_word = init_string(buffer);
  push_quoted(cur, sizeval);
}

void cog_type(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  value_t *v1 = stack_peek(stack);
  if (v1 == NULL) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *typeval = init_value(VWORD);
  if (v1->type == VSTACK)
    typeval->str_word = init_string("VSTACK");
  else if (v1->type == VMACRO)
    typeval->str_word = init_string("VMACRO");
  else die("BAD VALUE ON STACK");
  push_quoted(cur, typeval);
}

void add_funcs_combinators(ht_t *flit) {
  add_func(flit, cog_quote, "quote");
  add_func(flit, cog_eval, "eval");
  add_func(flit, cog_child, "child");
  add_func(flit, cog_stack, "stack");
  add_func(flit, cog_macro, "macro");
  add_func(flit, cog_expand, "expand");
  add_func(flit, cog_cast, "cast");
  add_func(flit, cog_sub, "sub");
  add_func(flit, cog_compose, "compose");
  add_func(flit, cog_prepose, "prepose");
  add_func(flit, cog_put, "put");
  add_func(flit, cog_dip, "dip");
  add_func(flit, cog_if, "if");
  /* add_func(flit, cog_split, "split"); */
  /* add_func(flit, cog_vat, "vat"); */
  /* add_func(flit, cog_substack, "substack"); */
  /* add_func(flit, cog_del, "del"); */
  /* add_func(flit, cog_uncompose, "uncompose"); */
  add_func(flit, cog_size, "size");
  add_func(flit, cog_type, "type");
}
