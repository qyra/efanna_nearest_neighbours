cmake_minimum_required (VERSION 2.6)
project (Efanna)

include(CheckSSEFeatures.cmake)

set (Efanna_VERSION_MAJOR 0)
set (Efanna_VERSION_MINOR 1)

set (CMAKE_CXX_STANDARD 14)

configure_file (
  "${PROJECT_SOURCE_DIR}/include/efanna_config.hpp.in"
  "${PROJECT_BINARY_DIR}/efanna_config.hpp"
)

include_directories("${PROJECT_SOURCE_DIR}/include")
# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")

# If in debug mode, use address sanitizer.

set(COMMON_FLAGS "-Wall -msse2 -msse3 -msse4 -mavx2")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${COMMON_FLAGS} -g -Wall -O1 -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer")

# Don't use address san with kcachegrind.
#~ set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${COMMON_FLAGS} -g -Wall")

# If in release enable higher optimization level
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${COMMON_FLAGS} -Wall -O3")


# Modify this to add new source files. Their .h files should be in the include folder.
add_executable(efanna src/main.cpp src/kdtree.cpp src/heap.cpp src/euclidean_distance.cpp)

set (EXTRA_LIBS ${EXTRA_LIBS} config++)
target_link_libraries (efanna  ${EXTRA_LIBS})
