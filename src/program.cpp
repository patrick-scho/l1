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
Function *Context::get_function(string_view name, vector<Type>& argTypes) {
  for (auto& f : functions) {
    if (f->name == name)
      return f.get();
  }
  if (parent != nullptr)
    return parent->get_function(name, argTypes);
  return nullptr;
}
Variable *Context::get_variable(string_view name) {
  for (auto& v : variables) {
    if (v->name == name)
      return v.get();
  }
  if (parent != nullptr)
    return parent->get_variable(name);
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
Type Function::get_type(Context &context) {
  static int stackDepth = 0;
  if (stackDepth > 10)
    throw ERROR(definition->location, "Return Type Inference too deeply nested!");
  if (returnType.name.empty()) {
    stackDepth++;
    if (expressions.empty())
      throw ERROR(definition->location, "Return Type cannot be inferred");
    return expressions.back()->get_type(context);
  } else {
    stackDepth = 0;
    return returnType;
  }
}

// Variable

void Variable::print() {
  fmt::print("{}({})", name, type.name);
}
Type Variable::get_type(Context &context) {
  if (type.name.empty()) {
    type = definition->get_type(context);
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

// get_type

Type FunctionCall::get_type(Context &context) {
  if (function == nullptr) {
    vector<Type> argTypes;
    for (auto& e: arguments)
      argTypes.push_back(e->get_type(context));
    function = context.get_function(name, argTypes);
  }
  if (function == nullptr) {
    throw ERROR(location, "Calling undefined Function {}", name);
  }
  return function->get_type(context);
}
Type FunctionRef::get_type(Context &context) {
  return function->get_type(context);
}
Type Assignment::get_type(Context &context) {
  return expression->get_type(context);
}
Type VariableRef::get_type(Context &context) { return variable->type; }
Type Number::get_type(Context &context) { return Type{"i32"}; }
Type String::get_type(Context &context) { return Type{"string"}; }