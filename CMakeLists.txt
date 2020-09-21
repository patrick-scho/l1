cmake_minimum_required(VERSION 3.0.0)

project(
  l1
  VERSION 0.0.1
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(fmt STATIC ../../libs/fmt/src/os.cc ../../libs/fmt/src/format.cc)
target_include_directories(fmt PUBLIC ../../libs/fmt/include)

if (UNIX)
add_library(tpl STATIC ../../libs/tiny-process-library/process.cpp ../../libs/tiny-process-library/process_unix.cpp)
endif (UNIX)
if (WIN32)
add_library(tpl STATIC ../../libs/tiny-process-library/process.cpp ../../libs/tiny-process-library/process_win.cpp)
endif (WIN32)
target_include_directories(fmt PUBLIC ../../libs/tiny-process-library)

add_executable(l1
  src/main.cpp
  src/parse.cpp
  src/program.cpp
  src/program_print.cpp
  src/program_get_type.cpp
  src/program_to_c.cpp
  src/program_to_meta.cpp
  src/source.cpp
  src/util.cpp
  src/codegen.cpp)

target_include_directories(l1 PRIVATE ../../libs/tcc/libtcc)
target_link_directories(l1 PRIVATE ../../libs/tcc/libtcc)

target_link_libraries(l1 fmt tpl libtcc)
if (UNIX)
target_link_libraries(l1 pthread)
endif (UNIX)

add_custom_command(TARGET l1 POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_CURRENT_SOURCE_DIR}/test2.l
  ${CMAKE_BINARY_DIR})
