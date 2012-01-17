cmake_minimum_required (VERSION 2.6)
project (ESharp)
# The version number.
set (ESHARP_VERSION_MAJOR 0)
set (ESHARP_VERSION_MINOR 1)
set (ESHARP_VERSION_REV 5)
 
# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/version.h.in"
  "${PROJECT_BINARY_DIR}/version.h"
  )
 
# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}/")
include_directories ("${PROJECT_BINARY_DIR}/antlr/")
include_directories ("${PROJECT_SOURCE_DIR}/include/")
include_directories ("${PROJECT_SOURCE_DIR}/antlr/include/")

add_subdirectory (antlr)

# generate the grammar
execute_process(COMMAND echo Ready)
execute_process(COMMAND java -jar ${PROJECT_SOURCE_DIR}/antlr/lib/antlr-3.4-complete.jar ${PROJECT_SOURCE_DIR}/grammar/esharp.g -o . WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
 
# add the executable
add_executable(ESharp
 src/bc_binary_expr.cpp
 src/bc_call.cpp
 src/bc_chunk.cpp
 src/bc_classdef.cpp
 src/bc_code.cpp
 src/bc_expr.cpp
 src/bc_id.cpp
 src/bc_literals.cpp
 src/bc_script.cpp
 src/bc_sourceinfo.cpp
 src/bc_statements.cpp
 src/bc_unary_expr.cpp
 src/bc_utils.cpp
 src/boolean.cpp
 src/console.cpp
 src/data.cpp
 src/dict.cpp
 src/exception.cpp
 src/fs.cpp
 src/gc.cpp
 src/integer.cpp
 src/list.cpp
 src/method.cpp
 src/object.cpp
 src/object_nogc.cpp
 src/octet.cpp
 src/real.cpp
 src/runtime.cpp
 src/rwfilestream.cpp
 src/rwstream.cpp
 src/scope.cpp
 src/str.cpp
 src/thrown_exception.cpp
 ${PROJECT_BINARY_DIR}/esharpLexer.c
 ${PROJECT_BINARY_DIR}/esharpParser.c
 src/main.cpp
)
target_link_libraries (ESharp antlr)
