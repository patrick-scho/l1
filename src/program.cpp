#include "program.h"

#include <fmt/core.h>

void Variable::print() {
  fmt::print("{}({})", name, type);
}
void FunctionCall::print() {
  fmt::print("{}(", function);
  for (Expression& e: arguments) {
    e.print();
    fmt::print(" ");
  }
  fmt::print(")");
}
void Function::print() {
  fmt::print("fn {}(", name);
  for (Variable& v: arguments) {
    v.print();
    fmt::print(" ");
  }
  fmt::print(") (\n");
  for (Expression& e: expressions) {
    e.print();
    fmt::print("\n");
  }
  fmt::print(")\n");
}
void Expression::print() {
  switch (type) {
  case ExpressionType::FnCall:
    functionCall.print(); break;
  case ExpressionType::FnDecl:
    functionDeclaration.print(); break;
  case ExpressionType::Number:
    fmt::print("{}", number); break;
  case ExpressionType::String:
    fmt::print(string); break;
  case ExpressionType::Var:
    fmt::print(variable); break;
  }
}