#ifndef BUILTINSLIB_H_
#define BUILTINSLIB_H_

#include <cognition.h>

bool strisint(string_t *s);

bool word_truth(value_t *v);

void contain_copy_attributes(contain_t *c, contain_t *newc);

void print_value(value_t *v, const char *end);

#endif // BUILTINSLIB_H_
