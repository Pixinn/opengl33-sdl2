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
conan_set_find_library_paths(${PROJECT_NAME})
conan_target_link_libraries(${PROJECT_NAME})
