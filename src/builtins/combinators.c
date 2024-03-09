#include <builtins/combinators.h>
#include <builtinslib.h>
#include <string.h>
#include <stdio.h>

extern stack_t *STACK;
extern stack_t *EVAL_STACK;

extern void add_funcs(ht_t *flit);

/* passes down nothing. should make child? */
void cog_quote(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *v1 = stack_pop(cur->stack);
  if (v1 == NULL) {
    eval_error("EMPTY STACK", v);
    return;
  }
  push_quoted(cur, v1);
}

void cog_eval(value_t *v) {
  evalf();
}

void cog_child(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *v1 = init_value(VSTACK);
  v1->container = calloc(1, sizeof(contain_t));
  contain_copy_attributes(cur, v1->container);
  v1->container->err_stack = stack_copy(cur->err_stack, value_copy);
  v1->container->stack = init_stack(10);
  stack_push(cur->stack, v1);
}

void cog_stack(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *v1 = init_value(VSTACK);
  v1->container = calloc(1, sizeof(contain_t));
  v->container->word_table = init_ht(500);
  v1->container->flit = init_ht(500);
  v1->container->cranks = init_stack(10);
  v1->container->err_stack = init_stack(10);
  v1->container->stack = init_stack(10);
  v1->container->faliases = init_stack(10);
  v1->container->delims = string_copy(cur->delims);
  v1->container->ignored = string_copy(cur->ignored);
  v1->container->dflag = cur->dflag;
  v1->container->iflag = cur->iflag;
  stack_push(cur->stack, v1);
}

void cog_wstack(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *list = stack_pop(STACK);
  if (list == NULL) {
    eval_error("EMPTY STACK", v);
    return;
  }
  value_t *retval = init_value(VSTACK);
  retval->container = calloc(1, sizeof(contain_t));
  retval->container->word_table = init_ht(100);
  retval->container->flit = init_ht(100);
  retval->container->cranks = init_stack(10);
  retval->container->err_stack = init_stack(10);
  retval->container->stack = init_stack(10);
  retval->container->faliases = init_stack(10);
  retval->container->delims = string_copy(cur->delims);
  retval->container->ignored = string_copy(cur->ignored);
  retval->container->dflag = cur->dflag;
  retval->container->iflag = cur->iflag;
  contain_t *expand;
  stack_t *macro;
  for (int i = 0; i < list->container->stack->size; i++) {
    value_t *v1 = list->container->stack->items[i];
    if ((macro = ht_get(cur->flit, v1->str_word))) {
      ht_add(retval->container->flit, v1->str_word, value_stack_copy(macro), value_stack_free);
    } else if ((expand = ht_get(cur->word_table, v1->str_word))) {
      ht_add(retval->container->word_table, v1->str_word, contain_value_copy(expand), contain_free);
    } else if (isfalias(v1)) {
      stack_push(retval->container->faliases, string_copy(v1->str_word));
    } else {
      string_t *full_err = string_copy(v->str_word);
      string_t *err = init_string(": no word ");
      string_concat(full_err, err);
      string_concat(full_err, v1->str_word);
      eval_error(full_err->value, v);
      string_free(err);
      string_free(full_err);
      // return;
    }
  }
  stack_push(cur->stack, retval);
}

//needs to not rely on cog_child(), specifically because cranks should not be copied
void cog_bstack(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *list = stack_pop(STACK);
  if (list == NULL) {
    eval_error("EMPTY STACK", v);
    return;
  }
  cog_child(v);
  value_t *v1 = stack_peek(cur->stack);
  for (int i = 0; i < list->container->stack->size; i++) {
    value_t *v2 = list->container->stack->items[i];
    if (ht_exists(v1->container->flit, v2->str_word)) {
      ht_delete(v1->container->flit, v2->str_word, value_stack_free);
    } else if (ht_exists(v1->container->word_table, v2->str_word)) {
      ht_delete(v1->container->word_table, v2->str_word, contain_free);
    } else if (isfaliasin(v1->container, v2)) {
      for (int i = 0; i < v1->container->faliases->size; i++) {
        string_t *f = v1->container->faliases->items[i];
        if (strcmp(f->value, v2->str_word->value) == 0) {
          stack_popdeep(v1->container->faliases, i);
        }
      }
    } else {
      string_t *full_err = string_copy(v->str_word);
      string_t *err = init_string(": no word ");
      string_concat(full_err, err);
      string_concat(full_err, v1->str_word);
      eval_error(full_err->value, v);
      string_free(err);
      string_free(full_err);
      // return;
    }
  }
}

void cog_sub(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *v1 = init_value(VSTACK);
  v1->container = init_contain(init_ht(500), init_ht(500), init_stack(10));
  stack_push(v1->container->faliases, init_string("f"));
  add_funcs(v1->container->flit);
  stack_push(cur->stack, v1);
}

void cog_compose(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  if (cur->stack->size < 2) {
    eval_error("EMPTY STACK", v);
    return;
  }
  value_t *v2 = stack_pop(cur->stack);
  if (v2->container->stack->size == 0) {
    value_free(v2);
    return;
  }
  value_t *v1 = stack_peek(cur->stack);
  for (int i = 0; i < v2->container->stack->size; i++) {
    stack_push(v1->container->stack, v2->container->stack->items[i]);
  }
  v2->container->stack->size = 0;
  value_free(v2);
}

void cog_prepose(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  if (cur->stack->size < 2) {
    eval_error("EMPTY STACK", v);
    return;
  }
  value_t *v2 = stack_pop(cur->stack);
  if (v2->container->stack->size == 0) {
    value_free(v2);
    return;
  }
  value_t *v1 = stack_peek(cur->stack);
  stack_t *stack = v2->container->stack;
  for (int i = 0; i < v1->container->stack->size; i++) {
    stack_push(stack, v1->container->stack->items[i]);
  }
  v1->container->stack->size = 0;
  v2->container->stack = v1->container->stack;
  v1->container->stack = stack;
  value_free(v2);
}

void cog_put(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  if (cur->stack->size < 3) {
    eval_error("EMPTY STACK", v);
    return;
  }
  value_t *index = stack_pop(cur->stack);
  if (index->container->stack->size != 1) {
    stack_push(cur->stack, index);
    eval_error("TYPE ERROR", v);
    return;
  }
  value_t *idxval = index->container->stack->items[0];
  if (idxval->type != VWORD) {
    stack_push(cur->stack, index);
    eval_error("TYPE ERROR", v);
    return;
  }
  value_t *v1 = stack_pop(cur->stack);
  value_t *stack = stack_peek(cur->stack);
  int idx = atoi(idxval->str_word->value);
  if (idx < 0 || idx > stack->container->stack->size) {
    stack_push(cur->stack, v1);
    stack_push(cur->stack, index);
    eval_error("OUT OF RANGE", v);
    return;
  }
  value_free(index);
  stack_push(stack->container->stack, NULL);
  for (int i = idx + 1; i < stack->container->stack->size; i++) {
    stack->container->stack->items[i] = stack->container->stack->items[i-1];
  }
  stack->container->stack->items[idx] = v1;
}

//??????
/* void cog_dip(value_t *v) { */
/*   contain_t *cur = stack_peek(STACK); */
/*   stack_t *stack = cur->stack; */
/*   value_t *v1 = stack_pop(stack); */
/*   if (!v1) { */
/*     eval_error("EMPTY STACK"); */
/*     return; */
/*   } */
/*   evalf(); */
/*   stack_push(stack, v1); */
/* } */
void cog_dip(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (cur->stack->size < 2) {
    eval_error("EMPTY STACK", v);
    return;
  }
  value_t *quot = stack_pop(stack);
  value_t *v1 = stack_pop(stack);
  stack_push(stack, quot);
  evalf();
  stack_push(stack, v1);
}

void cog_if(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  value_t *v3 = stack_pop(stack);
  if (!v3) {
    eval_error("EMPTY STACK", v);
    return;
  }
  value_t *v2 = stack_pop(stack);
  if (!v2) {
    stack_push(cur->stack, v3);
    eval_error("EMPTY STACK", v);
    return;
  }
  value_t *v1 = stack_pop(stack);
  if (!v1) {
    stack_push(cur->stack, v2);
    stack_push(cur->stack, v3);
    eval_error("EMPTY STACK", v);
    return;
  }
  if (v1->type != VWORD) {
    stack_push(stack, v1);
    stack_push(stack, v2);
    stack_push(stack, v3);
    eval_error("TYPE ERROR", v);
    return;
  }
  bool v1_fixed = word_truth(v1);
  value_free(v1);
  if (v1_fixed) {
    stack_push(stack, v2);
    value_free(v3);
    evalf();
  } else {
    stack_push(stack, v3);
    value_free(v2);
    evalf();
  }
}

// currenty equivalent to evalf'ing the loop body every repetition
// you could truly 'loop' the code by applying eval() to each member of the quote instead
void cog_loop(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  if (cur->stack->size == 0) {
    eval_error("EMPTY STACK", v);
    return;
  }
  value_t *body = stack_pop(cur->stack);
  stack_push(EVAL_STACK, body);
  bool cont = true;
  stack_t *family = init_stack(10);
  stack_push(family, cur);
  do {
    evalstack(body->container, family);
    value_t *cont_val = stack_peek(STACK);
    if (cont_val == NULL) {
      eval_error("EMPTY STACK", v);
      break;
    }
    if (cont_val->container->stack->size != 1) {
      eval_error("TYPE ERROR", v);
      break;
    }
    value_t *cont_val_value = cont_val->container->stack->items[0];
    if (cont_val_value->type != VWORD) {
      eval_error("TYPE ERROR", v);
      break;
    }
    cont = word_truth(cont_val_value);
  } while (cont);
  value_t *b = stack_pop(EVAL_STACK);
  value_free(b);
  free(family->items);
  free(family);
}

void cog_times(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  if (cur->stack->size < 2) {
    eval_error("EMPTY STACK", v);
    return;
  }
  value_t *q1 = stack_pop(cur->stack);
  if (q1->container->stack->size != 1) {
    stack_push(cur->stack, q1);
    eval_error("TYPE ERROR", v);
    return;
  }
  value_t *w1 = q1->container->stack->items[0];
  if (w1->type != VWORD) {
    stack_push(cur->stack, q1);
    eval_error("TYPE ERROR", v);
    return;
  }
  if (!strisint(w1->str_word)) {
    stack_push(cur->stack, q1);
    eval_error("TYPE ERROR", v);
    return;
  }
  int n = atoi(w1->str_word->value);
  value_t *body = stack_pop(STACK);
  stack_push(EVAL_STACK, body);
  stack_t *family = init_stack(10);
  stack_push(family, cur);
  for (int i = 0; i < n; i++) {
    evalstack(body->container, family);
  }
  free(family->items);
  free(family);
  value_free(body);
  value_free(q1);
}

void cog_split(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  if (cur->stack->size < 2) {
    eval_error("EMPTY STACK", v);
    return;
  }
  value_t *num = stack_pop(cur->stack);
  if (num->container->stack->size != 1) {
    stack_push(cur->stack, num);
    eval_error("TYPE ERROR", v);
    return;
  }
  value_t *numval = num->container->stack->items[0];
  if (numval->type != VWORD) {
    stack_push(cur->stack, num);
    eval_error("TYPE ERROR", v);
    return;
  }
  if (!strisint(numval->str_word)) {
    stack_push(cur->stack, num);
    eval_error("TYPE ERROR", v);
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
  q2->container->stack = init_stack(10);
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
    eval_error("EMPTY STACK", v);
    return;
  }
  value_t *index = stack_pop(stack);
  if (index->container->stack->size != 1) {
    stack_push(stack, index);
    eval_error("TYPE ERROR", v);
    return;
  }
  value_t *idxval = index->container->stack->items[0];
  if (idxval->type != VWORD) {
    stack_push(stack, index);
    eval_error("TYPE ERROR", v);
    return;
  }
  if (!strisint(idxval->str_word)) {
    stack_push(stack, index);
    eval_error("TYPE ERROR", v);
    return;
  }
  int n = atoi(idxval->str_word->value);
  value_t *quot = stack_peek(stack);
  if (n < 0 || n >= quot->container->stack->size) {
    stack_push(stack, index);
    eval_error("OUT OF RANGE", v);
    return;
  }
  value_free(index);
  value_t *v1 = value_copy(quot->container->stack->items[n]);
  stack_push(stack, v1);
}

void cog_substack(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  if (stack->size < 3) {
    eval_error("EMPTY STACK", v);
    return;
  }
  value_t *index2 = stack_pop(stack);
  value_t *index1 = stack_pop(stack);
  if (index1->container->stack->size != 1 || index2->container->stack->size != 1) {
    stack_push(stack, index1);
    stack_push(stack, index2);
    eval_error("TYPE ERROR", v);
    return;
  }
  value_t *idxval1 = index1->container->stack->items[0];
  value_t *idxval2 = index2->container->stack->items[0];
  if (idxval1->type != VWORD || idxval2->type != VWORD) {
    stack_push(stack, index1);
    stack_push(stack, index2);
    eval_error("TYPE ERROR", v);
    return;
  }
  if (!strisint(idxval1->str_word) || !strisint(idxval2->str_word)) {
    stack_push(stack, index1);
    stack_push(stack, index2);
    eval_error("TYPE ERROR", v);
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
  value_free(index1);
  value_free(index2);
  stack_t *qstack = quot->container->stack;
  for (int i = 0; i < n1; i++) {
    value_free(qstack->items[i]);
  }
  for (int i = n2 + 1; i < qstack->size; i++) {
    value_free(qstack->items[i]);
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
    eval_error("EMPTY STACK", v);
    return;
  }
  value_t *index = stack_pop(stack);
  if (index->container->stack->size != 1) {
    stack_push(stack, index);
    eval_error("TYPE ERROR", v);
    return;
  }
  value_t *idxval = index->container->stack->items[0];
  if (idxval->type != VWORD) {
    stack_push(stack, index);
    eval_error("TYPE ERROR", v);
    return;
  }
  if (!strisint(idxval->str_word)) {
    stack_push(stack, index);
    eval_error("TYPE ERROR", v);
    return;
  }
  int n = atoi(idxval->str_word->value);
  value_t *quot = stack_peek(stack);
  if (n < 0 || n >= quot->container->stack->size) {
    stack_push(stack, index);
    eval_error("OUT OF RANGE", v);
    return;
  }
  value_free(index);
  value_free(stack_popdeep(quot->container->stack, n));
}

void cog_uncompose(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  value_t *quot = stack_pop(STACK);
  if (quot == NULL) {
    eval_error("EMPTY STACK", v);
    return;
  }
  for (int i = 0; i < quot->container->stack->size; i++) {
    value_t *element = init_value(VSTACK);
    element->container = calloc(1, sizeof(contain_t));
    element->container->stack = init_stack(10);
    element->container->err_stack = stack_copy(quot->container->err_stack, value_copy);
    contain_copy_attributes(quot->container, element->container);
    stack_push(element->container->stack, quot->container->stack->items[i]);
    stack_push(stack, element);
  }
  quot->container->stack->size = 0;
  value_free(quot);
}

void cog_size(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  value_t *v1 = stack_peek(stack);
  if (v1 == NULL) {
    eval_error("EMPTY STACK", v);
    return;
  }
  int size = v1->container->stack->size;
  char buffer[11];
  snprintf(buffer, 11, "%d", size);
  value_t *sizeval = init_value(VWORD);
  sizeval->str_word = init_string(buffer);
  push_quoted(cur, sizeval);
}

void add_funcs_combinators(ht_t *flit) {
  add_func(flit, cog_quote, "quote");
  add_func(flit, cog_eval, "eval");
  add_func(flit, cog_child, "child");
  add_func(flit, cog_stack, "stack");
  add_func(flit, cog_wstack, "wstack");
  add_func(flit, cog_bstack, "bstack");
  add_func(flit, cog_sub, "sub");
  add_func(flit, cog_compose, "compose");
  add_func(flit, cog_prepose, "prepose");
  add_func(flit, cog_put, "put");
  add_func(flit, cog_dip, "dip");
  add_func(flit, cog_if, "if");
  add_func(flit, cog_loop, "loop");
  add_func(flit, cog_times, "times");
  add_func(flit, cog_split, "split");
  add_func(flit, cog_vat, "vat");
  add_func(flit, cog_substack, "substack");
  add_func(flit, cog_del, "del");
  add_func(flit, cog_uncompose, "uncompose");
  add_func(flit, cog_size, "size");
}
