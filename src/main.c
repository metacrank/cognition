#include "better_string.h"
#include <builtins.h>
#include <builtinslib.h>
#include <cognition.h>
#include <hash_table.h>
#include <strnum.h>
#include <dlfcn.h>
#include <locale.h>
#include <macros.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern stack_t *STACK;
extern stack_t *EVAL_CONTAINERS;
extern stack_t *EVAL_CONTAIN_TRASH;
extern string_t *ROOT;
extern parser_t *PARSER;
extern stack_t *EVAL_STACK;
extern stack_t *CONTAIN_DEF_STACK;
extern stack_t *MACRO_DEF_STACK;
extern stack_t *FAMILY;
extern stack_t *FAMILY_IDX;
extern stack_t *CONTAINERS;
extern stack_t *MACROS;
extern stack_t *OBJ_TABLE_STACK;
extern stack_t *OBJ_TABLE_REF_STACK;
extern string_t *EXIT_CODE;
extern bool EXITED;
extern string_t **CAST_ARGS;
extern bool RETURNED;

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
  if (cur->faliases) {
    printf("\n");
    printf("Faliases: ");
    for (int i = 0; i < cur->faliases->size; i++) {
      printf("'");
      print(cur->faliases->items[i]);
      printf("' ");
    }
  }
  printf("\n");
  printf("delims: '");
  print_str_formatted(cur->delims);
  if (cur->dflag)
    printf("' (whitelist)\n");
  else
    printf("' (blacklist)\n");
  printf("ignored: '");
  print_str_formatted(cur->ignored);
  if (cur->iflag)
    printf("' (whitelist)\n");
  else
    printf("' (blacklist)\n");
  printf("singlets: '");
  print_str_formatted(cur->singlets);
  if (cur->sflag)
    printf("' (whitelist)\n");
  else
    printf("' (blacklist)\n");

  if (cur->cranks) {
    if (cur->cranks->size) {
      printf("cranks:");
      for (int i = 0; i < cur->cranks->size; i++) {
        int(*cr)[2] = cur->cranks->items[i];
        printf(" %d:(%d,%d)", i, cr[0][0], cr[0][1]);
      }
      printf("\n");
    } else
      printf("crank 0\n");
  } else
    printf("null crank\n");
  if (EXIT_CODE) {
    printf("\nExit code: '");
    print(EXIT_CODE);
    printf("'\n");
  }
  else
    printf("\nExit code: (none)\n");
}

/*! frees all global variables */
void global_free() {
  printf("%lu\n", CONTAIN_DEF_STACK->size);
  stack_free(CONTAIN_DEF_STACK, contain_free);
  return;
  string_free(PARSER->source);
  stack_free(OBJ_TABLE_STACK, ht_free_free);
  stack_free(OBJ_TABLE_REF_STACK, nop);
  string_free(ROOT);
  if (!EXITED) {
    contain_free(STACK->items[0]);
  }
  free(STACK->items);
  free(STACK);
  free(PARSER);
  stack_free(EVAL_STACK, value_free);
  stack_free(EVAL_CONTAINERS, nop);
  stack_free(CONTAIN_DEF_STACK, contain_free);
  return;
  stack_free(EVAL_CONTAIN_TRASH, nop);
  stack_free(MACRO_DEF_STACK, value_stack_free);
  stack_free(FAMILY, nop);
  stack_free(FAMILY_IDX, nop);
  stack_free(CONTAINERS, nop);
  stack_free(MACROS, nop);
  string_free(EXIT_CODE);
  for (int i = 0; i < 4; i++) {
    string_free(CAST_ARGS[i]);
  }
  free(CAST_ARGS);
  math_free();
}

int main(int argc, char **argv) {
  value_t *v;
  size_t len = 0;
  char *locale = setlocale(LC_ALL, "");
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
    } else if (strcmp(argv[i], "-v") == 0 ||
               strcmp(argv[i], "--version") == 0) {
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

  string_t *buffer = file_read(FP);
  fclose(FP);

  /* Set up global variables */
  PARSER = init_parser(buffer);
  STACK = init_stack(DEFAULT_STACK_SIZE);
  EVAL_STACK = init_stack(DEFAULT_STACK_SIZE);
  CONTAIN_DEF_STACK = init_stack(DEFAULT_STACK_SIZE);
  MACRO_DEF_STACK = init_stack(DEFAULT_STACK_SIZE);
  EVAL_CONTAIN_TRASH = init_stack(DEFAULT_STACK_SIZE);
  FAMILY = init_stack(DEFAULT_STACK_SIZE);
  stack_push(FAMILY, NULL);
  FAMILY_IDX = init_stack(DEFAULT_STACK_SIZE);
  CONTAINERS = init_stack(DEFAULT_STACK_SIZE);
  MACROS = init_stack(DEFAULT_STACK_SIZE);
  OBJ_TABLE_REF_STACK = init_stack(DEFAULT_STACK_SIZE);
  OBJ_TABLE_STACK = init_stack(DEFAULT_STACK_SIZE);
  stack_push(OBJ_TABLE_STACK, init_ht(DEFAULT_HT_SIZE));
  ROOT = init_string(U"");
  EXIT_CODE = NULL;
  EVAL_CONTAINERS = init_stack(DEFAULT_STACK_SIZE);
  RETURNED = false;
  CAST_ARGS = malloc(4 * sizeof(string_t*));
  CAST_ARGS[0] = init_string(U"VSTACK");
  CAST_ARGS[1] = init_string(U"0");
  CAST_ARGS[2] = init_string(U"VMACRO");
  CAST_ARGS[3] = init_string(U"1");

  /* initialise environment */
  contain_t *stack =
      init_contain(init_ht(DEFAULT_HT_SIZE), init_ht(DEFAULT_HT_SIZE), NULL);
  stack->faliases = init_stack(DEFAULT_STACK_SIZE);
  stack_push(stack->faliases, init_string(U"f"));
  stack_push(stack->faliases, init_string(U"ing"));
  add_funcs(stack->flit);
  stack_push(STACK, stack);
  stack_push(OBJ_TABLE_REF_STACK, stack);
  string_append(ROOT, 0);

  init_math();

  /* parse and eval loop */
  while (1) {
    v = parser_get_next(PARSER);
    if (v == NULL)
      break;
    eval(v);
    if (EXITED)
      break;
  }
  if (!args.q) {
    print_end();
  }

  /* Free all global variables */
  global_free();
  return 0;
}
