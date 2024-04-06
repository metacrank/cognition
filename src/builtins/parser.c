#include <builtins/parser.h>
#include <builtinslib.h>
#include <macros.h>
#include <stdio.h>
#include <string.h>

extern stack_t *STACK;
extern stack_t *FAMILY;
extern string_t *FAMILY_IDX;
extern bool EXITED;

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
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  stack_t *lstack = *value_stack(list);
  if (lstack->size == 0) {
    value_stack_free(cur->faliases);
    cur->faliases = NULL;
    value_free_safe(list);
    return;
  }
  for (int i = 0; i < lstack->size; i++) {
    value_t *val = lstack->items[i];
    if (val->type != VWORD) {
      eval_error(U"BAD ARGUMENT TYPE", v);
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
  for (int i = 0; i < lstack->size; i++) {
    value_t *val = lstack->items[i];
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
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  stack_t *lstack = *value_stack(list);
  if (lstack->size == 0) {
    value_free_safe(list);
    return;
  }
  for (int i = 0; i < lstack->size; i++) {
    value_t *val = lstack->items[i];
    if (val->type != VWORD) {
      eval_error(U"BAD ARGUMENT TYPE", v);
      stack_push(cur->stack, list);
      return;
    }
  }
  if (cur->faliases == NULL)
    cur->faliases = init_stack(DEFAULT_STACK_SIZE);
  for (int i = 0; i < lstack->size; i++) {
    value_t *val = lstack->items[i];
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
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  stack_t *lstack = *value_stack(list);
  if (lstack->size == 0) return;
  for (int i = 0; i < lstack->size; i++) {
    value_t *val = lstack->items[i];
    if (val->type != VWORD) {
      eval_error(U"BAD ARGUMENT TYPE", v);
      stack_push(cur->stack, list);
      return;
    }
  }
  if (cur->faliases == NULL) return;
  if (cur->faliases->size == 0) return;
  for (int i = 0; i < lstack->size; i++) {
    value_t *val = lstack->items[i];
    for (int j = 0; j < cur->faliases->size; j++) {
      string_t *str = cur->faliases->items[j];
      if (string_comp(val->str_word, str) == 0)
        string_free(stack_popdeep(cur->faliases, j));
    }
  }
  value_free_safe(list);
}

void cog_d(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *stack = stack_pop(cur->stack);
  if (stack == NULL) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  if (value_stack(stack)[0]->size != 1) {
    stack_push(cur->stack, stack);
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  value_t *word = value_stack(stack)[0]->items[0];
  if (word->type != VWORD) {
    stack_push(cur->stack, stack);
    eval_error(U"BAD ARGUMENT TYPE", v);
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
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  if (value_stack(stack)[0]->size != 1) {
    stack_push(cur->stack, stack);
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  value_t *word = value_stack(stack)[0]->items[0];
  if (word->type != VWORD) {
    stack_push(cur->stack, stack);
    eval_error(U"BAD ARGUMENT TYPE", v);
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
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  if (value_stack(stack)[0]->size != 1) {
    stack_push(cur->stack, stack);
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  value_t *word = value_stack(stack)[0]->items[0];
  if (word->type != VWORD) {
    stack_push(cur->stack, stack);
    eval_error(U"BAD ARGUMENT TYPE", v);
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
    list->str_word = init_string(U"");
  push_quoted(cur, list);
}

void cog_getd(value_t *v) {
  value_t *list = init_value(VWORD);
  contain_t *cur = stack_peek(STACK);
  list->str_word = string_copy(cur->delims);
  if (list->str_word == NULL)
    list->str_word = init_string(U"");
  push_quoted(cur, list);
}

void cog_gets(value_t *v) {
  value_t *list = init_value(VWORD);
  contain_t *cur = stack_peek(STACK);
  list->str_word = string_copy(cur->singlets);
  if (list->str_word == NULL)
    list->str_word = init_string(U"");
  push_quoted(cur, list);
}

void cog_delim(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *strc = stack_pop(cur->stack);
  if (!strc) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  if (value_stack(strc)[0]->size != 1) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(cur->stack, strc);
    return;
  }
  value_t *strv = value_stack(strc)[0]->items[0];
  if (strv->type != VWORD) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(cur->stack, strc);
    return;
  }
  if (!strv->str_word->length) {
    value_free_safe(strc);
    return;
  }
  if (!cur->delims) {
    cur->delims = init_string(U"");
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
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  if (value_stack(strc)[0]->size != 1) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(cur->stack, strc);
    return;
  }
  value_t *strv = value_stack(strc)[0]->items[0];
  if (strv->type != VWORD) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(cur->stack, strc);
    return;
  }
  if (!strv->str_word->length) {
    value_free_safe(strc);
    return;
  }
  if (!cur->ignored) {
    cur->ignored = init_string(U"");
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
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  if (value_stack(strc)[0]->size != 1) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(cur->stack, strc);
    return;
  }
  value_t *strv = value_stack(strc)[0]->items[0];
  if (strv->type != VWORD) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(cur->stack, strc);
    return;
  }
  if (!strv->str_word->length) {
    value_free_safe(strc);
    return;
  }
  if (!cur->singlets) {
    cur->singlets = init_string(U"");
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
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  if (value_stack(strc)[0]->size != 1) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(cur->stack, strc);
    return;
  }
  value_t *strv = value_stack(strc)[0]->items[0];
  if (strv->type != VWORD) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(cur->stack, strc);
    return;
  }
  if (!strv->str_word->length) {
    value_free_safe(strc);
    return;
  }
  if (!cur->delims) {
    cur->delims = init_string(U"");
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
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  if (value_stack(strc)[0]->size != 1) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(cur->stack, strc);
    return;
  }
  value_t *strv = value_stack(strc)[0]->items[0];
  if (strv->type != VWORD) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(cur->stack, strc);
    return;
  }
  if (!strv->str_word->length) {
    value_free_safe(strc);
    return;
  }
  if (!cur->ignored) {
    cur->ignored = init_string(U"");
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
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  if (value_stack(strc)[0]->size != 1) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(cur->stack, strc);
    return;
  }
  value_t *strv = value_stack(strc)[0]->items[0];
  if (strv->type != VWORD) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(cur->stack, strc);
    return;
  }
  if (!strv->str_word->length) {
    value_free_safe(strc);
    return;
  }
  if (!cur->singlets) {
    cur->singlets = init_string(U"");
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
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  string_append(FAMILY_IDX, 0);
  if (strc->type == VSTACK)
    stack_push(FAMILY, strc->container);
  else stack_push(FAMILY, NULL);
  string_append(FAMILY_IDX, FAMILY->size - 1);
  stack_t *strst = *value_stack(strc);
  for (int i = 0; i < strst->size; i++) {
    value_t *str = strst->items[i];
    parser_t *parser = init_parser(str->str_word);
    while(1) {
      value_t *w = parser_get_next(parser);
      if (w == NULL)
        break;
      eval(w);
      if (EXITED)
        break;
    }
    free(parser);
    if (EXITED) break;
  }
  stack_pop(FAMILY);
  FAMILY_IDX->length -= 2;
  value_free_safe(strc);
}

void cog_strstack(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *strc = stack_pop(cur->stack);
  if (!strc) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *quot = init_value(VSTACK);
  quot->container = init_contain(NULL, NULL, NULL);
  stack_t *qstack = quot->container->stack;
  for (int i = 0; i < value_stack(strc)[0]->size; i++) {
    value_t *str = value_stack(strc)[0]->items[i];
    parser_t *parser = init_parser(str->str_word);
    while(1) {
      value_t *w = parser_get_next(parser);
      if (w == NULL)
        break;
      stack_push(qstack, w);
    }
    free(parser);
  }
  value_free_safe(strc);
  stack_push(cur->stack, quot);
}

void add_funcs_parser(ht_t* flit) {
  add_func(flit, cog_getf, U"getf");
  add_func(flit, cog_setf, U"setf");
  add_func(flit, cog_aliasf, U"aliasf");
  add_func(flit, cog_unaliasf, U"unaliasf");
  add_func(flit, cog_d, U"d");
  add_func(flit, cog_i, U"i");
  add_func(flit, cog_s, U"s");
  add_func(flit, cog_dtgl, U"dtgl");
  add_func(flit, cog_itgl, U"itgl");
  add_func(flit, cog_stgl, U"stgl");
  add_func(flit, cog_geti, U"geti");
  add_func(flit, cog_getd, U"getd");
  add_func(flit, cog_gets, U"gets");
  add_func(flit, cog_delim, U"delim");
  add_func(flit, cog_ignore, U"ignore");
  add_func(flit, cog_singlet, U"singlet");
  add_func(flit, cog_undelim, U"undelim");
  add_func(flit, cog_unignore, U"unignore");
  add_func(flit, cog_unsinglet, U"unsinglet");
  add_func(flit, cog_evalstr, U"evalstr");
  add_func(flit, cog_strstack, U"strstack");
}
