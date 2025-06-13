# shell.nix
{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  # Define the packages available in the development shell
  buildInputs = with pkgs; [
    qt6.full          # Full Qt6 development environment
    nlohmann_json     # nlohmann/json C++ library
    cmake             # CMake build system
    gcc               # C++ compiler (or clang)
    gnumake           # Make utility
    git               # Git version control
  ];

  shellHook = ''
    echo "Entering Nix development shell with Qt6, nlohmann_json, and CMake."
  '';
}
