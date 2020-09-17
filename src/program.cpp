#include "program.h"

#include <fmt/core.h>

void FunctionCall::print() {
  fmt::print("{}(", function);
  for (auto& e: arguments) {
    e->print();
    fmt::print(" ");
  }
  fmt::print(")");
}
void Function::print() {
  fmt::print("fn {}(", name);
  for (auto& v: arguments) {
    v->print();
    fmt::print(" ");
  }
  fmt::print(") (\n");
  for (auto& e: expressions) {
    e->print();
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