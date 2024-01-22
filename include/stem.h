#ifndef PARSER_H_
#define PARSER_H_
#include <better_string.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct VALUE_STRUCT value_t;
typedef struct ARRAY_STRUCT array_t;

/*! @brief non-generic array data structure used for stacks and quotes */
/*! array_t holds items of type value_t *; see VALUE_STRUCT for more details*/
struct ARRAY_STRUCT {
  /*! @brief Allowed items */
  value_t **items;
  /*! @brief total number of elements */
  size_t size;
  /*! @brief the buffer size */
  size_t capacity;
};

/*! @brief holds a string, int, float, word, or quote, as well as custom
 * datatypes. */
/*! VCUSTOM allows for definitions of custom structs, where the name of such a
 *custom data structure is to be held in str_word, and the actual custom struct
 *is to be held in custom. custom_t allows for the definition of required
 *functions in order for the builtin functions to remain memory safe. */
struct VALUE_STRUCT {
  /*! @brief Enum that defines different types within the language. */
  enum { VWORD, VINT, VFLOAT, VSTR, VQUOTE, VERR, VCUSTOM } type;
  union {
    /*! @brief floats and ints in this language are to be stored in this
     * variable. */
    long double int_float;
    /*! @brief A quote is an array of more values, which can contain more
     * quotes. */
    array_t *quote;
    /*! @brief this holds the string value of a string, word, or the name of a
     * custom type. */
    string_t *str_word;
  };
  /*! @brief this variable holds the value of a custom type. */
  void *custom;
  /*! @brief only to be used with values of type word */
  /*! This variable tells the evaluator if the word in question needs to be
   * evaluated as a funtion or pushed onto the stack. */
  bool escaped;
};

/*! @brief Parser implementation directly parses without lexer */
/*! the parser data structure parses a string of valid stem code and
 * returns a value until it reaches EOF or end of string. */
typedef struct PARSER_STRUCT {
  /*! @brief The string that contains valid stem code. */
  char *source;
  /*! @brief Index of current character */
  int i;
  /*! @brief The current character */
  char c;
} parser_t;

/*! @brief This structure is to be used in singly linked lists that hold
 * key-value pairs for a hash table. */
typedef struct NODE_STRUCT {
  /*! @brief Stores the key */
  string_t *key;
  /*! @brief Stores the value */
  void *value;
  /*! @brief Stores the next node in the singly linked list */
  struct NODE_STRUCT *next;
} node_t;

/*! @brief Singly Linked List used for separate chaining for a hash table */
typedef struct {
  /*! @brief The first node. Is NULL until value is added to sll. */
  node_t *head;
  /*! @brief current size of hash table */
  size_t size;
} sll_t;

/*! @brief hash table implementation */
/*! this hash table stores a value in a numbered bucket according to what number
 * the key hashes to; each bucket is a singly linked list to handle collisions
 */
typedef struct {
  /*! @brief Stores the key-value pairs */
  sll_t **buckets;
  /*! @brief Total number of buckets in hash table */
  size_t size;
} ht_t;

/*! @brief stores functions for custom structs */
/*! custom_t hold three functions that is to be stored in a hash table named
 *OBJ_TABLE. The key is the custom struct name, and the values are three
 *functions that make the builtin functions memory safe. */
typedef struct {
  /*! @brief Function that prints the custom struct */
  void (*printfunc)(void *);
  /*! @brief deep copy of custom struct */
  void *(*copyfunc)(void *);
  /*! @brief free function for custom struct */
  void (*freefunc)(void *);
} custom_t;

/*! Useless function that is only used in order to be passed into a hash table.
 */
void func_free(void *f);

/*! Allocates memory for new array */
array_t *init_array(size_t size);

/*! appends element to back of array */
void array_append(array_t *a, value_t *v);

/*! appends element to front of array */
void array_curry(array_t *a, value_t *v);

/*! pops last element off of array */
value_t *array_pop(array_t *a);

/*! Deep copy of array and its contents. */
array_t *array_copy(array_t *a);

/*! Concatenate two arrays and put the result in a1. */
void array_extend(array_t *a1, array_t *a2);

/*! Free array and all value_t elements. */
void array_free(array_t *a);

/*! Allocates memory for new value_t. */
value_t *init_value(int type);

/*! deep copy of value struct. */
value_t *value_copy(value_t *v);

/*! Frees value struct */
void value_free(void *v);

/*! Allocates memory for new custom_t */
custom_t *init_custom(void (*)(void *), void (*)(void *), void *(*)(void *));

/*! Frees custom_t *. */
void custom_free(void *);

/*! Adds function to FLIT. */
void add_func(ht_t *h, void (*func)(value_t *), char *key);

/*! Adds object functions to OBJ_TABLE and adds constructor for custom type to
 * FLIT. */
void add_obj(ht_t *h, ht_t *h2, void (*printfunc)(void *),
             void (*freefunc)(void *), void *(*copyfunc)(void *),
             void (*createfunc)(void *), char *key);

/*! Allocates memory for new parser */
parser_t *init_parser(char *source);

/*! Removes comments from string s. */
parser_t *parser_pp(char *s);

/*! Resets state of parser */
void parser_reset(parser_t *p, char *source);

/*! Moves parser by one character if able */
void parser_move(parser_t *p);

/*! skips all whitespace between important characters. */
void parser_skip_whitespace(parser_t *p);

/*! returns VSTR value_t, called when '"' character is seen */
value_t *parse_string(parser_t *p);

/*! returns VQUOTE value_t, called when `[` character is seen */
value_t *parse_quote(parser_t *p);

/*! returns VINT or VFLOAT if the character is a digit. */
value_t *parse_num(parser_t *p);

/*! returns VWORD value_t, called if not any other type. If word starts with
 * `\`, then the word is escaped. */
value_t *parse_word(parser_t *p);

/*! Error in parsing strings can occur if wrong escape code. */
void parser_error(parser_t *p);

/*! Gets the next value_t from the string, returns NULL if EOF. */
value_t *parser_get_next(parser_t *p);

/*! Allocates memory for new node struct. */
node_t *init_node(string_t *key, void *v);

/*! Frees node */
void node_free(node_t *n, void (*freefunc)(void *));

/*! Allocates memory for new singly linked list */
sll_t *init_sll();

/*! Adds value to singly linked list. If key already exists, the value is
 * replaced and the old one is freed according to freefunc. */
void sll_add(sll_t *l, string_t *key, void *v, void (*freefunc)(void *));

/*! Gets value by key from singly linked list. */
void *sll_get(sll_t *l, string_t *key);

/*! deletes item from singly linked list */
void sll_delete(sll_t *l, string_t *k, void (*freefunc)(void *));

/*! Frees singly linked list */
void sll_free(sll_t *l, void (*freefunc)(void *));

/*! Allocates memory for new hash table */
ht_t *init_ht(size_t size);

/*! add key value pair to hash table. If the key already exists, the value is
 *replaced and the old value is freed according to freefunc. */
void ht_add(ht_t *h, string_t *key, void *v, void (*freefunc)(void *));

/*! Gets value from hash table by key */
void *ht_get(ht_t *h, string_t *key);

/*! Deletes item from hash table */
void ht_delete(ht_t *h, string_t *key, void (*freefunc)(void *));

/*! returns true if key exists in hash table. false otherwise */
bool ht_exists(ht_t *h, string_t *key);

/*! Frees hash table */
void ht_free(ht_t *h, void (*freefunc)(void *));

/*! hashes key into integer for hash table */
unsigned long hash(ht_t *h, char *key);

/*! returns true if word found in FLIT table, and then executes that function */
bool eval_builtins(value_t *v);

/*! returns true if word found in WORD_TABLE, and then executes that function */
bool eval_ht(value_t *v);

/*! Evaluates a value returned by the parser. */
void eval(value_t *v);

#endif // PARSER_H_
