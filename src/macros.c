#include <macros.h>
#include <stdio.h>
#include <stdlib.h>
#include <cognition.h>

extern stack_t *STACK;

void die(char *message) {
  fprintf(stderr, "ERROR: %s\n", message);
  exit(1);
}

void print_crank(char prefix[]) {
  contain_t *cur = stack_peek(STACK);
  int mod = 0;
  int base = 0;
  int(*cr)[2] = NULL;
  if (cur->cranks)
    if (cur->cranks->size)
      cr = cur->cranks->items[0];
  if (cr) {
    mod = cr[0][0];
    base = cr[0][1];
  }
  printf("%s: modcrank %d, crankbase %d\n", prefix, mod, base);
}
