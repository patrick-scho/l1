#include "codegen.h"

#include <libtcc.h>
#include <fmt/core.h>

#include "program.h"

using ptr = int (*)(int);

char my_program[] =
    R"(
  int fib(int n) {
    if (n <= 2) return 1;
    return fib(n-1)+fib(n-2);
  }
)";

string to_c(Function f) {
  return "";
}

void test() {
  TCCState *s = tcc_new();
  if (!s) return;

  tcc_set_output_type(s, TCC_OUTPUT_MEMORY);

  if (tcc_compile_string(s, my_program) == -1)
    return;

  if (tcc_relocate(s, TCC_RELOCATE_AUTO) < 0)
    return;

  ptr func = (ptr)tcc_get_symbol(s, "fib");
  if (!func)
    return;

  fmt::print("{}\n", func(4));

  tcc_delete(s);
}
