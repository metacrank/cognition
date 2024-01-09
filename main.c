#include "builtins.h"
#include "parser.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern ht_t *WORD_TABLE;

extern array_t *STACK;
extern char *INBUF;
extern parser_t *PARSER;
extern array_t *EVAL_STACK;
extern ht_t *OBJ_TABLE;
extern ht_t *FLIT;

void usage() {
  printf("Usage: stem [-hv] [file]\n");
  exit(1);
}

void version() {
  printf("Author: Preston Pan, MIT License 2023\n");
  printf("stem, version 1.2 alpha\n");
  exit(0);
}

int main(int argc, char **argv) {
  value_t *v;
  size_t len;

  if (argc < 2) {
    usage();
  }

  if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
    usage();
  } else if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
    version();
  }

  FILE *FP = fopen(argv[1], "rb");

  if (!FP) {
    usage();
  }

  ssize_t bytes_read = getdelim(&INBUF, &len, '\0', FP);
  fclose(FP);

  PARSER = init_parser(INBUF);
  STACK = init_array(10);
  WORD_TABLE = init_ht(500);
  EVAL_STACK = init_array(10);
  FLIT = init_ht(500);
  OBJ_TABLE = init_ht(500);

  add_funcs();

  while (1) {
    v = parser_get_next(PARSER);
    if (v == NULL)
      break;
    eval(v);
  }

  free(INBUF);
  ht_free(WORD_TABLE, value_free);
  ht_free(FLIT, func_free);
  ht_free(OBJ_TABLE, custom_free);
  array_free(STACK);
  free(PARSER);
  array_free(EVAL_STACK);
  return 0;
}
