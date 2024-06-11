#include "better_string.h"
#include <builtins.h>
#include <builtinslib.h>
#include <cognition.h>
#include <hash_table.h>
#include <strnum.h>
#include <pool.h>
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
extern stack_t *CONTAIN_DEF_STACK;
extern stack_t *MACRO_DEF_STACK;
extern stack_t *FAMILY;
extern string_t *FAMILY_IDX;
extern int FAMILY_RECURSION_DEPTH;
extern stack_t *CONTAINERS;
extern stack_t *MACROS;
extern stack_t *OBJ_TABLE_STACK;
extern stack_t *OBJ_TABLE_REF_STACK;
extern string_t *EXIT_CODE;
extern bool EXITED;
extern string_t **CAST_ARGS;
extern bool RETURNED;
extern stack_t *ARGS;
extern pool_t *OBJ_POOL;
extern string_t *F;
extern string_t *ING;

void pool_checker() {
  OBJ_POOL = init_pool();

  value_t *v = init_value(VERR);
  v->error = malloc(sizeof(error_t));
  v->error->error = init_string(U"ERROR");
  v->error->str_word = init_string(U"STR_WORD");

  printf("%p\n", v);

  show_pool();
  printf("\n-------------\n\n");
  pool_add(OBJ_POOL, POOL_VERR, v);

  show_pool();
  printf("\n-------------\n\n");

  pool_gc(OBJ_POOL);

  show_pool();
  printf("\n-------------\n\n");

  value_t *vget = pool_req(0, POOL_VERR);

  show_pool();
  printf("%p\n", vget);

  vget->error->error = pool_req(5, POOL_STRING);
  vget->error->str_word = pool_req(8, POOL_STRING);

  pool_add(OBJ_POOL, POOL_VALUE, vget);

  pool_free(OBJ_POOL);
}

/*! prints usage then exits */
void usage(int e) {
  math_free();
  pool_free(OBJ_POOL);
  printf("Usage: crank [-hqsv] [file...] [arg...]\n");
  exit(e);
}

void help() {
  math_free();
  pool_free(OBJ_POOL);
  printf("Usage: crank [-hqsv] [file...] [arg...]\n\n");
  printf(" -h    --help            print this help message\n");
  printf(" -q    --quiet           don't show stack information at program end\n");
  printf(" -s N  --sources N       specify N source files to be composed (default is N=1)\n");
  printf(" -v    --version         print version information\n");
  exit(0);
}

/*! prints version and exits */
void version() {
  math_free();
  pool_free(OBJ_POOL);
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
    print_str_formatted(EXIT_CODE);
    printf("'\n");
  }
  else
    printf("\nExit code: (none)\n");
}

/*! frees all global variables */
void global_free() {
  value_stack_free(ARGS);
  stack_free(OBJ_TABLE_STACK, ht_free_free);
  stack_free(OBJ_TABLE_REF_STACK, nop);
  string_free(ROOT);
  if (!EXITED) {
    contain_free(STACK->items[0]);
  }
  free(STACK->items);
  free(STACK);
  free(PARSER);
  stack_free(EVAL_CONTAINERS, nop);
  stack_free(CONTAIN_DEF_STACK, contain_free);
  stack_free(EVAL_CONTAIN_TRASH, nop);
  stack_free(MACRO_DEF_STACK, value_stack_free);
  stack_free(FAMILY, nop);
  string_free(FAMILY_IDX);
  stack_free(CONTAINERS, nop);
  stack_free(MACROS, nop);
  string_free(EXIT_CODE);
  for (int i = 0; i < 4; i++) {
    string_free(CAST_ARGS[i]);
  }
  free(CAST_ARGS);
  string_free(F);
  string_free(ING);
  math_free();
  pool_free(OBJ_POOL);
}

int main(int argc, char **argv) {
  value_t *v;
  size_t len = 0;
  char *locale = setlocale(LC_ALL, "");

  if (strcmp(argv[1], "pool_checker") == 0) {
    pool_checker();
    return 0;
  }

  OBJ_POOL = init_pool();
  init_math();

  /* Parsing arguments */
  if (argc < 2) {
    usage(1);
  }

  struct {
    bool h;
    bool q;
    int s;
    bool v;
  } args = {false, false, -1, false};

  int fileidx = 0;

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
    } else if (strcmp(argv[i], "-s") == 0 ||
               strcmp(argv[i], "--sources") == 0) {
      if (args.s >= 0 || i + 1 == argc) {
        usage(1);
        break;
      }
      i++;
      string_t *str = init_string(U"");
      for (int c = 0; argv[i][c] != '\0'; c++) {
        string_append(str, argv[i][c]);
      }
      if (!isint(str)) {
        usage(2);
        break;
      }
      args.s = string_to_int(str);
      pool_addobj(POOL_STRING, str);
    } else {
      fileidx = i;
      if (fileidx + args.s > argc) {
        printf("Missing filename\n");
        math_free();
        pool_free(OBJ_POOL);
        return 3;
        break;
      }
      break;
    }
  }

  if (args.s < 0) {
    args.s = 1;
  }
  if (args.h) {
    help();
  }
  if (args.v) {
    version();
  }
  if (!fileidx && args.s) {
    usage(1);
  }

  /* Set up global variables */
  ARGS = pool_req(DEFAULT_STACK_SIZE, POOL_STACK);
  stack_push(ARGS, pool_req(DEFAULT_STRING_LENGTH, POOL_VWORD));
  for (int i = fileidx + args.s; i < argc; i++) {
    value_t *argword = init_value(VWORD);
    argword->str_word = init_string(U"");
    for (int c = 0; argv[i][c] != '\0'; c++) {
      string_append(argword->str_word, argv[i][c]);
    }
    stack_push(ARGS, argword);
  }
  PARSER = init_parser(NULL);
  STACK = pool_req(DEFAULT_STACK_SIZE, POOL_STACK);
  CONTAIN_DEF_STACK = pool_req(DEFAULT_STACK_SIZE, POOL_STACK);
  MACRO_DEF_STACK = pool_req(DEFAULT_STACK_SIZE, POOL_STACK);
  EVAL_CONTAIN_TRASH = pool_req(DEFAULT_STACK_SIZE, POOL_STACK);
  FAMILY = pool_req(DEFAULT_STACK_SIZE, POOL_STACK);
  stack_push(FAMILY, NULL);
  FAMILY_IDX = pool_req(DEFAULT_STRING_LENGTH, POOL_STRING);
  CONTAINERS = pool_req(DEFAULT_STACK_SIZE, POOL_STACK);
  MACROS = pool_req(DEFAULT_STACK_SIZE, POOL_STACK);
  OBJ_TABLE_REF_STACK = pool_req(DEFAULT_STACK_SIZE, POOL_STACK);
  OBJ_TABLE_STACK = pool_req(DEFAULT_STACK_SIZE, POOL_STACK);
  stack_push(OBJ_TABLE_STACK, pool_req(0, POOL_HT));
  ROOT = pool_req(DEFAULT_STRING_LENGTH, POOL_STRING);
  EXIT_CODE = NULL;
  FAMILY_RECURSION_DEPTH = 0;
  EVAL_CONTAINERS = pool_req(DEFAULT_STACK_SIZE, POOL_STACK);
  RETURNED = false;
  CAST_ARGS = paw_alloc(4, sizeof(string_t*));
  CAST_ARGS[0] = init_string(U"VSTACK");
  CAST_ARGS[1] = init_string(U"0");
  CAST_ARGS[2] = init_string(U"VMACRO");
  CAST_ARGS[3] = init_string(U"1");
  F = init_string(U"f");
  ING = init_string(U"ing");

  /* initialise environment */
  contain_t *stack =
      init_contain(pool_req(0, POOL_HT), pool_req(0, POOL_HT), NULL);
  stack->faliases = init_stack(DEFAULT_STACK_SIZE);
  stack_push(stack->faliases, string_copy(F));
  stack_push(stack->faliases, string_copy(ING));
  add_funcs(stack->flit);
  stack_push(STACK, stack);
  stack_push(OBJ_TABLE_REF_STACK, stack);
  string_append(ROOT, 0);

  for (int i = 0; i < args.s; i++) {
    if (EXITED) break;
    /* Read code from file */
    FILE *FP = fopen(argv[fileidx + i], "rb");
    if (!FP) {
      printf("Could not open file for reading: %s\n", argv[fileidx + i]);
      global_free();
      return 4;
    }
    value_t *argword = ARGS->items[0];
    for (int c = 0; argv[fileidx + i][c] != '\0'; c++) {
      string_append(argword->str_word, argv[fileidx + i][c]);
    }

    string_t *buffer = file_read(FP);
    fclose(FP);

    parser_reset(PARSER, buffer);

    /* parse and eval loop */
    while (1) {
      v = parser_get_next(PARSER);
      if (v == NULL)
        break;
      eval(v);
      if (EXITED)
        break;
    }
    string_free(PARSER->source);
    argword->str_word->length = 0;
  }


  if (!args.q) {
    print_end();
  }

  /* Free all global variables */
  global_free();
  return 0;
}
