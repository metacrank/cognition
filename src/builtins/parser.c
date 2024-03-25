#include <builtins/parser.h>
#include <builtinslib.h>
#include <macros.h>
#include <stdio.h>
#include <string.h>

extern stack_t *STACK;

void cog_getf(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *list = init_value(VSTACK);
  list->container = init_contain(NULL, NULL, NULL);
  stack_push(cur->stack, list);
  if (cur->faliases == NULL) return;
  for (int i = 0; i < cur->faliases->size; i++) {
    value_t *entry = init_value(VWORD);
    entry->str_word = string_copy(cur->faliases->items[i]);
    stack_push(list->container->stack, entry);
  }
}

void cog_setf(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *list = stack_pop(cur->stack);
  if (list == NULL) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  if (list->container->stack->size == 0) {
    value_stack_free(cur->faliases);
    cur->faliases = NULL;
    value_free_safe(list);
    return;
  }
  for (int i = 0; i < list->container->stack->size; i++) {
    value_t *val = list->container->stack->items[i];
    if (val->type != VWORD) {
      eval_error("BAD ARGUMENT TYPE", v);
      stack_push(cur->stack, list);
      return;
    }
  }
  if (cur->faliases == NULL)
    cur->faliases = init_stack(DEFAULT_STACK_SIZE);
  for (int i = 0; i < cur->stack->size; i++) {
    string_free(cur->faliases->items[i]);
  }
  cur->faliases->size = 0;
  for (int i = 0; i < list->container->stack->size; i++) {
    value_t *val = list->container->stack->items[i];
    if (!isfalias(val)) {
      stack_push(cur->faliases, val->str_word);
      val->str_word = NULL;
    }
  }
  value_free_safe(list);
}

void cog_aliasf(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *list = stack_pop(cur->stack);
  if (list == NULL) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  if (list->container->stack->size == 0) {
    value_free_safe(list);
    return;
  }
  for (int i = 0; i < list->container->stack->size; i++) {
    value_t *val = list->container->stack->items[i];
    if (val->type != VWORD) {
      eval_error("BAD ARGUMENT TYPE", v);
      stack_push(cur->stack, list);
      return;
    }
  }
  if (cur->faliases == NULL)
    cur->faliases = init_stack(DEFAULT_STACK_SIZE);
  for (int i = 0; i < list->container->stack->size; i++) {
    value_t *val = list->container->stack->items[i];
    if (!isfalias(val)) {
      stack_push(cur->faliases, val->str_word);
      val->str_word = NULL;
    }
  }
  value_free_safe(list);
}

void cog_unaliasf(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *list = stack_pop(cur->stack);
  if (list == NULL) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  if (list->container->stack->size == 0) return;
  for (int i = 0; i < list->container->stack->size; i++) {
    value_t *val = list->container->stack->items[i];
    if (val->type != VWORD) {
      eval_error("BAD ARGUMENT TYPE", v);
      stack_push(cur->stack, list);
      return;
    }
  }
  if (cur->faliases == NULL) return;
  if (cur->faliases->size == 0) return;
  for (int i = 0; i < list->container->stack->size; i++) {
    value_t *val = list->container->stack->items[i];
    for (int j = 0; j < cur->faliases->size; j++) {
      string_t *str = cur->faliases->items[j];
      if (string_comp(val->str_word, str) == 0)
        string_free(stack_popdeep(cur->faliases, j));
    }
  }
  value_free_safe(list);
}
extern stack_t *CONTAIN_DEF_STACK;
void cog_d(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *stack = stack_pop(cur->stack);
  if (stack == NULL) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  if (value_stack(stack)[0]->size != 1) {
    stack_push(cur->stack, stack);
    eval_error("BAD ARGUMENT TYPE", v);
    return;
  }
  value_t *word = value_stack(stack)[0]->items[0];
  if (word->type != VWORD) {
    stack_push(cur->stack, stack);
    eval_error("BAD ARGUMENT TYPE", v);
    return;
  }
  string_free(cur->delims);
  cur->delims = string_copy(word->str_word);
  value_free_safe(stack);
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
    eval_error("BAD ARGUMENT TYPE", v);
    return;
  }
  value_t *word = stack->container->stack->items[0];
  if (word->type != VWORD) {
    stack_push(cur->stack, stack);
    eval_error("BAD ARGUMENT TYPE", v);
    return;
  }
  string_free(cur->ignored);
  cur->ignored = string_copy(word->str_word);
  value_free_safe(stack);
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
    eval_error("BAD ARGUMENT TYPE", v);
    return;
  }
  value_t *word = stack->container->stack->items[0];
  if (word->type != VWORD) {
    stack_push(cur->stack, stack);
    eval_error("BAD ARGUMENT TYPE", v);
    return;
  }
  string_free(cur->singlets);
  cur->singlets = string_copy(word->str_word);
  value_free_safe(stack);
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
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(cur->stack, strc);
    return;
  }
  value_t *strv = strc->container->stack->items[0];
  if (strv->type != VWORD) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(cur->stack, strc);
    return;
  }
  if (!strv->str_word->length) {
    value_free_safe(strc);
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
  value_free_safe(strc);
}

void cog_ignore(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *strc = stack_pop(cur->stack);
  if (!strc) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  if (strc->container->stack->size != 1) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(cur->stack, strc);
    return;
  }
  value_t *strv = strc->container->stack->items[0];
  if (strv->type != VWORD) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(cur->stack, strc);
    return;
  }
  if (!strv->str_word->length) {
    value_free_safe(strc);
    return;
  }
  if (!cur->ignored) {
    cur->ignored = init_string("");
  }
  for (int i = 0; i < strv->str_word->length; i++) {
    if (!isignore(strv->str_word->value[i])) {
      if (cur->iflag) {
        string_append(cur->ignored, strv->str_word->value[i]);
      } else {
        for (int j = 0; j < cur->ignored->length; j++) {
          if (strv->str_word->value[i] == cur->ignored->value[j]) {
            for (int k = j; k < cur->ignored->length; k++) {
              cur->ignored->value[k] = cur->ignored->value[k + 1];
            }
            cur->ignored->length--;
            break;
          }
        }
      }
    }
  }
  value_free_safe(strc);
}

void cog_singlet(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *strc = stack_pop(cur->stack);
  if (!strc) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  if (strc->container->stack->size != 1) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(cur->stack, strc);
    return;
  }
  value_t *strv = strc->container->stack->items[0];
  if (strv->type != VWORD) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(cur->stack, strc);
    return;
  }
  if (!strv->str_word->length) {
    value_free_safe(strc);
    return;
  }
  if (!cur->singlets) {
    cur->singlets = init_string("");
  }
  for (int i = 0; i < strv->str_word->length; i++) {
    if (!issinglet(strv->str_word->value[i])) {
      if (cur->sflag) {
        string_append(cur->singlets, strv->str_word->value[i]);
      } else {
        for (int j = 0; j < cur->singlets->length; j++) {
          if (strv->str_word->value[i] == cur->singlets->value[j]) {
            for (int k = j; k < cur->singlets->length; k++) {
              cur->singlets->value[k] = cur->singlets->value[k + 1];
            }
            cur->singlets->length--;
            break;
          }
        }
      }
    }
  }
  value_free_safe(strc);
}

void cog_undelim(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *strc = stack_pop(cur->stack);
  if (!strc) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  if (strc->container->stack->size != 1) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(cur->stack, strc);
    return;
  }
  value_t *strv = strc->container->stack->items[0];
  if (strv->type != VWORD) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(cur->stack, strc);
    return;
  }
  if (!strv->str_word->length) {
    value_free_safe(strc);
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
  value_free_safe(strc);
}

void cog_unignore(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *strc = stack_pop(cur->stack);
  if (!strc) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  if (strc->container->stack->size != 1) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(cur->stack, strc);
    return;
  }
  value_t *strv = strc->container->stack->items[0];
  if (strv->type != VWORD) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(cur->stack, strc);
    return;
  }
  if (!strv->str_word->length) {
    value_free_safe(strc);
    return;
  }
  if (!cur->ignored) {
    cur->ignored = init_string("");
  }
  for (int i = 0; i < strv->str_word->length; i++) {
    if (isignore(strv->str_word->value[i])) {
      if (cur->iflag) {
        for (int j = 0; j < cur->ignored->length; j++) {
          if (strv->str_word->value[i] == cur->ignored->value[j]) {
            for (int k = j; k < cur->ignored->length; k++) {
              cur->ignored->value[k] = cur->ignored->value[k + 1];
            }
            cur->ignored->length--;
            break;
          }
        }
      } else {
        string_append(cur->ignored, strv->str_word->value[i]);
      }
    }
  }
  value_free_safe(strc);
}

void cog_unsinglet(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *strc = stack_pop(cur->stack);
  if (!strc) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  if (strc->container->stack->size != 1) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(cur->stack, strc);
    return;
  }
  value_t *strv = strc->container->stack->items[0];
  if (strv->type != VWORD) {
    eval_error("BAD ARGUMENT TYPE", v);
    stack_push(cur->stack, strc);
    return;
  }
  if (!strv->str_word->length) {
    value_free_safe(strc);
    return;
  }
  if (!cur->singlets) {
    cur->singlets = init_string("");
  }
  for (int i = 0; i < strv->str_word->length; i++) {
    if (issinglet(strv->str_word->value[i])) {
      if (cur->sflag) {
        for (int j = 0; j < cur->singlets->length; j++) {
          if (strv->str_word->value[i] == cur->singlets->value[j]) {
            for (int k = j; k < cur->singlets->length; k++) {
              cur->singlets->value[k] = cur->singlets->value[k + 1];
            }
            cur->singlets->length--;
            break;
          }
        }
      } else {
        string_append(cur->singlets, strv->str_word->value[i]);
      }
    }
  }
  value_free_safe(strc);
}

void cog_evalstr(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *strc = stack_pop(cur->stack);
  if (!strc) {
    eval_error("TOO FEW ARGUMENTS", v);
    return;
  }
  for (int i = 0; i < strc->container->stack->size; i++) {
    value_t *str = strc->container->stack->items[i];
    parser_t *parser = init_parser(str->str_word->value);
    while(1) {
      value_t *w = parser_get_next(parser);
      if (w == NULL)
        break;
      eval(w);
      if (STACK == NULL)
        break;
    }
    free(parser);
    if (STACK == NULL) break;
  }
  value_free_safe(strc);
}

void add_funcs_parser(ht_t* flit) {
  add_func(flit, cog_getf, "getf");
  add_func(flit, cog_setf, "setf");
  add_func(flit, cog_aliasf, "aliasf");
  add_func(flit, cog_unaliasf, "unaliasf");
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
  add_func(flit, cog_ignore, "ignore");
  add_func(flit, cog_singlet, "singlet");
  add_func(flit, cog_undelim, "undelim");
  add_func(flit, cog_unignore, "unignore");
  add_func(flit, cog_unsinglet, "unsinglet");
  add_func(flit, cog_evalstr, "evalstr");
}
