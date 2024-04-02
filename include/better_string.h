#ifndef BETTER_STRING_H
#define BETTER_STRING_H
#include <stdlib.h>
#include <stdbool.h>
#include <uchar.h>
#include <stdio.h>
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
  char32_t *value;
} string_t;


/* returns the number of bytes encoding the unicode character at b */
int sizeof_utf8(byte_t *b);

/* increments a byte pointer to the next unicode character */
void inc_utf8(byte_t **b);

/* decrements a byte pointer to the previous unicode character */
void dec_utf8(byte_t **b);

int utf8cmp(byte_t *b1, byte_t *b2);

size_t utf32_to_utf8(byte_t *const buf, const char32_t utf32);

char32_t utf8_to_utf32(byte_t *const buf);

/* analagous to strcmp for better_string */
int string_comp(string_t *s1, string_t *s2);

/* returns an identical string with the bufsize 'size' */
void realloc_string(string_t *s, size_t size);

/* ensures there is space to put n characters in the string by reallocing if necessary */
void string_ensure_space(string_t *s, size_t n);

/*! Allocates memory for new string */
string_t *init_string(char32_t *value);

/*! Copies string to another string */
string_t *string_copy(string_t *s);

/*! Concatenates a string_t type and a char * type */
void string_concat(string_t *s1, string_t *s2);

/*! Appends single characters */
void string_append(string_t *s, char32_t c);

/* reverse order of characters of a string */
void string_reverse(string_t *s);

/* prints a string dumbly formatted */
void print_formatted(const char32_t *format, ...);

/* prints a single utf32 character */
void print_utf32(int, ...);
void fprint_utf32(FILE *, int, ...);

/* prints a string */
void print(string_t *s);
void fprint(FILE *f, string_t *s);

/* reads a utf8 file into string_t */
string_t *file_read(FILE *FP);

/* prints to utf8 file */
void file_print(FILE *FP, string_t *s);

/*! Frees space for string */
void string_free(string_t *s);

/* returns allocated space to pool */
void string_pool_add(string_t *s);

#endif
