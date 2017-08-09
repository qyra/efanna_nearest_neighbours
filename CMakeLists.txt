cmake_minimum_required (VERSION 2.6)
project (Efanna)

set (Efanna_VERSION_MAJOR 0)
set (Efanna_VERSION_MINOR 1)

set (CMAKE_CXX_STANDARD 14)

configure_file (
  "${PROJECT_SOURCE_DIR}/include/efannaConfig.h.in"
  "${PROJECT_BINARY_DIR}/efannaConfig.h"
)

include_directories("${PROJECT_SOURCE_DIR}/include")
# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")

# If in debug mode, use address sanitizer.
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -O1 -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer")

# Modify this to add new source files. Their .h files should be in the include folder.
add_executable(efanna src/main.cpp)

set (EXTRA_LIBS ${EXTRA_LIBS} config++)
target_link_libraries (efanna  ${EXTRA_LIBS})
