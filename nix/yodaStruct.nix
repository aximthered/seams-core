# [[file:~/Git/Github/C++/Forks/structureFactor/literateNix.org::*Expression][Expression:1]]
# Using patterns, and white space negligence
{ clangStdenv
, catch2
, fmtlib
, yamlCpp
, sharkML
, lua
, conan
, luaPackages
, lib
, boost
, cmake }:
  clangStdenv.mkDerivation {
  name = "yodaStruct";
  src = lib.cleanSource ../.;
  nativeBuildInputs = [
  catch2
  fmtlib
  cmake
  lua
  conan
  ];
  buildInputs = [
  yamlCpp
  sharkML
  boost
  luaPackages.luafilesystem
  ];
  }
# Expression:1 ends here
