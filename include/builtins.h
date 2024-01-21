#ifndef BUILTINS_H_
#define BUILTINS_H_
#include <stem.h>

/*! @brief adds two numbers together, pushes result on the stack */
void stemadd(value_t *v);

/*! @brief subtracts two numbers together, pushes result on the stack */
void stemsub(value_t *v);

/*! @brief multiplies two numbers together, pushes result on the stack */
void stemmul(value_t *v);

/*! @brief divides two numbers together, pushes result on the stack */
void stemdiv(value_t *v);

/*! @brief [word] [quote] func, creates function with name [word] and value
 * [quote] */
void stemfunc(value_t *v);

/*! @brief takes first number to the power of the second, pushes result on the
 * stack */
void stempow(value_t *v);

/*! @brief takes sine of first value on the stack */
void stemsin(value_t *v);

/*! @brief takes cosine of first value on the stack */
void stemcos(value_t *v);

/*! @brief takes exp of first value on the stack */
void stemexp(value_t *v);

/*! @brief evaluates the first value on the stack */
void stemeval(value_t *v);

/*! @brief takes ln of first value on the stack */
void stemln(value_t *v);

/*! @brief takes ceiling function of first value on the stack */
void stemceil(value_t *v);

/*! @brief takes floor function of first value on the stack */
void stemfloor(value_t *v);

/*! @brief converts valid stem code string and converts it into a quote, pushes
 * quote onto the stack */
void strquote(value_t *v);

/* @brief [value] [quote] curry  */
void curry(value_t *v);

/*! @brief does literally nothing */
void nop(value_t *v);

/*! @brief reads a file into a string, returns on the stack */
void stemfread(value_t *v);

/*! @brief reads stdin into string */
void stemread(value_t *v);

/*! @brief exits the program */
void stemexit(value_t *v);

/*! @brief quotes first value on the stack */
void quote(value_t *v);

/*! @brief returns type of first stack element as an integer */
void stemtype(value_t *v);

/*! @brief discards and frees the first element of the stack */
void dsc(value_t *v);

/*! @brief swaps the top two values of the stack */
void swap(value_t *v);

/*! @brief [VWORD] isdef; returns 1 if name of function is defined */
void isdef(value_t *v);

/*! @brief deep copies first value of the stack */
void stemdup(value_t *v);

/*! @brief prints the entire stack */
void questionmark(value_t *v);

/*! @brief pops first value off of the stack and prints it */
void period(value_t *v);

/*! @brief [VWORD/VSTR/VQUOTE] len; returns length of value */
void stemlen(value_t *v);

/*! @brief pops first element, evaluates second element, then pushes first
 * element back */
void dip(value_t *v);

/*! @brief keeps value on the stack after evaluating the value */
void keep(value_t *v);

/*! @brief deletes value at index from quote, string, or word */
void del(value_t *v);

/*! @brief clears the stack and frees all elements */
void clear(value_t *v);

/*! @brief [cond] [if true] [if false] if; evaluates 2nd term if cond is true;
 * otherwise evaluates the other one. */
void stemif(value_t *v);

/*! @brief greater than or equals */
void gtequals(value_t *v);

/*! @brief less than or equals */
void ltequals(value_t *v);

/*! @brief greater than */
void gthan(value_t *v);

/*! @brief less than */
void lthan(value_t *v);

/*! @brief polymorphic equals; arguments cannot be quotes */
void equals(value_t *v);

/*! @brief polymorphic not equals; arguments cannot be quotes */
void nequals(value_t *v);

/*! @brief converts word to string */
void wtostr(value_t *v);

/*! @brief joins two quotes together, pushes a new quote. Also joins two strings
 * together and pushes a new string. */
void compose(value_t *v);

/*! @brief checks if string is a number. */
void isnum(value_t *v);

/*! @brief string to integer */
void stoi(value_t *v);

/*! @brief returns current stack size */
void ssize(value_t *v);

/*! @brief puts entire stack into a quote, then pushes that quote onto the stack
 */
void qstack(value_t *v);

/*! @brief [number] [VQUOTE/VSTR/VWORD] vat; gets the value at a certain index.
 */
void vat(value_t *v);

/*! @brief */
void stemfwrite(value_t *v);

/*! @brief [VINT/VFLOAT] sleep; sleeps for an amount of seconds. */
void stemsleep(value_t *v);

void stemcut(value_t *v);

void undef(value_t *v);

/*! @brief adds all the custom objects defined to OBJ_TABLE */
void add_objs();

/*! @brief Adds functions to FLIT table */
void add_funcs();

#endif // BUILTINS_H_
