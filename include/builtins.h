#ifndef BUILTINS_H_
#define BUILTINS_H_
#include <stem.h>

void stemadd(value_t *v);
void stemsub(value_t *v);
void stemmul(value_t *v);
void stemdiv(value_t *v);
void stemfunc(value_t *v);
void stempow(value_t *v);
void stemsin(value_t *v);
void stemcos(value_t *v);
void stemexp(value_t *v);
void stemeval(value_t *v);
void stemln(value_t *v);
void stemceil(value_t *v);
void stemfloor(value_t *v);
void strquote(value_t *v);
void curry(value_t *v);
void nop(value_t *v);
void stemfread(value_t *v);
void stemread(value_t *v);
void stemexit(value_t *v);
void quote(value_t *v);
void stemtype(value_t *v);
void dsc(value_t *v);
void swap(value_t *v);
void isdef(value_t *v);
void stemdup(value_t *v);
void questionmark(value_t *v);
void period(value_t *v);
void stemlen(value_t *v);
void dip(value_t *v);
void keep(value_t *v);
void del(value_t *v);
void clear(value_t *v);
void stemif(value_t *v);
void gtequals(value_t *v);
void ltequals(value_t *v);
void gthan(value_t *v);
void lthan(value_t *v);
void equals(value_t *v);
void nequals(value_t *v);
void wtostr(value_t *v);
void compose(value_t *v);
void isnum(value_t *v);
void stoi(value_t *v);
void ssize(value_t *v);
void qstack(value_t *v);
void vat(value_t *v);
void stemfwrite(value_t *v);

void add_funcs();
#endif // BUILTINS_H_
