#ifndef BUILTINS_H_
#define BUILTINS_H_
#include <cognition.h>

/* push error to error stack */
void eval_error(char *s);

/*! @brief dds functions to flit table */
void add_funcs(ht_t* flit);

#endif // BUILTINS_H_
