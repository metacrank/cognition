#ifndef PARSER_H_
#define PARSER_H_
#include "better_string.h"
#include <stdbool.h>
#include <stdlib.h>

typedef struct VALUE_STRUCT value_t;
typedef struct ARRAY_STRUCT array_t;

struct ARRAY_STRUCT {
  value_t **items;
  size_t size;
  size_t capacity;
};

struct VALUE_STRUCT {
  enum { VWORD, VINT, VFLOAT, VSTR, VQUOTE, VERR, VCUSTOM } type;
  union {
    long double int_float;
    array_t *quote;
    string_t *str_word;
  };
  void *custom;
  bool escaped;
};

typedef struct PARSER_STRUCT {
  char *source;
  int i;
  char c;
} parser_t;

typedef struct NODE_STRUCT {
  string_t *key;
  void *value;
  struct NODE_STRUCT *next;
} node_t;

typedef struct {
  node_t *head;
  size_t size;
} sll_t;

typedef struct {
  sll_t **buckets;
  size_t size;
} ht_t;

typedef struct {
  void (*printfunc)(void *);
  void *(*copyfunc)(void *);
  void (*freefunc)(void *);
} custom_t;

void func_free(void *f);

array_t *init_array(size_t size);

void array_append(array_t *a, value_t *v);

value_t *array_pop(array_t *a);

array_t *array_copy(array_t *a);

void array_extend(array_t *a1, array_t *a2);

void array_free(array_t *a);

value_t *init_value(int type);

value_t *value_copy(value_t *v);

void value_free(void *v);

custom_t *init_custom(void (*)(void *), void (*)(void *), void *(*)(void *));

void custom_free(void *);

parser_t *init_parser(char *source);

void parser_reset(parser_t *p, char *source);

void parser_move(parser_t *p);

void parser_skip_whitespace(parser_t *p);

value_t *parser_get_next(parser_t *p);

node_t *init_node(string_t *key, void *v);

sll_t *init_sll();

void sll_add(sll_t *l, string_t *key, void *v);

void *sll_get(sll_t *l, string_t *key);

void sll_free(sll_t *l, void (*freefunc)(void *));

ht_t *init_ht(size_t size);

void ht_add(ht_t *h, string_t *key, void *v);

void *ht_get(ht_t *h, string_t *key);

bool ht_exists(ht_t *h, string_t *key);

void ht_free(ht_t *h, void (*freefunc)(void *));

unsigned long hash(ht_t *h, char *key);

bool eval_builtins(value_t *v);

bool eval_ht(value_t *v);

bool eval_quote(value_t *v);

void eval(value_t *v);

#endif // PARSER_H_
