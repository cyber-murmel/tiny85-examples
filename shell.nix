{
  pkgs ? import <nixpkgs> {}
}:

with pkgs;
mkShell {
  buildInputs = [
    pkgsCross.avr.buildPackages.gcc
    avrdude
    micronucleus
    clang-tools
    pythonPackages.pyserial
  ];
}
