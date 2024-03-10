#include <builtins.h>
#include <builtinslib.h>
#include <cognition.h>
#include <macros.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern stack_t *STACK;
extern parser_t *PARSER;
extern stack_t *EVAL_STACK;
extern stack_t *OBJ_STACK;

/*! prints usage then exits */
void usage(int e) {
  printf("Usage: crank [-hqv] [file]\n");
  exit(e);
}

/*! prints version and exits */
void version() {
  printf("Authors: Preston Pan, Matthew Hinton, MIT License 2024\n");
  printf("cognition, version 1.0 alpha\n");
  exit(0);
}

/*! prints state of program when execution stops */
void print_end() {
  printf("\n");
  printf("Stack at end:\n");
  contain_t *cur = stack_peek(STACK);
  for (int i = 0; i < cur->stack->size; i++) {
    print_value(cur->stack->items[i], "\n");
  }
  printf("\n");
  printf("Error stack:\n");
  stack_t *error_stack = cur->err_stack;
  if (error_stack) {
    for (int i = 0; i < error_stack->size; i++) {
      print_value(error_stack->items[i], "\n");
    }
  }
  printf("\n");
  printf("delims: '");
  print_str_formatted(cur->delims);
  if (cur->dflag) printf("' (whitelist)\n");
  else printf("' (blacklist)\n");
  printf("ignored: '");
  print_str_formatted(cur->ignored);
  if (cur->iflag) printf("' (whitelist)\n");
  else printf("' (blacklist)\n");
  printf("singlets: '");
  print_str_formatted(cur->singlets);
  if (cur->sflag) printf("' (whitelist)\n");
  else printf("' (blacklist)\n");


  if (cur->cranks) {
    if (cur->cranks->size) {
      int(*cr)[2] = cur->cranks->items[0];
      printf("crank %d\n", cr[0][1]);
    } else printf("crank 0\n");
  } else printf("crank 0\n");
}

/*! frees all global variables */
void global_free() {
  free(PARSER->source);
  stack_free(OBJ_STACK, obj_free);
  stack_free(STACK, contain_free);
  free(PARSER);
  stack_free(EVAL_STACK, value_free);
}

int main(int argc, char **argv) {
  value_t *v;
  size_t len = 0;
  char *buf = "";

  /* Parsing arguments */
  if (argc < 2) {
    usage(1);
  }

  struct {
    bool h;
    bool v;
    bool q;
  } args = {};

  int fileidx = 0;
  bool filefound = false;

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
      if (args.h) {
        usage(1);
        break;
      }
      args.h = true;
    } else if (strcmp(argv[i], "-q") == 0 || strcmp(argv[i], "--quiet") == 0) {
      if (args.q) {
        usage(1);
        break;
      }
      args.q = true;
    } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
      if (args.v) {
        usage(1);
        break;
      }
      args.v = true;
    } else if (filefound) {
      usage(1);
    } else {
      fileidx = i;
      filefound = true;
    }
  }

  if (args.h) {
    usage(0);
  }
  if (args.v) {
    version();
  }
  if (!filefound) {
    usage(1);
  }

  /* Read code from file */
  FILE *FP = fopen(argv[fileidx], "rb");

  if (!FP) {
    usage(1);
  }

  ssize_t bytes_read = getdelim(&buf, &len, '\0', FP);
  fclose(FP);

  /* Set up global variables */
  PARSER = init_parser(buf);
  STACK = init_stack(DEFAULT_STACK_SIZE);
  EVAL_STACK = init_stack(DEFAULT_STACK_SIZE);
  OBJ_STACK = init_stack(DEFAULT_STACK_SIZE);

  /* initialise environment */
  contain_t *stack = init_contain(init_ht(DEFAULT_HT_SIZE),
                                  init_ht(DEFAULT_HT_SIZE),
                                  init_stack(DEFAULT_STACK_SIZE));
  stack->faliases = init_stack(DEFAULT_STACK_SIZE);
  stack_push(stack->faliases, init_string("f"));
  add_funcs(stack->flit);
  stack_push(STACK, stack);
  void *(ot)[2] = {stack, init_ht(DEFAULT_HT_SIZE)};
  stack_push(OBJ_STACK, ot);

  /* parse and eval loop */
  while (1) {
    v = parser_get_next(PARSER);
    if (v == NULL)
      break;
    eval(v);
  }
  if (!args.q) {
    print_end();
  }

  /* Free all global variables */
  global_free();
  return 0;
}
