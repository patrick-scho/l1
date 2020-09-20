#include "codegen.h"

#include <sstream>
#include <fstream>
#include <filesystem>

#include <fmt/core.h>
#include <process.hpp>

#include "program.h"

using namespace std;
using namespace TinyProcessLib;

// using MainFn = void(*)();

// TCCState *s = tcc_new();
// if (!s)
//   return;

// tcc_add_include_path(s, "../../libs/tcc/include");

// tcc_set_output_type(s, TCC_OUTPUT_MEMORY);

// if (tcc_compile_string(s, src.c_str()) == -1)
//   return;

// if (tcc_relocate(s, TCC_RELOCATE_AUTO) < 0)
//   return;

// MainFn func = (MainFn)tcc_get_symbol(s, "main");
// if (!func)
//   return;

// MainFn();

// tcc_delete(s);

void compile(const string& filename, const string& src) {
  filesystem::create_directories("tmp");
  string c_file = "tmp/" + filename + ".c";
  write_file(c_file, src);
#ifdef _WIN32
  string cmd = "cl " + c_file;
  Process p("cmd /C " + cmd, "", [](const char *bytes, size_t n) {
#else
  string cmd = "gcc " + c_file + " -o " + filename;
  Process p(cmd, "", [](const char *bytes, size_t n) {
#endif
  });
  if (p.get_exit_status() != 0)
    fmt::print("Error compiling C source");
}

void run(const string& filename) {
  string exe = filename;
#ifdef _WIN32
  Process p("cmd /C" + exe + ".exe", "", [](const char *bytes, size_t n) {
#else
  Process p("./" + exe, "", [](const char *bytes, size_t n) {
#endif
    fmt::print("{}", string(bytes, n));
  });
  if (p.get_exit_status() != 0)
    fmt::print("Error running {}", filename);
}

string get_c_type(Type type) {
  if (type.name == "i32") return "int";
  if (type.name == "i64") return "long long";
  if (type.name == "string") return "const char * ";
  return "void";
}
