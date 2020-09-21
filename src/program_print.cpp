#include "program.h"

#include <fmt/core.h>

void Context::print(stringstream& str) {
  str << "[ ";
  for (auto &v : variables) {
    str << v->name << "(" << v->type.name << ") ";
  }
  str << "]\n";
  for (auto &f : functions) {
    f->print(str);
  }
}

void Function::print(stringstream& str) {
  str << "func " << name << "( ";
  for (auto &v : arguments) {
    v->print(str);
    str << " ";
  }
  //str << ") -> " << returnType.name << " (\n";
  str << "): " << returnType.name << " (\n";
  //context.print(str);
  for (auto &e : expressions) {
    e->print(str);
    str << endl;
  }
  str << ")\n";
}

void MetaFunction::print(stringstream& str) {
  str << "func " << name << "( ";
  for (auto &v : arguments) {
    v->print(str);
    str << " ";
  }
  str << "): void (\n";
  //context.print(str);
  str << "result = \"\"\n";
  for (auto &e : expressions) {
    e->to_meta(str);
  }
  str << "result\n";
  str << ")\n";
}

void Variable::print(stringstream& str) {
  str << name << "(" << type.name << ")";
}


void FunctionCall::print(stringstream& str) {
  str << name << "(";
  for (auto &e : arguments) {
    e->print(str);
    str << " ";
  }
  str << ")";
}
void FunctionRef::print(stringstream& str) {
  //str << function->name;
}
void MetaFunctionRef::print(stringstream& str) {
  meta_function->print(str);
}
void Assignment::print(stringstream& str) {
  var->print(str);
  str << " = ";
  expression->print(str);
}
void VariableRef::print(stringstream& str) {
  str << name;
}
void CCall::print(stringstream& str) { str << value; }
void Number::print(stringstream& str) { str << value; }
void String::print(stringstream& str) { str << '"' << value << '"'; }