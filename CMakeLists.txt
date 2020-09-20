cmake_minimum_required(VERSION 3.0.0)

project(
  l1
  VERSION 0.0.1
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(fmt STATIC ../../libs/fmt/src/os.cc ../../libs/fmt/src/format.cc)
target_include_directories(fmt PUBLIC ../../libs/fmt/include)

add_library(tpl STATIC ../../libs/tiny-process-library/process.cpp ../../libs/tiny-process-library/process_win.cpp)
target_include_directories(fmt PUBLIC ../../libs/tiny-process-library)

add_executable(l1
  src/main.cpp
  src/parse.cpp
  src/program.cpp
  src/source.cpp
  src/util.cpp
  src/codegen.cpp)

#target_include_directories(l1 PRIVATE ../../libs/tcc/libtcc)
#target_link_directories(l1 PRIVATE ../../libs/tcc/libtcc)

target_link_libraries(l1 fmt tpl)

add_custom_command(TARGET l1 POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_CURRENT_SOURCE_DIR}/test1
  ${CMAKE_BINARY_DIR})