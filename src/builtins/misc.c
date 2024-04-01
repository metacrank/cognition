#include <builtins/misc.h>
#include <macros.h>
#include <builtins.h>

extern stack_t *STACK;

void cog_reset(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  ht_free(cur->word_table, contain_free);
  ht_free(cur->flit, value_stack_free);
  cur->word_table = init_ht(DEFAULT_HT_SIZE);
  cur->flit = init_ht(DEFAULT_HT_SIZE);
  add_funcs(cur->flit);
  string_t *str;
  while((str = stack_pop(cur->faliases))) {
    string_free(str);
  }
  stack_push(cur->faliases, init_string(U"f"));
  stack_free(cur->cranks, free);
  cur->cranks = NULL;
  stack_free(cur->err_stack, value_free);
  string_free(cur->delims);
  cur->delims = NULL;
  string_free(cur->ignored);
  cur->delims = NULL;
  string_free(cur->singlets);
  cur->delims = NULL;
  cur->dflag = false;
  cur->iflag = true;
  cur->sflag = true;
}

void add_funcs_misc(ht_t *flit) {
  add_func(flit, cog_reset, U"reset");
}
