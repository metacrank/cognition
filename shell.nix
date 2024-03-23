{ pkgs,  ... }:
pkgs.mkShellNoCC {
  packages = with pkgs; [
    gcc
    bear
    valgrind
    tig
    doxygen
    nano
    gdb
  ];
}

