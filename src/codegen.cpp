#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "libtcc.h"

using ptr = int(*)(int);

char my_program[] =
R"(
  int fib(int n) {
    if (n <= 2) return 1;
    return fib(n-1)+fib(n-2);
  }
)";

int maiin(int argc, char **argv)
{
    TCCState *s;
    int i;
    ptr func;

    s = tcc_new();
    if (!s) {
        fprintf(stderr, "Could not create tcc state\n");
        exit(1);
    }

    tcc_add_library_path(s, "lib");

    /* MUST BE CALLED before any compilation */
    tcc_set_output_type(s, TCC_OUTPUT_MEMORY);

    if (tcc_compile_string(s, my_program) == -1)
        return 1;

    /* relocate the code */
    if (tcc_relocate(s, TCC_RELOCATE_AUTO) < 0)
        return 1;

    /* get entry symbol */
    func = (ptr)tcc_get_symbol(s, "fib");
    if (!func)
        return 1;

    /* run the code */
    printf("%d\n", func(4));

    /* delete the state */
    tcc_delete(s);

    return 0;
}
