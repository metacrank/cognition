#include <macros.h>
#include <stdio.h>
#include <stdlib.h>

void die(void *message) {
  fprintf(stderr, "ERROR: %s\n", (char *)message);
  exit(1);
}

