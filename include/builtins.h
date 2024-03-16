#ifndef BUILTINS_H_
#define BUILTINS_H_
#include <cognition.h>
#include <builtins/combinators.h>
#include <builtins/cranker.h>
#include <builtins/errors.h>
#include <builtins/hashtable.h>
#include <builtins/io.h>
#include <builtins/math.h>
#include <builtins/metastack.h>
#include <builtins/misc.h>
#include <builtins/parser.h>
#include <builtins/stackops.h>
#include <builtins/strings.h>

/*! @brief dds functions to flit table */
void add_funcs(ht_t *flit);

#endif // BUILTINS_H_
