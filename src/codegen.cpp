#include "codegen.h"

#include <sstream>
#include <fstream>

#include <fmt/core.h>
#include <libtcc.h>

#include "program.h"

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

using MainFn = void(*)();

void write_to_file(string filename, const string& file) {
  ofstream ofs(filename, ios::binary | ios::out);
  ofs.write(file.c_str(), file.size());
  ofs.close();
}

void compile(const string& filename, const string& src) {
  string c_file = "tmp/" + filename + ".c";
  write_to_file(c_file, src);
  string cmd = "cl " + c_file;
  system(cmd.c_str());
}

void run(const string& filename) {
  string exe= ".\\" + filename;
  system(exe.c_str());
}

string get_c_type(Type type) {
  if (type.name == "i32") return "int";
  if (type.name == "i64") return "long";
  if (type.name == "string") return "const char * ";
  return "void";
}