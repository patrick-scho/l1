#include "parse.h"
#include "program.h"
#include "util.h"
#include "codegen.h"

#include <fmt/core.h>

int main(int argc, char** argv) {
  string file = read_file("test1");

  Source source { file };
  Function main = parse_file(source);

  main.print();

  fmt::print(to_c(main));

  return 0;
}