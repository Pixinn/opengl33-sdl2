This is a *bootstrap* project to use OpenGL3.3+ along with SDL2.

As a test, it will draw an orange rectangle in a SDL2 window. This test is heavily inspired from this [Tutorial](https://learnopengl.com/Getting-started/Hello-Triangle).

# Prerequisites

## conan.io

This project requires *SDL2* and *GLEW* and all paths to *GLM* will be set-up.
Those dependencies are handled by [*conan*](https://conan.io/).

Install *conan* and add the *bincrafters* public repository:
  > pip install conan
  
  > conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
  
## cmake

The build chain is handled by *cmake*.

# How to build

  > mkdir build && cd build
  
  > conan install ..
  
  > cmake .. -DCMAKE_BUILD_TYPE=Release
  
  > cmake --build . --config Release
  
**Please note** that you must be consistent between your conan 32/64 bit configuration and the build. Otherwise you'll get link errors.
