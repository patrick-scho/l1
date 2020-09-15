#include "parse.h"
#include "source.h"
#include "util.h"
#include "array.h"
#include "program.h"

int main(int argc, char** argv) {
  Source source = read_file("test1");

  Function* main = parse_file(&source);

  Function_print(main);

  Function_free(main);

  free(source.data);

  return 0;
}