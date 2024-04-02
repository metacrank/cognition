#include <builtins/io.h>
#include <builtinslib.h>
#include <stdio.h>
#include <macros.h>
#include <string.h>

extern stack_t *STACK;
extern byte_t print_buffer[5];

void cog_questionmark(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  printf("%sSTACK:%s\n", GRN, COLOR_RESET);
  for (int i = 0; i < cur->stack->size; i++) {
    print_value(cur->stack->items[i], "\n");
  }
  printf("\n");
}

void cog_period(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *elt = stack_pop(cur->stack);
  if (elt == NULL) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  print_value(elt, "\n");
  value_free_safe(elt);
}

void cog_print(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *v1 = stack_pop(cur->stack);
  if (v1 == NULL) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  if (value_stack(v1)[0]->size == 0) {
    stack_push(cur->stack, v1);
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  value_t *word = value_stack(v1)[0]->items[0];
  if (word->type != VWORD) {
    stack_push(cur->stack, v1);
    eval_error(U"BAD ARGUMENT TYPE", v);
    return;
  }
  print(word->str_word);
  value_free_safe(v1);
}

void cog_read(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *strval = init_value(VWORD);
  strval->str_word = get_line(stdin);
  push_quoted(cur, strval);
}

void cog_fquestionmark(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *fileq = stack_pop(cur->stack);
  if (!fileq) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  if (value_stack(fileq)[0]->size != 1) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(cur->stack, fileq);
    return;
  }
  value_t *fileword = value_stack(fileq)[0]->items[0];
  if (fileword->type != VWORD) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(cur->stack, fileq);
    return;
  }
  string_t *filename = fileword->str_word;
  size_t strlength = 0;
  for (long i = 0; i < filename->length; i++) {
    utf32_to_utf8(print_buffer, filename->value[i]);
    strlength += sizeof_utf8(print_buffer);
  }
  char *buf = malloc((strlength + 1) * sizeof(char));
  strlength = 0;
  for (long i = 0; i < filename->length; i++) {
    utf32_to_utf8(print_buffer, filename->value[i]);
    memcpy(buf + strlength, print_buffer, sizeof_utf8(print_buffer));
    strlength += sizeof_utf8(print_buffer);
  }
  buf[strlength] = '\0';
  FILE *f = fopen(buf, "ab");
  free(buf);
  if (!f) {
    eval_error(U"INVALID FILENAME", v);
    stack_push(cur->stack, fileq);
    return;
  }
  fprintf(f, "STACK:\n");
  for (int i = 0; i < cur->stack->size; i++) {
    fprint_value(f, cur->stack->items[i], "\n");
  }
  fclose(f);
  value_free_safe(fileq);
}

void cog_fperiod(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  if (cur->stack->size < 2) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *fileq = stack_pop(cur->stack);
  if (value_stack(fileq)[0]->size != 1) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(cur->stack, fileq);
    return;
  }
  value_t *fileword = value_stack(fileq)[0]->items[0];
  if (fileword->type != VWORD) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(cur->stack, fileq);
    return;
  }
  string_t *filename = fileword->str_word;
  size_t strlength = 0;
  for (long i = 0; i < filename->length; i++) {
    utf32_to_utf8(print_buffer, filename->value[i]);
    strlength += sizeof_utf8(print_buffer);
  }
  char *buf = malloc((strlength + 1) * sizeof(char));
  strlength = 0;
  for (long i = 0; i < filename->length; i++) {
    utf32_to_utf8(print_buffer, filename->value[i]);
    memcpy(buf + strlength, print_buffer, sizeof_utf8(print_buffer));
    strlength += sizeof_utf8(print_buffer);
  }
  buf[strlength] = '\0';
  FILE *f = fopen(buf, "ab");
  free(buf);
  if (!f) {
    eval_error(U"INVALID FILENAME", v);
    stack_push(cur->stack, fileq);
    return;
  }
  value_t *printitem = stack_pop(cur->stack);
  fprint_value(f, printitem, "\n");
  fclose(f);
  value_free_safe(fileq);
  value_free_safe(printitem);
}

void cog_fread(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  value_t *fileq = stack_pop(cur->stack);
  if (!fileq) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  if (value_stack(fileq)[0]->size != 1) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(cur->stack, fileq);
    return;
  }
  value_t *fileword = value_stack(fileq)[0]->items[0];
  if (fileword->type != VWORD) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(cur->stack, fileq);
    return;
  }
  string_t *filename = fileword->str_word;
  size_t strlength = 0;
  for (long i = 0; i < filename->length; i++) {
    utf32_to_utf8(print_buffer, filename->value[i]);
    strlength += sizeof_utf8(print_buffer);
  }
  char *buf = malloc((strlength + 1) * sizeof(char));
  strlength = 0;
  for (long i = 0; i < filename->length; i++) {
    utf32_to_utf8(print_buffer, filename->value[i]);
    memcpy(buf + strlength, print_buffer, sizeof_utf8(print_buffer));
    strlength += sizeof_utf8(print_buffer);
  }
  buf[strlength] = '\0';
  FILE *f = fopen(buf, "rb");
  free(buf);
  if (!f) {
    eval_error(U"INVALID FILENAME", v);
    stack_push(cur->stack, fileq);
    return;
  }
  value_t *fileread = init_value(VWORD);
  fileread->str_word = file_read(f);
  fclose(f);
  value_free_safe(fileq);
  push_quoted(cur, fileread);
}

void cog_fwrite(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  if (cur->stack->size < 2) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *fileq = stack_pop(cur->stack);
  value_t *stringq = stack_pop(cur->stack);
  if (value_stack(fileq)[0]->size != 1 || value_stack(stringq)[0]->size != 1) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(cur->stack, stringq);
    stack_push(cur->stack, fileq);
    return;
  }
  value_t *fileword = value_stack(fileq)[0]->items[0];
  value_t *stringval = value_stack(stringq)[0]->items[0];
  if (fileword->type != VWORD || stringval->type != VWORD) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(cur->stack, stringq);
    stack_push(cur->stack, fileq);
    return;
  }
  string_t *filename = fileword->str_word;
  size_t strlength = 0;
  for (long i = 0; i < filename->length; i++) {
    utf32_to_utf8(print_buffer, filename->value[i]);
    strlength += sizeof_utf8(print_buffer);
  }
  char *buf = malloc((strlength + 1) * sizeof(char));
  strlength = 0;
  for (long i = 0; i < filename->length; i++) {
    utf32_to_utf8(print_buffer, filename->value[i]);
    memcpy(buf + strlength, print_buffer, sizeof_utf8(print_buffer));
    strlength += sizeof_utf8(print_buffer);
  }
  buf[strlength] = '\0';
  FILE *f = fopen(buf, "wb");
  free(buf);
  if (!f) {
    eval_error(U"INVALID FILENAME", v);
    stack_push(cur->stack, stringq);
    stack_push(cur->stack, fileq);
    return;
  }
  fprint(f, stringval->str_word);
  fclose(f);
  value_free_safe(fileq);
  value_free_safe(stringq);
}

void cog_fprint(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  if (cur->stack->size < 2) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  value_t *fileq = stack_pop(cur->stack);
  value_t *stringq = stack_pop(cur->stack);
  if (value_stack(fileq)[0]->size != 1 || value_stack(stringq)[0]->size != 1) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(cur->stack, stringq);
    stack_push(cur->stack, fileq);
    return;
  }
  value_t *fileword = value_stack(fileq)[0]->items[0];
  value_t *stringval = value_stack(stringq)[0]->items[0];
  if (fileword->type != VWORD || stringval->type != VWORD) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(cur->stack, stringq);
    stack_push(cur->stack, fileq);
    return;
  }
  string_t *filename = fileword->str_word;
  size_t strlength = 0;
  for (long i = 0; i < filename->length; i++) {
    utf32_to_utf8(print_buffer, filename->value[i]);
    strlength += sizeof_utf8(print_buffer);
  }
  char *buf = malloc((strlength + 1) * sizeof(char));
  strlength = 0;
  for (long i = 0; i < filename->length; i++) {
    utf32_to_utf8(print_buffer, filename->value[i]);
    memcpy(buf + strlength, print_buffer, sizeof_utf8(print_buffer));
    strlength += sizeof_utf8(print_buffer);
  }
  buf[strlength] = '\0';
  FILE *f = fopen(buf, "ab");
  free(buf);
  if (!f) {
    eval_error(U"INVALID FILENAME", v);
    stack_push(cur->stack, stringq);
    stack_push(cur->stack, fileq);
    return;
  }
  fprint(f, stringval->str_word);
  fclose(f);
  value_free_safe(fileq);
  value_free_safe(stringq);
}

void add_funcs_io(ht_t* flit) {
  add_func(flit, cog_questionmark, U"?");
  add_func(flit, cog_period, U".");
  add_func(flit, cog_print, U"print");
  add_func(flit, cog_read, U"read");
  add_func(flit, cog_fquestionmark, U"f?");
  add_func(flit, cog_fperiod, U"f.");
  add_func(flit, cog_fread, U"fread");
  add_func(flit, cog_fwrite, U"fwrite");
  add_func(flit, cog_fprint, U"fprint");
}
