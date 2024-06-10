#ifndef COGNITION_H_
#define COGNITION_H_
#include <better_string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <hash_table.h>

typedef struct VALUE_STRUCT value_t;
typedef struct STACK_STRUCT stack_t;
typedef struct CONTAINER_STRUCT contain_t;
typedef struct ERROR_STRUCT error_t;

/*! @brief non-generic stack data structure used for stacks and quotes */
/*! stack_t holds items of type value_t *; see VALUE_STRUCT for more details*/
struct STACK_STRUCT {
  /*! @brief Allowed items */
  void **items;
  /*! @brief total number of elements */
  size_t size;
  /*! @brief the buffer size */
  size_t capacity;
};

/* holds a str_word with the name of the word that produced the error, and an error string */
struct ERROR_STRUCT {
  string_t *error;
  string_t *str_word;
};

/*! @brief holds a string, int, float, stack, or function pointer, as well as custom
 * datatypes. */
/*! VCUSTOM allows for definitions of custom structs, where the name of such a
 *custom data structure is to be held in str_word, and the actual custom struct
 *is to be held in custom. custom_t allows for the definition of required
 *functions in order for the builtin functions to remain memory safe. */
struct VALUE_STRUCT {
  /*! @brief Enum that defines different types within the language. */
  enum { VWORD, VSTACK, VMACRO, VERR, VCUSTOM, VCLIB } type;
  union {
    /*! @brief A container is a complete cognition environment, and can contain more containers. */
    contain_t *container;
    /* a stack to hold a macro (a stack of values that gets executed without any cranking) */
    stack_t *macro;
    /*! @brief this holds the string value of a string, word, or the name of a
     * custom type. */
    string_t *str_word;
    /* holds the name and word reference for a VERR */
    error_t *error;
  };
  /*! @brief this variable holds the value of a custom type, clib function pointer, or pointer to
   * a copy of the str_word that resulted in the VERR */
  void *custom;
};

/*! @brief Parser implementation directly parses without lexer */
/*! the parser data structure parses a string of valid stem code and
 * returns a value until it reaches EOF or end of string. */
typedef struct PARSER_STRUCT {
  /*! @brief The string that contains valid stem code. */
  string_t *source;
  /*! @brief Index of current character */
  int i;
  /*! @brief The current character */
  char32_t c;
} parser_t;


/*! @brief stores functions for custom structs */
/*! custom_t hold three functions that is to be stored in a hash table named
 *OBJ_TABLE. The key is the custom struct name, and the values are three
 *functions that make the builtin functions memory safe. */
typedef struct {
  /*! @brief Function that prints the custom struct */
  void (*printfunc)(FILE *f, void *);
  /*! @brief deep copy of custom struct */
  void *(*copyfunc)(void *);
  /*! @brief free function for custom struct */
  void (*freefunc)(void *);
} custom_t;

/*! @brief container for a complete cognition environment. Cognition
 * environments can contain other cognition environments. */
struct CONTAINER_STRUCT {
  /*! @brief a stack of containers */
  stack_t *stack;
  /*! @brief a stack of errors emitted by executing words */
  stack_t *err_stack;
  /*! @brief a hash table that stores words defined in the environment */
  ht_t *word_table;
  /*! @brief foreign language interface table */
  ht_t *flit;
  /*! @brief crank array */
  stack_t *cranks;
  /*! @brief The list of delimiter characters */
  string_t *delims;
  /*! @brief false is delimiter blacklist */
  bool dflag;
  /*! @brief aliases for evalf */
  stack_t *faliases;
  /*! @brief list of ignored characters */
  string_t *ignored;
  /*! @brief false is ignored blacklist */
  bool iflag;
  /*! list of singlet characters */
  string_t *singlets;
  /*! false is singlet blacklist */
  bool sflag;
};

/*! Useless function that is only used in order to be passed into a hash table.
 */
void func_free(void *f);

// for debugging
void print_crank(char prefix[]);

/* push error to error stack */
void eval_error(char32_t *s, value_t *w);

/*! Allocates memory for new stack */
stack_t *init_stack(size_t size);

/*! pushes element to back of stack */
void stack_push(stack_t *a, void *v);

/*! add element to stack at index */
void stack_add(stack_t *a, value_t *v, int index);

/*! pops last element off of stack */
void *stack_pop(stack_t *a);

/* pops deep element in stack */
void *stack_popdeep(stack_t *a, int index);

/* returns pointer to top of stack */
void *stack_peek(stack_t *s);

/*! Deep copy of stack and its contents. */
void *stack_copy(void *a, void *(*copyfunc)(void *));

/* free stacks with value_t's in them */
void value_stack_free(void *a);

/* copy stacks with value_t's in them */
void *value_stack_copy(void *a);

/*! Concatenate two stacks and put the result in a1. */
void stack_extend(stack_t *a1, stack_t *a2);

/*! Free elements in stack and set stack size to zero. */
void stack_empty(void *a, void (*freefunc)(void *));

/* returns true if and only if e is in stack */
bool stack_exists(stack_t *a, void *e);

/*! Free stack and all value_t elements. */
void stack_free(void *a, void (*freefunc)(void *));

/*! Allocates memory for new value_t. */
value_t *init_value(int type);

/*! deep copy of value struct. */
void *value_copy(void *v);

/*! Frees value struct */
void value_free(void *v);

/* Frees an error type */
void error_free(void *v);

/* copies an error type */
error_t *error_copy(void *e);

/* frees an object table */
void obj_free(void *v);

/*! Allocates memory for new custom_t */
custom_t *init_custom(void (*)(FILE *, void *), void (*)(void *), void *(*)(void *));

/*! Frees custom_t *. */
void custom_free(void *);

/*! Adds function to FLIT. */
void add_func(ht_t *h, void (*func)(value_t *), char32_t *key);

/* Adds function stack to FLIT */
void add_macro(ht_t *h, stack_t *macro, char32_t *key);

/*! Adds object functions to OBJ_TABLE */
void add_obj(ht_t *h, void (*printfunc)(FILE *, void *), void (*freefunc)(void *),
             void *(*copyfunc)(void *), char32_t *key);

/*! Allocates memory for new container */
contain_t *init_contain(ht_t *h, ht_t *flit, stack_t *cranks);

/* Copies container structure */
contain_t *contain_copy(contain_t *c, void *(*copyfunc)(void *));

/* Copies a container containing value_t */
void *contain_value_copy(void *c);

/*! Allocates memory for new container */
void contain_free(void *con);

/* copies faliases */
void *falias_copy(void *f);

/*! Allocates memory for new parser */
parser_t *init_parser(string_t *source);

/*! Moves parser by one character. */
void parser_move(parser_t *p);

/*! Resets state of parser */
void parser_reset(parser_t *p, string_t *source);

/* gets the next word */
value_t *parse_word(parser_t *p, bool skipped);

/* returns true if ignored characters were skipped, false if none were */
bool parser_skip_ignore(parser_t *p);

/*! Gets the next value_t from the string, returns NULL if EOF. */
value_t *parser_get_next(parser_t *p);

/* returns true if p is a singlet character */
bool issinglet(char32_t c);

/* returns true if p is an ignored character */
bool isignore(char32_t c);

/* returns true if p is a delimiter character */
bool isdelim(char32_t c);

/* checks if a value_t is an falias in a container */
bool isfaliasin(contain_t *c, value_t *v);

/* checks if a value_t is an falias */
bool isfalias(value_t *v);

/* crankless evaluation on the stack */
void evalf(value_t *alias);

/* increments and decrements crank stack */
void inc_crank(contain_t *cur);
void dec_crank(contain_t *cur);

/* copies crank stack */
void *cranks_copy(void *cranks);

/* pushes a value to a container */
void contain_push(contain_t *c, value_t *v);

void push_quoted(contain_t *cur, value_t *v);

/* eval's a value in a stack being evalstack'd */
void eval_value(contain_t *c, contain_t *cur, value_t **val, value_t *callword, bool always_evalf, bool destructive);

bool return_function(void *stack, bool macro);

/* recursively evaluates a stack, with cranking. defcontain is the container of definition,
 * callstack is the stack calling callval, which contains the callval pointer */
void evalstack(contain_t *c, value_t *callword, bool destructive);

/* recursively evaluates a flit macro without cranking */
void evalmacro(stack_t *macro, value_t *word, bool destructive);

/* expands and recursively evaluates a word value, with cranking */
void evalword(value_t *v, bool always_evalf);

/* performs one crank */
void crank();

/*! Evaluates a value returned by the parser. */
void eval(value_t *v);

#endif // COGNITION_H_
