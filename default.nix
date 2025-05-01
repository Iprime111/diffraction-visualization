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
    xorg.libXcursor
    xorg.libXrandr
    xorg.libXi
    xorg.libX11
    libGL
    udev
    freetype
    libvorbis
    flac
  ];

  buildInputs = with pkgs; [
    fftw
    xorg.libXcursor
    xorg.libXrandr
    xorg.libXi
    xorg.libX11
    libGL
    udev
    freetype
    libvorbis
    flac
  ];
}
