#ifndef BETTER_STRING_H
#define BETTER_STRING_H
#include <stdlib.h>
#include <stdbool.h>
/*! We want better strings to realloc less and keep track of the length of the
 * string so we don't have to call strlen every time */

typedef unsigned char byte_t;
/*! @brief An array implementation of the string data structure */
typedef struct STRING_STRUCT {
  /*! @brief length of string */
  size_t length;
  /*! @brief Size of current value buffer */
  size_t bufsize;
  /*! @brief String value of struct */
  byte_t *value;
} string_t;

size_t string_len(byte_t *str);

byte_t *utf8cpy(byte_t *dst, byte_t *src, size_t sizeDest);

bool string_comp(string_t *s1, string_t *s2);

/*! Allocates memory for new string */
string_t *init_string(void *value);

/*! Copies string to another string */
string_t *string_copy(string_t *s);

/*! Concatenates a string_t type and a char * type */
void string_concat(string_t *s1, string_t *s2);

/*! Appends single characters */
void string_append(string_t *s, byte_t c);

/*! Frees space for string */
void string_free(string_t *s);
#endif
