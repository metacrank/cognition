#include <builtins.h>
#include <builtins/combinators.h>
#include <builtins/cranker.h>
#include <builtins/hashtable.h>
#include <builtins/io.h>
#include <builtins/math.h>
#include <builtins/metastack.h>
#include <builtins/misc.h>
#include <builtins/parser.h>
#include <builtins/stackops.h>

extern stack_t *STACK;

void eval_error(char *s) {
  value_t *v = init_value(VERR);
  v->str_word = init_string(s);
  contain_t *cur = stack_peek(STACK);
  stack_push(cur->err_stack, v);
}

void add_funcs(ht_t *flit) {
  add_funcs_combinators(flit);
  add_funcs_cranker(flit);
  add_funcs_hashtable(flit);
  add_funcs_io(flit);
  add_funcs_math(flit);
  add_funcs_metastack(flit);
  add_funcs_misc(flit);
  add_funcs_parser(flit);
  add_funcs_stackops(flit);
}
