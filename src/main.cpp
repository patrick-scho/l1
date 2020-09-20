#include "codegen.h"
#include "parse.h"
#include "program.h"
#include "util.h"

#include <sstream>

#include <fmt/core.h>


int main(int argc, char **argv) {
  string filename = "test1.l";
  string file = read_file(filename);

  Source source{file};
  source.location.filename = filename;
  auto main = parse_file(source);

  if (main == nullptr)
    return 1;

  main->name = "main";
  main->returnType = Type{"void"};

  //main->print();

  stringstream str;
  str << "#include <stdio.h>" << endl;
  
  main->context.to_c(str);
  main->to_c(str, main->context);

  //fmt::print("C:\n{}\n", str.str());

  compile("test1", str.str());

  run("test1");

  return 0;
}
