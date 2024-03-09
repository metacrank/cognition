#ifndef BUILTINSLIB_H_
#define BUILTINSLIB_H_

#include <cognition.h>
#include <ctype.h>

bool strisint(string_t *s);

bool word_truth(value_t *v);

void contain_copy_attributes(contain_t *c, contain_t *newc);

#endif // BUILTINSLIB_H_
