#ifndef BUILTINSLIB_H_
#define BUILTINSLIB_H_

#include <cognition.h>
#include <stdio.h>

bool strisint(string_t *s);

bool word_truth(value_t *v);

stack_t **value_stack(value_t *v);

void contain_copy_attributes(contain_t *c, contain_t *newc);

void print_str_formatted(string_t *string);

void print_value(value_t *v, const char *end);

char *get_line(FILE *f);

void nop(void *v);

void value_free_safe(void *v);

void contain_def_stack_push(void *v);

void macro_def_stack_push(void *v);

#endif // BUILTINSLIB_H_
