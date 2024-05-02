{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  name = "cognition";
  buildInputs = with pkgs; [ gcc doxygen glibc ];
  packages = with pkgs; [
    gcc
    bear
    valgrind
    doxygen
    gdb
    glibc
  ];
}

