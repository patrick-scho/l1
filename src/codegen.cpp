#include "codegen.h"

#include <sstream>

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

string get_type(Function& f) {
  return "int";
}

void func_call_to_c(FunctionCall& f, stringstream& str) {
  str << f.function << "(";
  bool comma = false;
  for (Expression& e: f.arguments) {
    if (comma) str << ", ";
    else comma = true;
    expr_to_c(e, str);
  }
  str << ")";
}

void expr_to_c(Expression& e, stringstream& str) {
  switch (e.type) {
  case ExpressionType::FnCall:
    func_call_to_c(e.functionCall, str);
    break;
  case ExpressionType::FnDecl:
    func_to_c(e.functionDeclaration, str);
    break;
  case ExpressionType::Number:
    str << e.number;
    break;
  case ExpressionType::String:
    str << e.string;
    break;
  case ExpressionType::Var:
    str << e.variable;
    break;
  }
}

void func_to_c(Function& f, stringstream& str) {
  str << get_type(f) << " " << f.name << "(";
  bool comma = false;
  for (Variable& var: f.arguments) {
    if (comma) str << ", ";
    else comma = true;
    str << var.type << " " << var.name;
  }
  str << ") {\n";
  int i = 0; int len = f.expressions.size();
  for (Expression& e: f.expressions) {
    if (++i == len)
      str << "return ";
    expr_to_c(e, str);
    str << ";\n";
  }
  str << "}";
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
