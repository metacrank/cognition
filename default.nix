{ pkgs ? import <nixpkgs> {} }: 

pkgs.stdenv.mkDerivation {
  name = "cognition";
  src = ./.;
  buildPhase = ''
  substituteInPlace Makefile --replace /usr/local $out
  make
  '';
  installPhase = ''
  mkdir -p $out/bin
  mkdir -p $out/include
  make install
  '';
}

