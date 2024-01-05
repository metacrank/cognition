#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50
#define JUSTDO(a)                                                              \
  if (!(a)) {                                                                  \
    perror(#a);                                                                \
    exit(1);                                                                   \
  }

extern ht_t *WORD_TABLE;
extern array_t *STACK;
extern char *INBUF;
extern parser_t *PARSER;

/** char *get_line  FILE *f
 * reads an arbitrarily long line of text or until EOF
 * caller must free the pointer returned after use
 */
char *get_line(FILE *f) {
  int len = MAX;
  char buf[MAX], *e = NULL, *ret;
  JUSTDO(ret = calloc(MAX, 1));
  while (fgets(buf, MAX, f)) {
    if (len - strlen(ret) < MAX)
      JUSTDO(ret = realloc(ret, len *= 2));
    strcat(ret, buf);
    if ((e = strrchr(ret, '\n')))
      break;
  }
  if (e)
    *e = '\0';
  return ret;
}

int main(int argc, char **argv) {
  PARSER = init_parser("");
  value_t *v;
  STACK = init_array(10);
  WORD_TABLE = init_ht(500);

  printf("exit to exit REPL;\n");
  while (1) {
    printf("> ");
    INBUF = get_line(stdin);
    parser_reset(PARSER, INBUF);
    while (1) {
      v = parser_get_next(PARSER);
      if (v == NULL)
        break;
      eval(v);
    }
    free(INBUF);
  }
  ht_free(WORD_TABLE);
  array_free(STACK);
  free(PARSER);
  free(INBUF);
  return 0;
}
