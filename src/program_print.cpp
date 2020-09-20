#include "program.h"

#include <fmt/core.h>

void Context::print() {
  fmt::print("[ ");
  for (auto &v : variables) {
    fmt::print("{}({})", v->name, v->type.name);
    fmt::print(" ");
  }
  fmt::print("]\n");
  for (auto &f : functions) {
    f->print();
  }
}

void Function::print() {
  fmt::print("fn {}( ", name);
  for (auto &v : arguments) {
    v->print();
    fmt::print(" ");
  }
  fmt::print(") -> {} (\n", returnType.name);
  context.print();
  for (auto &e : expressions) {
    e->print();
    fmt::print("\n");
  }
  fmt::print(")\n");
}

void Variable::print() {
  fmt::print("{}({})", name, type.name);
}


void FunctionCall::print() {
  fmt::print("{}( ", name);
  for (auto &e : arguments) {
    e->print();
    fmt::print(" ");
  }
  fmt::print(")");
}
void FunctionRef::print() {
  fmt::print("{}", function->name);
}
void Assignment::print() {
  var->print();
  fmt::print(" = ");
  expression->print();
}
void VariableRef::print() {
  fmt::print("{}", variable->name);
}
void CCall::print() { fmt::print("{}", value); }
void Number::print() { fmt::print("{}", value); }
void String::print() { fmt::print("\"{}\"", value); }