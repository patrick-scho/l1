#include "program.h"

#include <fmt/core.h>

#include "util.h"

void Context::print() {
  if (parent != nullptr)
    parent->print();
  fmt::print("[ ");
  for (auto& f: functions) {
    fmt::print("{}(fn)", f->name);
    fmt::print(" ");
  }
  for (auto& v: variables) {
    fmt::print("{}({})", v->name, v->type.name);
    fmt::print(" ");
  }
  fmt::print("]\n");
}
Function* Context::getFunction(Expression* expr, string_view name) {
  for (auto f: functions) {
    if (f->name == name)
      return f;
  }
  if (parent != nullptr)
    return parent->getFunction(expr, name);
  throw ERROR(expr, "Undefined Function {}", name);
}
Variable* Context::getVariable(Expression* expr, string_view name) {
  for (auto v: variables) {
    if (v->name == name)
      return v;
  }
  if (parent != nullptr)
    return parent->getVariable(expr, name);
  throw ERROR(expr, "Undefined Variable {}", name);
}

void FunctionCall::print() {
  fmt::print("{}( ", function);
  for (auto& e: arguments) {
    e->print();
    fmt::print(" ");
  }
  fmt::print(")");
}
void Function::print() {
  fmt::print("fn {}( ", name);
  for (auto& v: arguments) {
    v->print();
    fmt::print(" ");
  }
  fmt::print(") -> {} (\n", returnType.name);
  context.print();
  for (auto& e: expressions) {
    e->print();
    fmt::print("\n");
  }
  fmt::print(")\n");
}
void Assignment::print() {
  var->print();
  fmt::print(" = ");
  expression->print();
}
void Variable::print() {
  fmt::print("{}({})", name, type.name);
}
void Number::print() {
  fmt::print("{}", value);
}
void String::print() {
  fmt::print("{}", value);
}

Type FunctionCall::getType(Context& context) {
  return context.getFunction(this, function)->getType(context);
}
Type Function::getType(Context& context) {
  return returnType;
}
Type Assignment::getType(Context& context) {
  return expression->getType(context);
}
Type Variable::getType(Context& context) {
  return type;
}
Type Number::getType(Context& context) {
  return Type { "i32" };
}
Type String::getType(Context& context) {
  return Type { "string" };
}