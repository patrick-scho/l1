#include "program.h"

#include <fmt/core.h>

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
void Assignment::print() {
  fmt::print("{} = ", var);
  expression->print();
}
void Variable::print() {
  fmt::print("{}({})", name, type);
}
void Number::print() {
  fmt::print("{}", value);
}
void String::print() {
  fmt::print("{}", value);
}