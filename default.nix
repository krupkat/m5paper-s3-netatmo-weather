with import <nixpkgs> { };

mkShell {
  nativeBuildInputs = [
    platformio
    llvmPackages_21.clang-unwrapped
    #ccls
  ];
}
