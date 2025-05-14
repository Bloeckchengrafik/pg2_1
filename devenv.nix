{ pkgs, lib, config, inputs, ... }:

{
  env.GREET = "pg2-1";

  # https://devenv.sh/packages/
  packages = [
    pkgs.git
    pkgs.nlohmann_json
    pkgs.qt6.full
  ];

  languages.cplusplus.enable = true;

  scripts.run.exec = ''
    mkdir -p build
    cd build
    cmake ..
    cmake --build .
    cd ..
    ./build/pg2_1
  '';
}
