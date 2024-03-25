{ pkgs,  ... }:
pkgs.mkShellNoCC {
  shellHook = ''
alias c="clear"
alias l="ls -la"
alias g="git"
alias gp="git pull"
alias gP="git push"
alias gc="git commit -m"
'';
  buildInputs = with pkgs; [ gcc doxygen glibc ];
  packages = with pkgs; [
    gcc
    bear
    valgrind
    tig
    doxygen
    nano
    vim
    gdb
    git
    glibc
  ];
}

