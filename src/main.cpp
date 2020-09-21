#include "codegen.h"
#include "parse.h"
#include "program.h"
#include "util.h"

#include <sstream>

#include <fmt/core.h>

/*
Roadmap:
- when types?
- TCC
- Meta
- c_type/c_name
- Syntax
- Operators
- if/for
- include
- import/export/using
- Standard Types
- Types
- ZPL
- Git Submodules
- Arbitrary Precision Integers
- Conversion
*/

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

  // stringstream str;
  // main->print(str);
  // fmt::print(str.str());

  stringstream str;
  str << "#include <stdio.h>" << endl;
  
  main->context.to_c(str);
  main->to_c(str, main->context);

  fmt::print("C:\n{}\n", str.str());

  test(R"(
#include <stdio.h>
void main() {
  puts("henlo");
}
  )");
  test(str.str());

  // compile(filename, str.str());

  // run(filename);

  return 0;
}
