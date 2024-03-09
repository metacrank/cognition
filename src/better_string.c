#include <better_string.h>
#include <macros.h>
#include <stdlib.h>
#include <string.h>

string_t *init_string(char *s) {
  string_t *str = malloc(sizeof(string_t));
  if (str == NULL)
    die("malloc in init_string");
  /* If we pass in NULL we assume that we want to append things to this in the
   * future */
  if (s == NULL || strcmp(s, "") == 0) {
    str->bufsize = 10;
    str->value = calloc(str->bufsize, sizeof(char));
    str->value[0] = '\0';
    if (str->value == NULL)
      die("calloc in init_string");
    str->length = 0;
    return str;
  }
  /* Otherwise we want the initial value to be equal to the parameter */
  str->length = strlen(s);
  str->bufsize = 2 * strlen(s);
  str->value = calloc(str->bufsize, sizeof(char));
  if (str->value == NULL)
    die("calloc in init_string");
  strcpy(str->value, s);
  return str;
}

string_t *string_copy(string_t *s) {
  if (s == NULL) return NULL;
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

void string_append(string_t *s, char c) {
  char str[2] = {c, '\0'};
  int oldsize = s->bufsize;
  if (s->bufsize - 1 <= s->length) {
    s->bufsize = s->bufsize * 2;
    s->value = realloc_zero(s->value, oldsize, s->bufsize);
  }
  s->length++;
  strcat(s->value, str);
}

void string_free(string_t *s) {
  free(s->value);
  free(s);
}
