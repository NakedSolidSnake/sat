cmake_minimum_required (VERSION 3.16)
project (SAT)
set (CMAKE_C_FLAGS "-D_GNU_SOURCE -Wall -Werror -std=c99")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set (LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_BUILD_TYPE Debug)

include (GNUInstallDirs)

enable_testing ()

include ("${CMAKE_CURRENT_SOURCE_DIR}/cmake/CMakeLoadPKG.txt")
include ("${CMAKE_CURRENT_SOURCE_DIR}/cmake/CMakeFunctions.txt")
include ("${CMAKE_CURRENT_SOURCE_DIR}/cmake/LoadModules.txt")

list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

file (WRITE ${CMAKE_CURRENT_LIST_DIR}/modules/main/include/sat_optionals.h "")

add_subdirectory (modules)