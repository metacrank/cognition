#include <builtins/combinators.h>
#include <builtinslib.h>
#include <string.h>

extern stack_t *STACK;
extern stack_t *EVAL_STACK;

extern void add_funcs(ht_t *flit);

/* passes down nothing. should make child? */
void cog_quote(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *v1 = stack_pop(cur->stack);
  if (v1 == NULL) {
    eval_error("EMPTY STACK");
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
  if (cur->word_table->size) {
    v1->container->word_table = ht_copy(cur->word_table, contain_value_copy);
  } else {
    v->container->word_table = init_ht(100);
  }
  if (cur->flit->size) {
    v1->container->flit = ht_copy(cur->flit, value_stack_copy);
  } else {
    v1->container->flit = init_ht(100);
  }
  v1->container->cranks = init_stack(10); /* stack_copy(cur->cranks, cranks_copy); */
  v1->container->err_stack = stack_copy(cur->err_stack, value_copy);
  v1->container->stack = init_stack(10);
  v1->container->faliases = stack_copy(cur->faliases, (void*(*)(void*))string_copy);
  v1->container->delims = string_copy(cur->delims);
  v1->container->ignored = string_copy(cur->ignored);
  v1->container->dflag = cur->dflag;
  v1->container->iflag = cur->iflag;
  stack_push(cur->stack, v1);
}

void cog_stack(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *v1 = init_value(VSTACK);
  v1->container = calloc(1, sizeof(contain_t));
  v->container->word_table = init_ht(100);
  v1->container->flit = init_ht(100);
  v1->container->cranks = init_stack(10); /* stack_copy(cur->cranks, cranks_copy); */
  v1->container->err_stack = init_stack(10);
  v1->container->stack = init_stack(10);
  // v1->container->faliases = stack_copy(cur->faliases, (void*(*)(void*))string_copy);
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
    eval_error("EMPTY STACK");
    return;
  }
  value_t *retval = init_value(VSTACK);
  retval->container = calloc(1, sizeof(contain_t));
  retval->container->word_table = init_ht(100);
  retval->container->flit = init_ht(100);
  retval->container->cranks = init_stack(10); /* stack_copy(cur->cranks, cranks_copy); */
  retval->container->err_stack = init_stack(10);
  retval->container->stack = init_stack(10);
  // retval->container->faliases = stack_copy(cur->faliases, (void*(*)(void*))string_copy);
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
      eval_error(full_err->value);
      string_free(err);
      string_free(full_err);
      // return;
    }
  }
  stack_push(cur->stack, retval);
}

void cog_bstack(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *list = stack_pop(STACK);
  if (list == NULL) {
    eval_error("EMPTY STACK");
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
      eval_error(full_err->value);
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
    eval_error("TOO FEW ARGUMENTS");
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
    eval_error("TOO FEW ARGUMENTS");
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
    eval_error("TOO FEW ARGUMENTS");
    return;
  }
  value_t *index = stack_pop(cur->stack);
  if (index->container->stack->size != 1) {
    stack_push(cur->stack, index);
    eval_error("TYPE ERROR");
    return;
  }
  value_t *idxval = index->container->stack->items[0];
  if (idxval->type != VWORD) {
    stack_push(cur->stack, index);
    eval_error("TYPE ERROR");
    return;
  }
  value_t *v1 = stack_pop(cur->stack);
  value_t *stack = stack_peek(cur->stack);
  int idx = atoi(idxval->str_word->value);
  if (idx < 0 || idx > stack->container->stack->size) {
    stack_push(cur->stack, v1);
    stack_push(cur->stack, index);
    eval_error("OUT OF RANGE ERROR");
    return;
  }
  value_free(index);
  stack_push(stack->container->stack, NULL);
  for (int i = idx + 1; i < stack->container->stack->size; i++) {
    stack->container->stack->items[i] = stack->container->stack->items[i-1];
  }
  stack->container->stack->items[idx] = v1;
}

void cog_dip(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  value_t *v1 = stack_pop(stack);
  if (!v1) {
    eval_error("EMPTY STACK");
    return;
  }
  evalf();
  stack_push(stack, v1);
}

void cog_if(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  stack_t *stack = cur->stack;
  value_t *v3 = stack_pop(stack);
  if (!v3) {
    eval_error("EMPTY STACK");
    return;
  }
  value_t *v2 = stack_pop(stack);
  if (!v2) {
    value_free(v3);
    eval_error("EMPTY STACK");
    return;
  }
  value_t *v1 = stack_pop(stack);
  if (!v1) {
    value_free(v2);
    value_free(v3);
    eval_error("EMPTY STACK");
    return;
  }
  if (!strisint(v1->str_word)) {
    value_free(v1);
    value_free(v2);
    value_free(v3);
    eval_error("TYPE ERROR");
    return;
  }
  int v1_fixed = atoi(v1->str_word->value);
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

void cog_loop(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  if (cur->stack->size == 0) {
    eval_error("EMPTY STACK");
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
      eval_error("EMPTY STACK");
      break;
    }
    if (cont_val->container->stack->size != 1) {
      eval_error("TYPE ERROR");
      break;
    }
    value_t *cont_val_value = cont_val->container->stack->items[0];
    if (cont_val_value->type != VWORD) {
      eval_error("TYPE ERROR");
      break;
    }
    // maybe generalise booleans beyond numbers?
    if (!strisint(cont_val_value->str_word)) {
      eval_error("TYPE ERROR");
      break;
    }
    int c = atoi(cont_val_value->str_word->value);
    if (c) cont = true;
    else cont = false;
  } while (cont);
  value_t *b = stack_pop(EVAL_STACK);
  value_free(b);
  free(family->items);
  free(family);
}



void add_funcs_combinators(ht_t* flit) {
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
}
