#include <builtins.h>
#include <cognition.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern stack_t *STACK;
extern parser_t *PARSER;
extern stack_t *EVAL_STACK;
extern ht_t *OBJ_TABLE;

/*! prints usage then exits */
void usage() {
  printf("Usage: stem [-hv] [file]\n");
  exit(1);
}

/*! prints version and exits */
void version() {
  printf("Author: Preston Pan, MIT License 2024\n");
  printf("stem, version 1.4 alpha\n");
  exit(0);
}

/*! frees all global variables */
void global_free() {
  free(PARSER->source);
  ht_free(OBJ_TABLE, custom_free);
  stack_free(STACK);
  free(PARSER);
  stack_free(EVAL_STACK);
}

int main(int argc, char **argv) {
  value_t *v;
  size_t len = 0;
  char *buf = "";

  /* Parsing arguments */
  if (argc < 2) {
    usage();
  }

  if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
    usage();
  } else if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
    version();
  }

  /* Read code from file */
  FILE *FP = fopen(argv[1], "rb");

  if (!FP) {
    usage();
  }

  ssize_t bytes_read = getdelim(&buf, &len, '\0', FP);
  fclose(FP);

  /* Set up global variables */
  PARSER = parser_pp(buf);
  STACK = init_stack(10);
  EVAL_STACK = init_stack(10);
  OBJ_TABLE = init_ht(500);

  add_funcs();

  /* parse and eval loop */
  while (1) {
    v = parser_get_next(PARSER);
    if (v == NULL)
      break;
    eval(v);
  }

  /* Free all global variables */
  global_free();
  return 0;
}
