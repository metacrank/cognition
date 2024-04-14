#include <builtins/misc.h>
#include <macros.h>
#include <builtins.h>
#include <builtinslib.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>

extern stack_t *STACK;
extern bool RETURNED;
extern stack_t *ARGS;
extern byte_t print_buffer[5];

void cog_reset(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  ht_free(cur->word_table, contain_def_stack_push);
  ht_free(cur->flit, macro_def_stack_push);
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
  stack_free(cur->err_stack, value_free_safe);
  string_free(cur->delims);
  cur->delims = NULL;
  string_free(cur->ignored);
  cur->ignored = NULL;
  string_free(cur->singlets);
  cur->singlets = NULL;
  cur->dflag = false;
  cur->iflag = true;
  cur->sflag = true;
}

void cog_clib(value_t *v) {
  contain_t *cur = stack_pop(STACK);
  value_t *v1 = stack_pop(cur->stack);
  if (!v1) {
    eval_error(U"TOO FEW ARGUMENTS", v);
    return;
  }
  if (value_stack(v1)[0]->size != 1) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(cur->stack, v1);
    return;
  }
  value_t *v1val = value_stack(v1)[0]->items[0];
  if (v1val->type != VWORD) {
    eval_error(U"BAD ARGUMENT TYPE", v);
    stack_push(cur->stack, v1);
    return;
  }
  string_t *str = v1val->str_word;
  size_t strlength = 0;
  for (long i = 0; i < str->length; i++) {
    utf32_to_utf8(print_buffer, str->value[i]);
    strlength += sizeof_utf8(print_buffer);
  }
  char *buf = malloc((strlength + 1) * sizeof(char));
  strlength = 0;
  for (long i = 0; i < str->length; i++) {
    utf32_to_utf8(print_buffer, str->value[i]);
    memcpy(buf + strlength, print_buffer, sizeof_utf8(print_buffer));
    strlength += sizeof_utf8(print_buffer);
  }
  buf[strlength] = '\0';
  void *handle = dlopen(buf, RTLD_LAZY);
  free(buf);
  if (!handle) {
    stack_push(cur->stack, v1);
    eval_error(U"INVALID FILENAME", v);
    return;
  }
  dlerror();
  void (*af)(void);
  void (*aobjs)(void);
  char *error;
  *(void **)(&af) = dlsym(handle, "add_funcs");
  *(void **)(&aobjs) = dlsym(handle, "add_objs");
  if ((error = dlerror()) != NULL) {
    value_free_safe(v1);
    fprintf(stderr, "%s\n", error);
    eval_error(U"DLSYM CANNOT FIND FUNCTION", v);
    return;
  }
  (*af)();
  (*aobjs)();
  dlclose(handle);
  value_free_safe(v1);
}

void cog_return(value_t *v) {
  RETURNED = true;
}

void cog_getargs(value_t *v) {
  contain_t *cur = stack_peek(STACK);
  contain_t *argc = calloc(1, sizeof(contain_t));
  argc->stack = value_stack_copy(ARGS);
  argc->iflag = true;
  argc->sflag = true;
  value_t *argv = init_value(VSTACK);
  argv->container = argc;
  stack_push(cur->stack, argv);
}

void add_funcs_misc(ht_t *flit) {
  add_func(flit, cog_reset, U"reset");
  add_func(flit, cog_clib, U"clib");
  add_func(flit, cog_return, U"return");
  add_func(flit, cog_getargs, U"getargs");
}
