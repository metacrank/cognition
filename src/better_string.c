#include <better_string.h>
#include <macros.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

size_t string_len(byte_t *str) {
  if (!str) return -1;
  size_t s = 0;
  byte_t b = str[0];
  while (b) {
    s ++;
    b = str[s];
  }
  return s;
}

byte_t *utf8cpy(byte_t *dst, byte_t *src, size_t sizeDest) {
  if (sizeDest) {
    size_t sizeSrc = string_len(src);
    while (sizeSrc >= sizeDest) {

      byte_t *lastByte = src + sizeSrc;
      while (lastByte-- > src)
        if ((*lastByte & 0xC0) != 0x80)
          break;

      sizeSrc = lastByte - src;
    }
    memcpy(dst, src, sizeSrc);
    dst[sizeSrc] = '\0';
  }
  return dst;
}

bool string_comp(byte_t *u1, byte_t *u2) {
  return strcmp(u1, u2);
  if (string_len(u1) != string_len(u2)) return true;
  for (int i = 0; i < string_len(u1); i ++) {
    if (u1[i] != u2[i]) return true;
  }
  return false;
}

string_t *init_string(byte_t *s) {
  string_t *str = malloc(sizeof(string_t));
  if (str == NULL)
    die("malloc in init_string");
  /* If we pass in NULL we assume that we want to append things to this in the
   * future */
  if (s == NULL || string_comp(s, "") == 0) {
    str->bufsize = 10 * sizeof(byte_t);
    str->value = calloc(str->bufsize, sizeof(byte_t));
    str->value[0] = '\0';
    if (str->value == NULL)
      die("calloc in init_string");
    str->length = 0;
    return str;
  }
  /* Otherwise we want the initial value to be equal to the parameter */
  size_t leng = string_len(s);
  str->length = leng;
  str->bufsize = 2 * leng;
  str->value = calloc(str->bufsize, sizeof(byte_t));
  if (str->value == NULL)
    die("calloc in init_string");
  for (int i = 0; i < leng + 1; i ++) {
    string_append(str, s[i]);
  }
  return str;
}

string_t *string_copy(string_t *s) {
  if (s == NULL)
    return NULL;
  return init_string(s->value);
}

/* stackoverflow code */
void *realloc_zero(void *pBuffer, size_t oldSize, size_t newSize) {
  void *pNew = realloc(pBuffer, newSize);
  if (pNew == NULL)
    die("realloc on realloc_zero");
  if (newSize > oldSize && pNew) {
    size_t diff = newSize - oldSize;
    void *pStart = ((char *)pNew) + oldSize;
    memset(pStart, 0, diff);
  }
  return pNew;
}

void string_concat(string_t *s1, string_t *s2) {
  for (int i = 0; i < s2->length; i++) {
    string_append(s1, s2->value[i]);
  }
}

void string_append(string_t *s, byte_t c) {
  byte_t str[2] = {c, '\0'};
  int oldsize = s->bufsize;
  if (s->bufsize - 1 <= s->length) {
    s->bufsize = s->bufsize * 2;
    s->value = realloc_zero(s->value, oldsize, s->bufsize * sizeof(byte_t));
  }
  byte_t *cur =s->value + s->length;
  memcpy(cur, str, 2 * sizeof (byte_t));
  s->length++;
  printf("%s\n", cur);
}

void string_free(string_t *s) {
  if (s == NULL)
    return;
  free(s->value);
  free(s);
}
