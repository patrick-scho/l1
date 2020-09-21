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
    if (dynamic_cast<FunctionRef*>(e.get()) != NULL || dynamic_cast<MetaFunctionRef*>(e.get()) != NULL)
      continue;
    e->to_c(str, this->context);
    str << ";\n";
  }
  str << "}";
}

void MetaFunction::to_c(stringstream& str, Context& context) {
  str << "void " << name << "(";
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
  for (auto& e: expressions)  {
    if (e->is_meta) {
      e->to_c(str, this->context);
      str << ";\n";
    } else {
      stringstream tmp;
      e->to_c(tmp, this->context);
      string s = tmp.str();
      string::size_type n = 0;
      while ((n = s.find("\\", n+2)) != string::npos)
        s = s.replace(n, 1, "\\\\");
      while ((n = s.find("\"", n+2)) != string::npos)
        s = s.replace(n, 1, "\\\"");
      str << "printf(\"" << s << "\\n\");\n";
    }
  }
  str << "}";
}


void FunctionCall::to_c(stringstream& str, Context& context) {
  vector<Type> argTypes;
  for (auto& e: arguments)
    argTypes.push_back(e->get_type(context));
  function = context.get_function(name, argTypes, true);
  if (function == nullptr) {
    MetaFunction *mf = context.get_meta_function(name, argTypes, true);
    if (mf == nullptr)
      throw ERROR(location, "Call to unknown Function {}", name);
    mf->to_c(str, context);
    return;
  }

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
void MetaFunctionRef::to_c(stringstream& str, Context& context) {
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