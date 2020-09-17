#include "parse.h"
#include "program.h"
#include "util.h"
#include "codegen.h"

#include <sstream>

#include <fmt/core.h>

int main(int argc, char** argv) {
  string file = read_file("test1");

  Source source { file };
  auto main = parse_file(source);
  main->name = "main";

  main->print();

  // stringstream str;
  // func_to_c(main, str);
  // fmt::print("C:\n{}\n", str.str());

  return 0;
}