#include "program.h"

#include <fmt/core.h>

#include "util.h"

// Context

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
Function *Context::getFunction(string_view name, vector<Type>& argTypes) {
  for (auto& f : functions) {
    if (f->name == name)
      return f.get();
  }
  if (parent != nullptr)
    return parent->getFunction(name, argTypes);
  return nullptr;
}
Variable *Context::getVariable(string_view name) {
  for (auto& v : variables) {
    if (v->name == name)
      return v.get();
  }
  if (parent != nullptr)
    return parent->getVariable(name);
  return nullptr;
}

// Function

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
Type Function::getType(Context &context) {
  static int stackDepth = 0;
  if (stackDepth > 10)
    throw ERROR(definition->location, "Return Type Inference too deeply nested!");
  if (returnType.name.empty()) {
    stackDepth++;
    if (expressions.empty())
      throw ERROR(definition->location, "Return Type cannot be inferred");
    return expressions.back()->getType(context);
  } else {
    stackDepth = 0;
    return returnType;
  }
}

// Variable

void Variable::print() {
  fmt::print("{}({})", name, type.name);
}
Type Variable::getType(Context &context) {
  if (type.name.empty()) {
    type = definition->getType(context);
  }
  return type;
}

// Expressions

// print

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
void Number::print() { fmt::print("{}", value); }
void String::print() { fmt::print("{}", value); }

// getType

Type FunctionCall::getType(Context &context) {
  if (function == nullptr) {
    vector<Type> argTypes;
    for (auto& e: arguments)
      argTypes.push_back(e->getType(context));
    function = context.getFunction(name, argTypes);
  }
  if (function == nullptr) {
    throw ERROR(location, "Calling undefined Function {}", name);
  }
  return function->getType(context);
}
Type FunctionRef::getType(Context &context) {
  return function->getType(context);
}
Type Assignment::getType(Context &context) {
  return expression->getType(context);
}
Type VariableRef::getType(Context &context) { return variable->type; }
Type Number::getType(Context &context) { return Type{"i32"}; }
Type String::getType(Context &context) { return Type{"string"}; }