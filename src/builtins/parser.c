#include <builtins/parser.h>
#include <stdio.h>

extern stack_t *STACK;

void cog_d(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *stack = stack_pop(cur->stack);
  if (stack == NULL) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  if (stack->container->stack->size != 1) {
    stack_push(cur->stack, stack);
    eval_error("TYPE ERROR", v);
    return;
  }
  value_t *word = stack->container->stack->items[0];
  if (word->type != VWORD) {
    stack_push(cur->stack, stack);
    eval_error("TYPE ERROR", v);
    return;
  }
  string_free(cur->delims);
  cur->delims = string_copy(word->str_word);
  value_free(stack);
}

void cog_i(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *stack = stack_pop(cur->stack);
  if (stack == NULL) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  if (stack->container->stack->size != 1) {
    stack_push(cur->stack, stack);
    eval_error("TYPE ERROR", v);
    return;
  }
  value_t *word = stack->container->stack->items[0];
  if (word->type != VWORD) {
    stack_push(cur->stack, stack);
    eval_error("TYPE ERROR", v);
    return;
  }
  string_free(cur->ignored);
  cur->ignored = string_copy(word->str_word);
  value_free(stack);
}

void cog_s(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *stack = stack_pop(cur->stack);
  if (stack == NULL) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  if (stack->container->stack->size != 1) {
    stack_push(cur->stack, stack);
    eval_error("TYPE ERROR", v);
    return;
  }
  value_t *word = stack->container->stack->items[0];
  if (word->type != VWORD) {
    stack_push(cur->stack, stack);
    eval_error("TYPE ERROR", v);
    return;
  }
  string_free(cur->singlets);
  cur->singlets = string_copy(word->str_word);
  value_free(stack);
}

void cog_dtgl(value_t *v) {
  contain_t *c = stack_peek(STACK);
  c->dflag = !c->dflag;
}

void cog_itgl(value_t *v) {
  contain_t *c = stack_peek(STACK);
  c->iflag = !c->iflag;
}

void cog_stgl(value_t *v) {
  contain_t *c = stack_peek(STACK);
  c->sflag = !c->sflag;
}

void cog_geti(value_t *v) {
  value_t *list = init_value(VWORD);
  contain_t *cur = stack_peek(STACK);
  list->str_word = string_copy(cur->ignored);
  if (list->str_word == NULL)
    list->str_word = init_string("");
  push_quoted(cur, list);
}

void cog_getd(value_t *v) {
  value_t *list = init_value(VWORD);
  contain_t *cur = stack_peek(STACK);
  list->str_word = string_copy(cur->delims);
  if (list->str_word == NULL)
    list->str_word = init_string("");
  push_quoted(cur, list);
}

void cog_gets(value_t *v) {
  value_t *list = init_value(VWORD);
  contain_t *cur = stack_peek(STACK);
  list->str_word = string_copy(cur->singlets);
  if (list->str_word == NULL)
    list->str_word = init_string("");
  push_quoted(cur, list);
}

void cog_delim(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *strc = stack_pop(cur->stack);
  if (!strc) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  if (strc->container->stack->size != 1) {
    eval_error("TYPE ERROR", v);
    stack_push(cur->stack, strc);
    return;
  }
  value_t *strv = strc->container->stack->items[0];
  if (strv->type != VWORD) {
    eval_error("TYPE ERROR", v);
    stack_push(cur->stack, strc);
    return;
  }
  if (!strv->str_word->length) {
    value_free(strc);
    return;
  }
  if (!cur->delims) {
    cur->delims = init_string("");
  }
  for (int i = 0; i < strv->str_word->length; i++) {
    if (!isdelim(strv->str_word->value[i])) {
      if (cur->dflag) {
        string_append(cur->delims, strv->str_word->value[i]);
      } else {
        for (int j = 0; j < cur->delims->length; j++) {
          if (strv->str_word->value[i] == cur->delims->value[j]) {
            for (int k = j; k < cur->delims->length; k++) {
              cur->delims->value[k] = cur->delims->value[k + 1];
            }
            cur->delims->length--;
            break;
          }
        }
      }
    }
  }
  value_free(strc);
}

void cog_undelim(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *strc = stack_pop(cur->stack);
  if (!strc) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  if (strc->container->stack->size != 1) {
    eval_error("TYPE ERROR", v);
    stack_push(cur->stack, strc);
    return;
  }
  value_t *strv = strc->container->stack->items[0];
  if (strv->type != VWORD) {
    eval_error("TYPE ERROR", v);
    stack_push(cur->stack, strc);
    return;
  }
  if (!strv->str_word->length) {
    value_free(strc);
    return;
  }
  if (!cur->delims) {
    cur->delims = init_string("");
  }
  for (int i = 0; i < strv->str_word->length; i++) {
    if (isdelim(strv->str_word->value[i])) {
      if (cur->dflag) {
        for (int j = 0; j < cur->delims->length; j++) {
          if (strv->str_word->value[i] == cur->delims->value[j]) {
            for (int k = j; k < cur->delims->length; k++) {
              cur->delims->value[k] = cur->delims->value[k + 1];
            }
            cur->delims->length--;
            break;
          }
        }
      } else {
        string_append(cur->delims, strv->str_word->value[i]);
      }
    }
  }
  value_free(strc);
}

void add_funcs_parser(ht_t* flit) {
  add_func(flit, cog_d, "d");
  add_func(flit, cog_i, "i");
  add_func(flit, cog_s, "s");
  add_func(flit, cog_dtgl, "dtgl");
  add_func(flit, cog_itgl, "itgl");
  add_func(flit, cog_stgl, "stgl");
  add_func(flit, cog_geti, "geti");
  add_func(flit, cog_getd, "getd");
  add_func(flit, cog_gets, "gets");
  add_func(flit, cog_delim, "delim");
  add_func(flit, cog_undelim, "undelim");
}
