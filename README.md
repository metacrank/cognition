# Introduction
Stem aims to be a minimal interpreted stack based programming language
that allows for metaprogramming and a foreign language interface. It features
a C API that is elegant and simple. Additionally, garbage collection is
not needed in this language as the responsibility of writing memory safe
operations is put in the hands of the maintainers of the builtin functions
and FLI library maintainers. Therefore, the end user does not need to worry
about memory allocation while the implementation remains extremely simple.

# Installation
In the terminal:
``` sh
git clone https://github.com/ret2pop/stem
cd stem/
make
sudo make install
```
To generate the html documentation, one must first install `doxygen` as an optional dependency.
If you are on a BSD or MacOS, you must use `gmake`. Also, you obviously need `git` to clone this
repository.
