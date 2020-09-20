#include "program.h"

#include <sstream>

#include "codegen.h"

void Context::to_c(stringstream& str) {
  for (auto& f: functions) {
    f->to_c(str, *this);
    str << endl;
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
    // if (dynamic_cast<FunctionRef*>(e.get()) != NULL)
    //   continue;
    e->to_c(str, this->context);
    str << ";\n";
  }
  str << "}";
}


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
  //str << "&" << function->name;
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