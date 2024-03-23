{
  description = "A cognition devshell environment";

  inputs.flake-utils.url = "github:numtide/flake-utils";
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-23.11";

  outputs = { self, nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem
      (system:
        let pkgs = nixpkgs.legacyPackages.${system}; in
        {
          devShells.default = import ./shell.nix { inherit pkgs; };
          stdenv.mkDerivation =
            {
              shellHook = ''
 alias l="ls -la"
 alias c="clear"
 alias g="git"
 alias gp="git push"
 alias gc="git commit"
 alias gP="git pull"
'';
              name = "crank";
              src = self;
              buildPhase = ''
mkdir -p build src/builtins
gcc  -fpic -std=c11 -I include -c -o build/better_string.o src/better_string.c
gcc  -fpic -std=c11 -I include -c -o build/builtins.o src/builtins.c
gcc  -fpic -std=c11 -I include -c -o build/cognition.o src/cognition.c
gcc  -fpic -std=c11 -I include -c -o build/builtinslib.o src/builtinslib.c
gcc  -fpic -std=c11 -I include -c -o build/macros.o src/macros.c
gcc  -fpic -std=c11 -I include -c -o build/builtins/parser.o src/builtins/parser.c
gcc  -fpic -std=c11 -I include -c -o build/builtins/errors.o src/builtins/errors.c
gcc  -fpic -std=c11 -I include -c -o build/builtins/combinators.o src/builtins/combinators.c
gcc  -fpic -std=c11 -I include -c -o build/builtins/math.o src/builtins/math.c
gcc  -fpic -std=c11 -I include -c -o build/builtins/hashtable.o src/builtins/hashtable.c
gcc  -fpic -std=c11 -I include -c -o build/builtins/strings.o src/builtins/strings.c
gcc  -fpic -std=c11 -I include -c -o build/builtins/io.o src/builtins/io.c
gcc  -fpic -std=c11 -I include -c -o build/builtins/cranker.o src/builtins/cranker.c
gcc  -fpic -std=c11 -I include -c -o build/builtins/metastack.o src/builtins/metastack.c
gcc  -fpic -std=c11 -I include -c -o build/builtins/misc.o src/builtins/misc.c
gcc  -fpic -std=c11 -I include -c -o build/builtins/stackops.o src/builtins/stackops.c
gcc  -fpic -std=c11 -I include -c -o build/main.o src/main.c
gcc  build/better_string.o build/builtins.o build/cognition.o build/builtinslib.o build/macros.o build/builtins/parser.o build/builtins/errors.o build/builtins/combinators.o build/builtins/math.o build/builtins/hashtable.o build/builtins/strings.o build/builtins/io.o build/builtins/cranker.o build/builtins/metastack.o build/builtins/misc.o build/builtins/stackops.o build/main.o -o crank -L lib -lm
'';
              installPhase = ''sudo make install'';
            };
        }
      );
}
