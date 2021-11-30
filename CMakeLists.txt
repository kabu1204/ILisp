cmake_minimum_required(VERSION 3.10)
project(ILisp)

set(CMAKE_CXX_STANDARD 11)

IF (CMAKE_BUILD_TYPE STREQUAL Debug)
ADD_DEFINITIONS(-DDEBUG)
ENDIF()

add_compile_options(-fno-elide-constructors)

set(INC_DIR /usr/local/include)
set(LINK_DIR /usr/local/lib)

include_directories(${INC_DIR})
link_directories(${LINK_DIR})

link_libraries(editline re2)

add_executable(ILisp src/main.cpp)