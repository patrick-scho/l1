#include "program.h"

#include <algorithm>

#include <fmt/core.h>

#include "util.h"
#include "codegen.h"

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
Function *Context::get_function(string_view name, vector<Type>& argTypes, bool recursive) {
  for (auto& f : functions) {
    if (f->name == name && argTypes.size() == f->arguments.size()) {
      bool equal = true;
      for (int i = 0; i < argTypes.size(); i++) {
        if (argTypes[i].name != f->arguments[i]->variable->type.name) {
          equal = false;
          break;
        }
      }
      if (equal)
        return f.get();
    }
  }
  if (recursive && parent != nullptr)
    return parent->get_function(name, argTypes);
  return nullptr;
}
Variable *Context::get_variable(string_view name, bool recursive) {
  for (auto& v : variables) {
    if (v->name == name)
      return v.get();
  }
  if (recursive && parent != nullptr)
    return parent->get_variable(name);
  return nullptr;
}
void Context::to_c(stringstream& str) {
  for (auto& f: functions) {
    f->to_c(str, *this);
    str << endl;
  }
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
void Function::to_c(stringstream& str, Context& context) {
  str << get_c_type(returnType) << " " << name << appendix << "(";
  bool comma = false;
  for (auto& arg: arguments) {
    if (comma) str << ", "; else comma = true;
    str << get_c_type(arg->variable->type) << " ";
    arg->to_c(str, this->context);
  }
  str << ") {\n";
  for (auto& var: this->context.variables) {
    if (lists::find(arguments,
        [&var](auto& arg) { return arg->variable->name == var->name; }))
      continue;
    str << get_c_type(var->type) << " " << var->name << ";\n";
  }
  for (auto& e: expressions) {
    if (returnType.name != "void" && e == expressions.back())
      str << "return ";
    e->to_c(str, this->context);
    str << ";\n";
  }
  str << "}";
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
void CCall::print() { fmt::print("{}", value); }
void Number::print() { fmt::print("{}", value); }
void String::print() { fmt::print("\"{}\"", value); }

// get_type

Type FunctionCall::get_type(Context &context) {
  if (function == nullptr) {
    vector<Type> argTypes;
    for (auto& e: arguments)
      argTypes.push_back(e->get_type(context));
    function = context.get_function(name, argTypes, true);
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
Type VariableRef::get_type(Context &context) { return variable->get_type(context); }
Type CCall::get_type(Context &context) { return Type{"void"}; }
Type Number::get_type(Context &context) { return Type{"i32"}; }
Type String::get_type(Context &context) { return Type{"string"}; }

// to_c

void FunctionCall::to_c(stringstream& str, Context& context) {
  if (name == "C") {
    String* s = dynamic_cast<String*>(arguments[0].get());
    str << s->value.substr(1, s->value.size() - 2);
    return;
  }
  
  vector<Type> argTypes;
  for (auto& e: arguments)
    argTypes.push_back(e->get_type(context));
  function = context.get_function(name, argTypes, true);

  str << name << function->appendix << "(";
  bool comma = false;
  for (auto& e: arguments) {
    if (comma) str << ", "; else comma = true;
    e->to_c(str, context);
  }
  str << ")";
}
void FunctionRef::to_c(stringstream& str, Context& context) {
  str << "&" << function->name;
}
void Assignment::to_c(stringstream& str, Context& context) {
  str << var->name << " = ";
  expression->to_c(str, context);
}
void VariableRef::to_c(stringstream& str, Context& context) {
  str << variable->name;
}
void CCall::to_c(stringstream& str, Context& context) {
  str << value;
}
void Number::to_c(stringstream& str, Context& context) {
  str << value;
}
void String::to_c(stringstream& str, Context& context) {
  str << '"' << value << '"';
}