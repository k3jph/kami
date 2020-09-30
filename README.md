# Kami is Agent-Based Modeling in Modern C++

# Compiling

```Bash

# Build, test and install the library
git clone https://github.com/k3jph/kami.git
cd kami
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/tmp/kami -DBUILD_SHARED_LIBS:BOOL=TRUE
cmake --build .
ctest -C Debug
cmake --build . --target install
```

# Conan Package Manager

This library comes with a `conanfile.py` to build it into a Conan package so that
it may be used by external applications/libraries.

## Create Conan Packages

Creating a Conan package is relatively easy. Simple cd into the source directory
and execute the conan create function.

```bash
git clone https://github.com/k3jph/kami.git
cd kami

conan create . kami/develop
```

## Contribution guidelines

* Use [GitFlow](http://nvie.com/posts/a-successful-git-branching-model/)

## For more information

* James P. Howard, II <<james.howard@jhu.edu>>