{pkgs ? import <nixpkgs> {}}:
pkgs.mkShell {
  nativeBuildInputs = with pkgs; [
    raylib

    libGL

    # X11 dependencies
    # libX11
    # libX11.dev
    # libXcursor
    # libXi
    # libXinerama
    # libXrandr

    # Web support (uncomment to enable)
    # emscripten
    # nodejs
    python3

    gcc
    gnumake
    cmake
    ninja
    ccache

    clang-tools
  ];

  CMAKE_CXX_COMPILER_LAUNCHER = "ccache";

  # Audio dependencies
  LD_LIBRARY_PATH = with pkgs; lib.makeLibraryPath [alsa-lib];
}
