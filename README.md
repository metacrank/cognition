# Introduction
Cognition is a fully introspective system designed so that the syntax and hierarchy
structure of the language is fully _public_; that is, a file that contains cognition
code can alter the way that it is being parsed, in real time. Cognition allows for this
because the language is completely postfix with extremely minimal syntax, and what exists
of the syntax can be changed at will. Because the language _never_ reads more than it has to,
and because the language allows for metaprogramming (talking about symbols as if they are
data, as well as code), the syntax of the language is made fluid. This allows for the advanced
manipulation of how the next token is tokenized, and how these tokens are arranged into
something like the AST without having to explicitly program a rigid syntax.

# Install
On unix-likes:
```bash
git clone --recursive-submodules https://github.com/ret2pop/cognition
cd cognition/
make
sudo make install
```
And on NixOS:
```
git clone --recursive-submodules https://github.com/ret2pop/cognition
cd cognition/
nix-shell
make
sudo make install
```
Note that if you're not on NixOS, you will have to install the dependency `doxygen`
if you want to compile the documentation.
