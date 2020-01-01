cmake_minimum_required(VERSION 3.10)

# Project name - should not contain whitespace 
project(Bootstrap_OpenGL33_Sdl2)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

#========== non-conan dependencies =============#
find_package(OpenGL REQUIRED COMPONENTS OpenGL)

#========== Global Configurations =============#

set(CMAKE_CXX_STANDARD 14)     
set(CMAKE_VERBOSE_MAKEFILE ON)


#========== Targets Configurations ============#
add_executable(${PROJECT_NAME} src/bootstrap_ogl33_sdl2.cpp)

# Set executable dependency libraries
target_include_directories(${PROJECT_NAME} PRIVATE ${CONAN_INCLUDE_DIRS_GLEW} ${CONAN_INCLUDE_DIRS_SDL2})
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS_GLEW}  ${CONAN_LIBS_SDL2})
