#include <builtinslib.h>
#include <ctype.h>
#include <macros.h>
#include <stdio.h>
#include <string.h>

extern ht_t *OBJ_TABLE;
extern stack_t *CONTAIN_DEF_STACK;
extern stack_t *MACRO_DEF_STACK;

bool word_truth(value_t *v) {
  string_t *str = v->str_word;
  return str->length;
}

stack_t **value_stack(value_t *v) {
  if (v->type == VSTACK)
    return &v->container->stack;
  else if (v->type == VMACRO)
    return &v->macro;
  printf("value_stack error\n");
  die("BAD VALUE ON STACK");
  return NULL;
}

void contain_copy_attributes(contain_t *c, contain_t *newc) {
  if (c == NULL) {
    return;
  }
  newc->flit = ht_copy(c->flit, value_stack_copy);
  newc->word_table = ht_copy(c->word_table, contain_value_copy);
  newc->cranks = stack_copy(c->cranks, cranks_copy);
  newc->faliases = stack_copy(c->faliases, falias_copy);
  newc->delims = string_copy(c->delims);
  newc->ignored = string_copy(c->ignored);
  newc->singlets = string_copy(c->singlets);
  newc->dflag = c->dflag;
  newc->iflag = c->iflag;
  newc->sflag = c->sflag;
}

void print_str_formatted(string_t *string) {
  fprint_str_formatted(stdout, string);
}

void fprint_str_formatted(FILE *f, string_t *string) {
  if (!string)
    return;
  for (int i = 0; i < string->length; i++) {
    if (string->value[i] == '\n') {
      fprintf(f, "\\n");
    } else if (string->value[i] == '\t') {
      fprintf(f, "\\t");
    } else {
      fprint_utf32(f, 1, string->value[i]);
    }
  }
}

void print_value(value_t *v, void *e) {
  fprint_value(stdout, v, e);
}

void fprint_value(FILE *f, value_t *v, void *e) {
  byte_t *end = e;
  custom_t *c;
  switch (v->type) {
  case VWORD:
    if (v->str_word) {
      fprintf(f, "'");
      fprint_str_formatted(f, v->str_word);
      fprintf(f, "'");
    } else {
      fprintf(f, "(null)");
    }
    break;
  case VSTACK:
    fprintf(f, "[ ");
    for (int i = 0; i < v->container->stack->size; i++) {
      fprint_value(f, v->container->stack->items[i], " ");
    }
    fprintf(f, "]");
    break;
  case VMACRO:
    fprintf(f, "( ");
    for (int i = 0; i < v->macro->size; i++) {
      fprint_value(f, v->macro->items[i], " ");
    }
    fprintf(f, ")");
    break;
  case VERR:
    fprintf(f, "'");
    fprint(f, v->error->str_word);
    fprintf(f, "':%s", RED);
    fprint(f, v->error->error);
    fprintf(f, "%s", COLOR_RESET);
    break;
  case VCUSTOM:
    c = ht_get(OBJ_TABLE, v->str_word);
    c->printfunc(f, v->custom);
    break;
  case VCLIB:
    fprintf(f, "CLIB_FUNC");
  }
  fprintf(f, "%s", end);
}

string_t *get_line(FILE *f) {
  if (!f) return NULL;
  string_t *s = init_string(U"");
  byte_t b;
  int c;
  char32_t utf32;
  while ((c = fgetc(f)) != '\n') {
    b = c;
    switch (sizeof_utf8(&b)) {
      case 1:
        utf32 = b;
        break;
      case 2:
        utf32 = b - 0xC0;
        utf32 *= 0x40;
        if ((c = fgetc(f)) == EOF) return s;
        utf32 += c - 0x80;
        break;
      case 3:
        utf32 = b - 0xE0;
        for (int _ = 0; _ < 2; _++) {
          utf32 *= 0x40;
          if ((c = fgetc(f)) == EOF) return s;
          utf32 += c - 0x80;
        }
      case 4:
        utf32 = b - 0xF0;
        for (int _ = 0; _ < 3; _++) {
          utf32 *= 0x40;
          if ((c = fgetc(f)) == EOF) return s;
          utf32 += c - 0x80;
        }
        break;
    }
    string_append(s, utf32);
  }
  return s;
}

void nop(void *v) {}

void value_free_safe(void *vtmp) {
  if (vtmp == NULL)
    return;
  value_t *v = (value_t *)vtmp;
  if (v == NULL)
    return;
  if (v->type == VWORD || v->type == VCLIB || v->type == VCUSTOM) {
    string_free(v->str_word);
  }
  if (v->type == VSTACK) {
    stack_push(CONTAIN_DEF_STACK, v->container);
  }
  if (v->type == VMACRO) {
    stack_push(MACRO_DEF_STACK, v->macro);
  }
  if (v->type == VERR) {
    error_free(v->error);
  }
  if (v->type == VCUSTOM) {
    custom_t *cstm = ht_get(OBJ_TABLE, v->str_word);
    cstm->freefunc(v->custom);
  }
  free(v);
}

void contain_def_stack_push(void *c) {
  if (c)
    stack_push(CONTAIN_DEF_STACK, c);
}

void macro_def_stack_push(void *m) {
  if (m)
    stack_push(MACRO_DEF_STACK, m);
}
