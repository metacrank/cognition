#include <strnum.h>
#include <better_string.h>
#include <macros.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#define BASE DEFAULT_NUMBER_BASE
#define B_ODD (BASE % 2)
#define N_O_D (BASE + 2 * (1 - B_ODD))
#define RADIUS (BASE / 2 + 1)
#define MODULO 13

// in the future, do not use pointer arrays for utf32
char32_t digits[N_O_D];
int digit_values[MODULO];
char32_t negation_table[MODULO];
char32_t addition_carry_table[MODULO][MODULO];
char32_t addition_sum_table[MODULO][MODULO];
char32_t subtraction_carry_table[MODULO][MODULO];
char32_t subtraction_sum_table[MODULO][MODULO];
char32_t multiplication_table[MODULO][MODULO];
char32_t multiplication_table[MODULO][MODULO];
char32_t srt_division_table[MODULO][MODULO];

int di(char32_t d) {
  return d % MODULO;
}

int didx(int i) {
  return di(digits[i]);
}

void init_digits() {
  if (BASE > 11)
    die("unsupported number base");
  char32_t d_array[] =
    { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
  char32_t d_negative_zero = 0x014C;
  char32_t d_negative_five = 0x218A;
  char32_t *ptr = d_array;
  int utf8size;
  int len = 0;
  for (int i = 0; i < RADIUS; i++) {
    digits[i] = *ptr;
    ptr++;
  }
  ptr = d_array + BASE - 1;
  if (!B_ODD) {
    digits[RADIUS] = d_negative_zero;
  }
  for (int i = RADIUS + (1 - B_ODD); i < RADIUS + (1 - B_ODD) + 4; i++) {
    digits[i] = *ptr;
    ptr--;
  }
  if (RADIUS + (1 - B_ODD) + 4 < N_O_D) {
    digits[RADIUS + (1 - B_ODD) + 4] = d_negative_five;
  }
  if (DEBUG) {
    printf("init digits: ");
    for (int i = 0; i < N_O_D; i++)
      print_utf32(1, digits[i]);
    printf("\n");
  }
}

void init_neg() {
  negation_table[di(digits[0])] = digits[RADIUS * (1 - B_ODD)];
  for (int i = 1; i < RADIUS; i++) {
    negation_table[di(digits[i])] = digits[RADIUS - B_ODD + i];
  }
  for (int i = RADIUS; i < N_O_D; i++) {
    negation_table[di(digits[i])] = digits[i - RADIUS + B_ODD];
  }
  if (DEBUG) {
    printf("init neg: ");
    for (int i = 0; i < N_O_D; i++) {
      print_utf32(1, negation_table[di(digits[i])]);
    }
    printf("\n");
  }
}

void init_digit_values() {
  for (int i = 0; i < RADIUS; i++) {
    digit_values[didx(i)] = i;
  }
  for (int i = RADIUS; i < N_O_D; i++) {
    digit_values[didx(i)] = RADIUS - i - B_ODD;
  }
}

void init_add() {
  int sum, carry;
  int b, a = 0;
  for (int i = 0; i < RADIUS; i++) {
    b = 0;
    for (int j = 0; j < RADIUS; j++) {
      sum = i + j;
      if (!B_ODD && sum == BASE / 2) {
        addition_carry_table[didx(a)][didx(b)] = digits[1];
        addition_sum_table[didx(a)][didx(b)] = digits[RADIUS + sum];
      } else {
        carry = sum/RADIUS;
        addition_carry_table[didx(a)][didx(b)] = digits[carry];
        sum -= carry * BASE;
        if (sum >= 0)
          addition_sum_table[didx(a)][didx(b)] = digits[sum];
        else
          addition_sum_table[didx(a)][didx(b)] = digits[RADIUS - B_ODD - sum];
      }
      b++;
    }
    for (int j = B_ODD; j < RADIUS; j++) {
      sum = i - j;
      if (!B_ODD && sum == BASE / 2) {
        addition_carry_table[didx(a)][didx(b)] = digits[1];
        addition_sum_table[didx(a)][didx(b)] = digits[RADIUS + sum];
      }
      else {
        addition_carry_table[didx(a)][didx(b)] = digits[RADIUS * (1 - B_ODD) * (sum < 0)];
        if (sum >= 0) {
          addition_sum_table[didx(a)][didx(b)] = digits[sum];
        } else {
          addition_sum_table[didx(a)][didx(b)] = digits[RADIUS - B_ODD - sum];
        }
      }
      b++;
    }
    a++;
  }
  for (int i = B_ODD; i < RADIUS; i++) {
    b = 0;
    for (int j = 0; j < RADIUS; j++) {
      sum = j - i;
      if (!B_ODD && sum == - BASE / 2) {
        addition_carry_table[didx(a)][didx(b)] = digits[RADIUS + 1];
        addition_sum_table[didx(a)][didx(b)] = digits[-sum];
      }
      else {
        addition_carry_table[didx(a)][didx(b)] = digits[RADIUS * (1 - B_ODD) * (sum <= 0)];
        if (sum > 0)
          addition_sum_table[didx(a)][didx(b)] = digits[sum];
        else if (sum == 0)
          addition_sum_table[didx(a)][didx(b)] = digits[RADIUS * (1 - B_ODD)];
        else
          addition_sum_table[didx(a)][didx(b)] = digits[RADIUS - B_ODD - sum];
      }
      b++;
    }
    for (int j = B_ODD; j < RADIUS; j++) {
      sum = i + j;
      if (!B_ODD && sum == BASE / 2) {
        addition_carry_table[didx(a)][didx(b)] = digits[RADIUS + 1];
        addition_sum_table[didx(a)][didx(b)] = digits[sum];
      } else {
        carry = sum/RADIUS;
        addition_carry_table[didx(a)][didx(b)] = digits[RADIUS + carry - B_ODD];
        sum -= carry * BASE;
        if (sum >= 0)
          addition_sum_table[didx(a)][didx(b)] = digits[RADIUS - B_ODD + sum];
        else
          addition_sum_table[didx(a)][didx(b)] = digits[-sum];
      }
      b++;
    }
    a++;
  }
  if (DEBUG) {
    printf("init add:\n");
    // print out resulting table
    for (int i = 0; i < N_O_D; i++) {
      for (int j = 0; j < N_O_D; j++) {
        char32_t b1 = addition_carry_table[didx(i)][didx(j)];
        char32_t b2 = addition_sum_table[didx(i)][didx(j)];
        print_utf32(2, b1, b2);
        printf(" ");
      }
      printf("\n");
    }
  }
}

void init_sub() {
  int m, n, r;
  for (int i = 0; i < N_O_D; i++) {
    for (int j = 0; j < N_O_D; j++) {
      m = di(digits[i]);
      n = di(digits[j]);
      r = di(negation_table[di(digits[j])]);
      subtraction_carry_table[m][n] = addition_carry_table[m][r];
      subtraction_sum_table[m][n] = addition_sum_table[m][r];
    }
  }
  if (DEBUG) {
    printf("init sub:\n");
    // print out resulting table
    for (int i = 0; i < N_O_D; i++) {
      for (int j = 0; j < N_O_D; j++) {
        char32_t b1 = subtraction_carry_table[didx(i)][didx(j)];
        char32_t b2 = subtraction_sum_table[didx(i)][didx(j)];
        print_utf32(2, b1, b2);
        printf(" ");
      }
      printf("\n");
    }
  }
}

void init_mul() {
  int product, carry;
  int b, a = 0;
  for (int i = 0; i < RADIUS; i++) {
    b = 0;
    for (int j = 0; j < RADIUS; j++) {

      b++;
    }
    for (int j = B_ODD; j < RADIUS; j++) {

      b++;
    }
    a++;
  }
  for (int i = B_ODD; i < RADIUS; i++) {
    b = 0;
    for (int j = 0; j < RADIUS; j++) {

      b++;
    }
    for (int j = B_ODD; j < RADIUS; j++) {

      b++;
    }
    a++;
  }
}

void init_math() {
  init_digits();
  init_digit_values();
  init_neg();
  init_add();
  init_sub();
}

void math_free() {}

bool isfloat(string_t *s) {
  for (long i = 0; i < s->length; i++)
    if (!isdigit(s->value[i]) && s->value[i] != '.') return false;
  return true;
}

bool isint(string_t *s) {
  for (long i = 0; i < s->length; i++)
    if (!isdigit(s->value[i])) return false;
  return true;
}

long order(string_t *s) {
  long i = 0;
  while (i < s->length) {
    if (s->value[i] == '.') return i - 1;
    i++;
  }
  return s->length - 1 - i;
}

long string_to_int(string_t *s) {
  int j = s->length - 1;
  long d = 0;
  for (long i = 0; j >= 0; i++) {
    d += digit_values[di(s->value[j])] * pow(BASE, i);
    j--;
  }
  return d;
}

long double string_to_double(string_t *s) {
  char32_t *p = s->value + s->length - 1;
  long double d = 0;
  long r = 0;
  long i;
  for (i = 0; p >= s->value; i++) {
    if (*p == '.') {
      r = i;
      i--;
      p--;
      continue;
    }
    d += digit_values[di(*p)] * pow(BASE, i);
    p--;
  }
  d = d / pow(BASE, r);
  return d;
}

string_t *int_to_string(long d) {
  // get from pool;
  string_t *s = init_string(U"");
  int offset = (1 - (d < 0) * 2) * 0.001;
  long abs_d = d * ((d >= 0) * 2 - 1);
  int power = log10(abs_d) / log10(BASE) + 1;
  int digit = round(d / pow(BASE, power) + offset);
  if (digit == 1) {
    string_append(s, digits[1]);
  } else if (digit == -1) {
    string_append(s, digits[RADIUS - B_ODD + 1]);
  }
  d -= digit * pow(BASE, power);
  long next;
  while(power > 0) {
    power--;
    digit = round(d / pow(BASE, power) + offset);
    d -= digit * pow(BASE, power);
    if (digit > 0) {
      string_append(s, digits[digit]);
    } else if (digit < 0) {
      string_append(s, digits[RADIUS - B_ODD - digit]);
    } else {
      if (d + offset > 0 || !B_ODD) {
        string_append(s, digits[0]);
      } else {
        string_append(s, digits[RADIUS]);
      }
    }
  }
  return s;
}

string_t *double_to_string(long double f) {
  // get from pool;
  string_t *s = init_string(U"");
  return s;
}

string_t *neg(string_t *s) {
  for (int idx = 0; idx < s->length; idx++) {
    if (s->value[idx] != '.') {
      s->value[idx] = negation_table[di(s->value[idx])];
    }
  }
  return s;
}

string_t *ip(string_t *s) {
  int len;
  for (len = 0; len < s->length; len++) {
    if (s->value[len] == '.') break;
  }
  s->length = len;
  return s;
}

string_t *fp(string_t *s) {
  int len;
  for (len = 0; len < s->length; len++) {
    if (s->value[len] == '.') break;
  }
  int i;
  for (i = 0; len < s->length; i++) {
    s->value[i] = s->value[len];
    len++;
  }
  s->length = i;
  return s;
}

bool sum_positive(string_t *m, string_t *n) {
  return true;
}

/* string_t *sum(string_t *m, string_t *n, char32_t *m_radix, char32_t *n_radix, */
/*               string_t *sum_buffer, char32_t *sb_radix) { */
/*   // TODO: get from pool */
/*   // and check whether string will be long enough and won't need to be realloced */
/*   string_t *s = init_string(U""); */
/*   char32_t *svalue = s->value; */

/*   bool m_high = false; */
/*   char32_t *pm = m->value; */
/*   char32_t *pn = n->value; */
/*   char32_t sum; */
/*   char32_t carry = digits[0]; */

/*   char32_t *m_end = pm + m->length; */
/*   char32_t *n_end = pn + n->length; */
/*   while(true) { */
/*     pm++; pn++; */
/*     if (pm == m_end || *pm == '.') { */
/*       while (pn != n_end && *pn != '.') { */
/*         pn++; */
/*       } */
/*       break; */
/*     } else if (pn == n_end || *pn == '.') { */
/*       while (pm != m_end && *pm != '.') { */
/*         pm++; */
/*       } */
/*       m_high = true; */
/*       break; */
/*     } */
/*   } */
/*   if (pm != m_end) { */
/*     if (pn != n_end) { */
/*       char32_t *pm2 = pm + 1; */
/*       char32_t *pn2 = pn + 1; */
/*       while(true) { */
/*         if (pm2 == m_end) { */
/*           if (pn2 != n_end) { */
/*             char32_t *p = n_end - 1; */
/*             while(p >= pn2) { */
/*               // replace with memcpy when bufsize is guaranteed to be enough */
/*               string_append(s, *p); */
/*               p--; */
/*             } */
/*           } */
/*           break; */
/*         } else if (pn2 == n_end) { */
/*           char32_t *p = m_end - 1; */
/*           while (p >= pm2) { */
/*             // replace with memcpy */
/*             string_append(s, *p); */
/*             p--; */
/*           } */
/*           break; */
/*         } */
/*         pm2++; */
/*         pn2++; */
/*       } */
/*       // add up until radix point */
/*       pm2--; pn2--; */
/*       //TODO: for even bases, check if rightmost digit will be a positive or negative form of 0 or B/2 */
/*       while(pm2 > pm) { */
/*         sum = addition_sum_table[di(*pm2)][di(*pn2)]; */
/*         sum = addition_sum_table[di(sum)][di(carry)]; */
/*         carry = addition_carry_table[di(*pm2)][di(*pn2)]; */
/*         // replace with memcpy */
/*         string_append(s, sum); */
/*         pm2--; */
/*         pn2--; */
/*       } */
/*       //replace with memcpy */
/*       string_append(s, *pm); */

/*     } else { */
/*       char32_t *p = m_end - 1; */
/*       while(p >= pm) { */
/*         // replace with memcpy */
/*         string_append(s, *p); */
/*         p--; */
/*       } */
/*     } */
/*   } else { */
/*     if (pn != n_end) { */
/*       char32_t *p = n_end - 1; */
/*       while(p >= pn) { */
/*         string_append(s, *p); */
/*         p--; */
/*       } */
/*     } */
/*   } */

/*   while(pm > m->value) { */
/*     pm--; pn--; */
/*     sum = addition_sum_table[di(*pm)][di(*pn)]; */
/*     sum = addition_sum_table[di(sum)][di(carry)]; */
/*     carry = addition_carry_table[di(*pm)][di(*pn)]; */
/*     //replace with memcpy */
/*     string_append(s, sum); */
/*   } */

/*   string_reverse(s); */
/*   return s; */
/* } */

string_t *sum(string_t *m, string_t *n, char32_t *m_radix, char32_t *n_radix,
              string_t *sum_buffer, char32_t *sb_radix) {
  char32_t *m_rad, *n_rad;
  char32_t *m_end = m->value + m->length;
  char32_t *n_end = n->value + n->length;
  if (m_radix) {
    m_rad = m_radix;
    if (n_radix) {
      n_rad = n_radix;
    } else {
      // find n_radix
      n_rad = n->value;
      while (n_rad < n_end) {
        if (*n_rad == '.') break;
        n_rad++;
      }
    }
  } else {
    if (n_radix) {
      n_rad = n_radix;
      // find m_radix
      m_rad = m->value;
      while (m_rad < m_end) {
        if (*m_rad == '.') break;
        m_rad++;
      }
    } else {
      // find both radix points
      m_rad = m->value;
      n_rad = n->value;
      while (true) {
        if (*m_rad == '.' || m_rad == m_end) {
          while (n_rad < n_end) {
            if (*n_rad == '.') break;
            n_rad++;
          }
          break;
        }
        if (*n_rad == '.' || n_rad == n_end) {
          while (m_rad < m_end) {
            if (*m_rad == '.') break;
            m_rad++;
          }
          break;
        }
        m_rad++;
        n_rad++;
      }
    }
  }

  //TODO: get from pool
  string_t *s = init_string(U"");

  char32_t *pm = m_end - 1;
  char32_t *pn = n_end - 1;
  char32_t sum, carry;

  if (sum_positive(m, n) || B_ODD)
    carry = digits[0];
  else
    carry = digits[RADIUS];

  if (m_rad != m_end) {
    if (n_rad != n_end) {
      unsigned m_mant = pm - m_rad;
      unsigned n_mant = pn - n_rad;
      while (m_mant > n_mant) {
        string_append(s, *pm);
        pm--;
        m_mant--;
      }
      while (n_mant > m_mant) {
        string_append(s, *pn);
        pn--;
        n_mant--;
      }
      // addition
      //
      // include radix point
      //TODO: for even bases, check if rightmost digit will be a positive or negative form of 0 or B/2
      while(pm > m_rad) {
        sum = addition_sum_table[di(*pm)][di(*pn)];
        sum = addition_sum_table[di(carry)][di(sum)];
        carry = addition_sum_table
          [di(addition_carry_table[di(carry)][di(sum)])]
          [di(addition_carry_table[di(*pm)][di(*pn)])];
        string_append(s, sum);
        pm--;
        pn--;
      }
      string_append(s, '.');
      if (pm == m->value) {
        pm = NULL;
      } else {
        pm--;
      }
      if (pn == n->value) {
        pn = NULL;
      } else {
        pn--;
      }

    } else {
      while (pm >= m_rad) {
        string_append(s, *pm);
        pm--;
      }
    }
  } else {
    //done
    if (n_rad != n_end) {
      while (pn >= n_rad) {
        string_append(s, *pn);
        pn--;
      }
    } else {
      if (m->length == 0) {
        for (int i = 0; i < n->length; i++) {
          string_append(s, n->value[i]);
        }
        return s;
      }
      if (n->length == 0) {
        for (int i = 0; i < m->length; i++) {
          string_append(s, m->value[i]);
        }
        return s;
      }
    }
  }
  if (pm) {
    if (pn) {
      // pm and pn are right before radix points
      while (pm >= m->value && pn >= n->value) {
        sum = addition_sum_table[di(*pm)][di(*pn)];
        sum = addition_sum_table[di(carry)][di(sum)];
        carry = addition_sum_table
          [di(addition_carry_table[di(carry)][di(sum)])]
          [di(addition_carry_table[di(*pm)][di(*pn)])];
        string_append(s, sum);
        pm--;
        pn--;
      }
    }
    while (pm >= m->value) {
      sum = addition_sum_table[di(carry)][di(*pm)];
      carry = addition_carry_table[di(carry)][di(*pm)];
      string_append(s, sum);
      pm--;
    }
  }
  if (pn) {
    while (pn >= n->value) {
      sum = addition_sum_table[di(carry)][di(*pn)];
      carry = addition_carry_table[di(carry)][di(*pn)];
      string_append(s, sum);
      pn--;
    }
  }
  if (carry == digits[1] || carry == digits[RADIUS - B_ODD + 1])
    string_append(s, carry);
  string_reverse(s);
  return s;
}

string_t *diff(string_t *m, string_t *n) {
  // temporary definition
  string_t *nneg = string_copy(n);
  neg(nneg);
  string_t *diff = sum(m, nneg, NULL, NULL, NULL, NULL);
  string_free(nneg);
  return diff;
}

string_t *product(string_t *m, string_t *n) {
  long double mf = string_to_double(m);
  long double nf = string_to_double(n);
  return double_to_string(mf * nf);
}

string_t *quotient(string_t *m, string_t *n) {
  long double mf = string_to_double(m);
  long double nf = string_to_double(n);
  return double_to_string(mf / nf);
}

string_t *str_sqrt(string_t *m) {
  long double mf = string_to_double(m);
  return double_to_string(sqrt(mf));
}

string_t *gaussian(string_t *m) {
  long double mf = string_to_double(m);
  return double_to_string(exp(-mf * mf));
}

string_t *str_exp(string_t *m) {
  long double mf = string_to_double(m);
  return double_to_string(exp(mf));
}

string_t *str_ln(string_t *m) {
  long double mf = string_to_double(m);
  return double_to_string(log(mf));
}

string_t *str_pow(string_t *m, string_t *n) {
  string_t *mln = str_ln(m);
  string_t *pr = product(mln, n);
  return str_exp(pr);
}

string_t *str_sin(string_t *m) {
  // replace with complex form
  long double mf = string_to_double(m);
  return double_to_string(sin(mf));
}

string_t *str_cos(string_t *m) {
  // replace with complex form
  long double mf = string_to_double(m);
  return double_to_string(cos(mf));
}

string_t *str_ceil(string_t *m) {
  long double mf = string_to_double(m);
  return double_to_string(ceil(mf));
}

string_t *str_floor(string_t *m) {
  long double mf = string_to_double(m);
  return double_to_string(floor(mf));
}

string_t *geq(string_t *m, string_t *n) {
  long double mf = string_to_double(m);
  long double nf = string_to_double(n);
  if (mf >= nf)
    return init_string(U"t");
  return init_string(U"");
}

string_t *leq(string_t *m, string_t *n) {
  long double mf = string_to_double(m);
  long double nf = string_to_double(n);
  if (mf <= nf)
    return init_string(U"t");
  return init_string(U"");
}

string_t *gthan(string_t *m, string_t *n) {
  long double mf = string_to_double(m);
  long double nf = string_to_double(n);
  if (mf > nf)
    return init_string(U"t");
  return init_string(U"");
}

string_t *lthan(string_t *m, string_t *n) {
  long double mf = string_to_double(m);
  long double nf = string_to_double(n);
  if (mf < nf)
    return init_string(U"t");
  return init_string(U"");
}
