#include "macros.h"
#include <stdio.h>
#include <stdlib.h>

void die(char *message) {
  fprintf(stderr, "ERROR: %s\n", message);
  exit(1);
}
