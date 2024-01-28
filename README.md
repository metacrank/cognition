# Introduction
Stem aims to be a minimal interpreted stack based programming language
that allows for metaprogramming and a foreign language interface. It features
a C API that is elegant and simple. Additionally, garbage collection is
not needed in this language as the responsibility of writing memory safe
operations is put in the hands of the maintainers of the builtin functions
and FLI library maintainers. Therefore, the end user does not need to worry
about memory allocation while the implementation remains extremely simple.

# Motivation
This language was designed after I heard about stack based programming languages
from my friend Matthew Hinton. I made it according to his description, and made
choices along the way in the design process according to what I thought was logical.

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

# Documentation
See the [Doxygen generated documentation](https://stemdoc.nullring.xyz) for the C API documentation and
[my blog post](https://ret2pop.nullring.xyz/blog/stem.html).
