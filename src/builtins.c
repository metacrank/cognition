#include <builtins.h>

void add_funcs(ht_t *flit) {
  add_funcs_combinators(flit);
  add_funcs_cranker(flit);
  add_funcs_errors(flit);
  add_funcs_hashtable(flit);
  add_funcs_io(flit);
  add_funcs_math(flit);
  add_funcs_metastack(flit);
  add_funcs_misc(flit);
  add_funcs_parser(flit);
  add_funcs_stackops(flit);
  add_funcs_strings(flit);
}
