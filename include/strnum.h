#ifndef STRNUM_H_
#define STRNUM_H_
#include <better_string.h>
#include <macros.h>
#include <stdbool.h>
#include <tgmath.h>

void init_math();

void math_free();

bool isfloat(string_t *s);

bool isint(string_t *s);

long order(string_t *s);

long string_to_int(string_t *s);

double complex string_to_double(string_t *s);

string_t *int_to_string(long d);

string_t *double_to_string(double complex f);

string_t *real(string_t *s);

string_t *imaginary(string_t *s);

string_t *neg(string_t *s);

string_t *ip(string_t *s);

string_t *fp(string_t *s);

bool sum_positive(string_t *m, string_t *n);

// returns the sum of m and n, where:
// m_radix is a pointer to the radix point in m, if known
// n_radix is a pointer to the radix point in n, if known
// sum_buffer has a buffer to put the sum in, if relevant
// sb_radix holds a pointer to the radix point (0th place value) in the buffer, if sum_buffer is not NULL
string_t *sum(string_t *m, string_t *n, char32_t *m_radix, char32_t *n_radix,
              string_t *sum_buffer, char32_t *sb_radix);

string_t *diff(string_t *m, string_t *n);

string_t *product(string_t *m, string_t *n);

string_t *quotient(string_t *m, string_t *n);

string_t *str_sqrt(string_t *m);

string_t *gaussian(string_t *m);

string_t *str_exp(string_t *m);

string_t *str_ln(string_t *m);

string_t *str_pow(string_t *m, string_t *n);

string_t *str_sin(string_t *m);

string_t *str_cos(string_t *m);

string_t *str_ceil(string_t *m);

string_t *str_floor(string_t *m);

string_t *geq(string_t *m, string_t *n);

string_t *leq(string_t *m, string_t *n);

string_t *gthan(string_t *m, string_t *n);

string_t *lthan(string_t *m, string_t *n);

#endif // STRNUM_H_
