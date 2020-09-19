#include "parse.h"
#include "program.h"
#include "util.h"
#include "codegen.h"

#include <sstream>

#include <fmt/core.h>

list<unique_ptr<Function>> getDefaultFunctions() {
  list<unique_ptr<Function>> result;
  auto add = make_unique<Function>();
  add->name = "add";
  add->returnType = Type { "i32" };
  result.push_back(move(add));
  return result;
}

int main(int argc, char** argv) {
  string filename = "test1";
  string file = read_file(filename);

  Source source { file };
  source.location.filename = filename;
  auto fns = getDefaultFunctions();
  auto main = parse_file(source, fns);
  
  if (main == nullptr)
    return 1;

  main->name = "main";
  main->returnType = Type {"void"};

  main->print();

  // stringstream str;
  // func_to_c(main, str);
  // fmt::print("C:\n{}\n", str.str());

  return 0;
}