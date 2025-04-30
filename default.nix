{ pkgs }:

with pkgs; stdenv.mkDerivation rec {
  name = "diffraction-visualize";

  src = ./.;

  nativeBuildInputs = with pkgs; [
    cmake
    gcc
    ninja
    pkg-config
    fftw
  ];

  buildInputs = with pkgs; [
    fftw
  ];
}
