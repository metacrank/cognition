#include <better_string.h>
#include <macros.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>

byte_t print_buffer[5];

int sizeof_utf8(byte_t *b) {
  if (*b < 0xC0) {
    return 1;
  }
  if (*b < 0xE0) {
    return 2;
  }
  if (*b < 0xF0) {
    return 3;
  }
  return 4;
}

void inc_utf8(byte_t **b) {
  *b += sizeof_utf8(*b);
}

void dec_utf8(byte_t **b) {
  do {
    (*b)--;
  } while (0x80 <= **b && **b < 0xC0);
}

int utf8cmp(byte_t *b1, byte_t *b2) {
  int size1 = sizeof_utf8(b1);
  int size2 = sizeof_utf8(b2);
  int retval;
  int i;
  for (i = 0; i < size1; i++) {
    if (i == size2) return b1[i];
    if ((retval = b1[i] - b2[i])) return retval;
  }
  if (i != size2)
    return - b2[i];
  return 0;
}

// assumes little endian
size_t utf32_to_utf8(byte_t *const buf, const char32_t utf32) {
  if (utf32 < 0x80) {
    buf[0] = utf32;
    return 1;
  }
  if (utf32 < 0x800) {
    buf[0] = 0xC0 | (utf32 >> 6);            /* 110xxxxx */
    buf[1] = 0x80 | (utf32 & 0x3F);          /* 10xxxxxx */
    return 2;
  }
  if (utf32 < 0x10000) {
    buf[0] = 0xE0 | (utf32 >> 12);           /* 1110xxxx */
    buf[1] = 0x80 | ((utf32 >> 6) & 0x3F);   /* 10xxxxxx */
    buf[2] = 0x80 | (utf32 & 0x3F);          /* 10xxxxxx */
    return 3;
  }
  if (utf32 < 0x110000) {
    buf[0] = 0xF0 | (utf32 >> 18);           /* 11110xxx */
    buf[1] = 0x80 | ((utf32 >> 12) & 0x3F);  /* 10xxxxxx */
    buf[2] = 0x80 | ((utf32 >> 6) & 0x3F);   /* 10xxxxxx */
    buf[3] = 0x80 | (utf32 & 0x3F);          /* 10xxxxxx */
    return 4;
  }
  return 0;
}

char32_t utf8_to_utf32(byte_t *const buf) {
  if (buf[0] < 0xC0) {
    return buf[0];
  }
  if (buf[0] < 0xE0) {
    return (buf[0] - 0xC0) * 0x40 + buf[1] - 0x80;
  }
  if (buf[0] < 0xF0) {
    return (buf[0] - 0xE0) * 0x1000 + (buf[1] - 0x80) * 0x40 + buf[2] - 0x80;
  }
  return (buf[0] - 0xF0) * 0x40000 + (buf[1] - 0x80) * 1000 + (buf[2] - 0x80) * 0x40 + buf[3] - 0x80;
}

int string_comp(string_t *s1, string_t *s2) {
  int retval;
  int i;
  for (i = 0; i < s1->length; i++) {
    if (i == s2->length) return s1->value[i];
    if ((retval = s1->value[i] - s2->value[i])) return retval;
  }
  if (i != s2->length)
    return - s2->value[i];
  return 0;
}

void realloc_string(string_t *s, size_t size) {
  //fetch from pool
  string_t *newstr = malloc(sizeof(string_t));
  newstr->bufsize = size;
  newstr->value = malloc(size * sizeof(char32_t));

  char32_t *val = newstr->value;
  newstr->value = s->value;
  s->value = val;

  memcpy(s->value, newstr->value, s->length * sizeof(char32_t));
  //add to pool
  string_free(newstr);
}

void string_ensure_space(string_t *s, size_t n) {
  if (s->bufsize < s->length + n) {
  int k = 2;
  while (k * s->bufsize < s->length + n)
    k *= 2;
  realloc_string(s, k * s->bufsize);
  }
}

string_t *init_string(char32_t *a) {
  if (!a) return NULL;
  string_t *s = malloc(sizeof(string_t));
  s->length = 0;
  s->bufsize = 24;
  // replace with pool access later
  s->value = malloc(s->bufsize * sizeof(char32_t));
  while (*a != '\0') {
    string_ensure_space(s, 1);
    s->value[s->length] = *a;
    s->length++;
    a++;
  }
  return s;
}

string_t *string_copy(string_t *s) {
  if (s == NULL)
    return NULL;
  if (s->length > s->bufsize) {
    printf("catastrophic string management failure");
    return NULL;
  }
  // fetch from pool
  string_t *cp = malloc(sizeof(string_t));
  cp->bufsize = s->bufsize;
  cp->length = s->length;
  cp->value = malloc(cp->bufsize * sizeof(char32_t));
  for (long i = 0; i < s->length; i++) {
    cp->value[i] = s->value[i];
  }
  return cp;
}

void string_concat(string_t *s1, string_t *s2) {
  string_ensure_space(s1, s2->length);
  for (long i = 0; i < s2->length; i++) {
    s1->value[s1->length] = s2->value[i];
    s1->length++;
  }
}

void string_append(string_t *s, char32_t c) {
  string_ensure_space(s, 1);
  s->value[s->length] = c;
  s->length++;
}

void string_reverse(string_t *s) {
  char32_t temp;
  size_t len = s->length;
  for (long i = 0; i < len / 2; i++) {
    temp = s->value[i];
    s->value[i] = s->value[len - 1 - i];
    s->value[len - 1 - i] = temp;
  }
}

void print_formatted(const char32_t *str, ...) {
  va_list args;
  va_start(args, str);
  //vwprintf((const wchar_t *)str, args);
  va_end(args);
}

void print(string_t *s) {
  for (long i = 0; i < s->length; i++) {
    utf32_to_utf8(print_buffer, s->value[i]);
    printf("%s", print_buffer);
  }
}

void print_utf32(int num, ...) {
  va_list args;
  va_start(args, num);
  for (int i = 0; i < num; i++) {
    char32_t utf32 = va_arg(args, char32_t);
    utf32_to_utf8(print_buffer, utf32);
    printf("%s", print_buffer);
  }
}

string_t *file_read(FILE *FP) {
  if (!FP) return NULL;
  string_t *s = init_string(U"");
  byte_t b;
  int c;
  char32_t utf32;
  while ((c = fgetc(FP)) != EOF) {
    b = c;
    switch (sizeof_utf8(&b)) {
      case 1:
        utf32 = b;
        break;
      case 2:
        utf32 = b - 0xC0;
        utf32 *= 0x40;
        if ((c = fgetc(FP)) == EOF) return s;
        utf32 += c - 0x80;
        break;
      case 3:
        utf32 = b - 0xE0;
        for (int _ = 0; _ < 2; _++) {
          utf32 *= 0x40;
          if ((c = fgetc(FP)) == EOF) return s;
          utf32 += c - 0x80;
        }
      case 4:
        utf32 = b - 0xF0;
        for (int _ = 0; _ < 3; _++) {
          utf32 *= 0x40;
          if ((c = fgetc(FP)) == EOF) return s;
          utf32 += c - 0x80;
        }
        break;
    }
    string_append(s, utf32);
  }
  return s;
}

void file_print(FILE *FP, string_t *s) {
  if (!s || !FP) return;
  for (long i = 0; i < s->length; i++) {
    utf32_to_utf8(print_buffer, s->value[i]);
    fprintf(FP, "%s", print_buffer);
  }
}

void string_free(string_t *s) {
  if (s == NULL) return;
  free(s->value);
  free(s);
}
