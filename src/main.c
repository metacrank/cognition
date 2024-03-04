#include <builtins.h>
#include <combinators.h>
#include <cranker.h>
#include <hashtable.h>
#include <io.h>
#include <math.h>
#include <metastack.h>
#include <misc.h>
#include <parser.h>
#include <stackops.h>
#include <cognition.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern stack_t *STACK;
extern parser_t *PARSER;
extern stack_t *EVAL_STACK;
//extern ht_t *OBJ_TABLE;
extern stack_t *OBJ_STACK;
extern ht_t *DEFAULT_FLIT;

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
  /* free(PARSER->source); */
  /* stack_free(OBJ_STACK); */
  /* stack_free(STACK); */
  /* free(PARSER); */
  /* stack_free(EVAL_STACK); */
  /* ht_free(DEFAULT_FLIT); */
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
  PARSER = init_parser(buf);
  STACK = init_stack(10);
  EVAL_STACK = init_stack(10);
  OBJ_STACK = init_stack(10);

  /* initialise environment */
  contain_t *stack = init_contain(init_ht(500), init_ht(500), init_stack(10));
  stack_push(stack->faliases, init_string("f"));
  add_funcs(stack->flit);
  stack_push(STACK, stack);
  void *(ot)[] = {stack, init_ht(10)};
  stack_push(OBJ_STACK, ot);

  printf("[%s] [%d] [%c]\n", PARSER->source, PARSER->i, PARSER->c);
  value_t *dummy = init_value(VWORD);

  /* parse and eval loop */
  while (1) {
    v = parser_get_next(PARSER);
    if (v == NULL)
      break;
    //eval(v);
    push_quoted(stack_peek(STACK), v);
    cog_questionmark(dummy);
    printf("\n\n");
  }

  /* Free all global variables */
  global_free();
  return 0;
}
