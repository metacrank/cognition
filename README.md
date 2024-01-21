# Introduction
Stem aims to be a minimal interpreted stack based programming language
that allows for metaprogramming and a foreign language interface. It features
a C API that is elegant and simple. Additionally, garbage collection is
not needed in this language as the responsibility of writing memory safe
operations is put in the hands of the maintainers of the builtin functions
and FLI library maintainers. Therefore, the end user does not need to worry
about memory allocation while the implementation remains extremely simple.

# Installation
`make` and `sudo make install`. To generate the html documentation, one must first
install `doxygen` as an optional dependency. If you are on a BSD or MacOS, you
must use `gmake`.

# Documentation
Builtin function definitions and the C API in general are documented [here](https://stemdoc.nullring.xyz).

# Quickstart
Because this is a stack based language, all operations are done in reverse polish. For example, to add two numbers together:
```
3 4 +
```
would return the result `7`. `3` and `4` get pushed onto the stack when they are written out as literals in the language, and `+` is a builtin
that adds the top two elements of the stack. In this language, there are two kinds of objects: literals
(strings, ints, floats, words, and literals of type `VERROR` are built in), and quotes that contain these literals (arrays of valid statements that can be evaluated).
Words might be new to you if you're coming from another language. If you're coming from lisp, a words are analogous to symbols. If you're coming from another
language, a word is a literal that has the special property that they can be bound to functions. For example, the `+` symbol is a word, which is bound
to the action of adding two numbers when called.

Let's look at a real example of a REPL implementation in this language:
```
repl [ "> " . read strquote eval repl ] def
repl
```
`repl` is a word, which means it is a literal, and everything that is a literal gets pushed onto the stack.
Everything between the `[` and `]` is an element in a quote. Then, we see the `def` word. If a word is already bound to a function,
the function gets called instead of getting pushed to the stack, so the `def` function gets called, which takes the top two
elements off the stack, and creates a function called `repl` where now every time `repl` is called in the future, the quote is evaluated
instead.

Let's take a closer look at the quote:
```
"> " . read strquote eval repl
```
`.` takes the first thing off the stack and prints it. In this case, it would print a prompt `> ` every REPL loop. `read` reads a value from stdin,
then `strquote` turns that string into a quote. `eval` pops the first thing off the stack and evaluates the quote in the same way calling a function
does, and then finally `repl` gets called again at the end so we can loop forever.

## Factorial
Let's take a closer look at the factorial function:
```
factorial [ dup 0 <= [ 1 + ] [ dup 1 - factorial * ] if ] def
```
