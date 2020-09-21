#include "program.h"

#include <fmt/core.h>

void result_add_expr(stringstream& str, Expression* e) {
  str << "add(result ";
  e->print(str);
  str << ")\n";
}

template <typename Arg, typename... Args>
void result_add_str(stringstream& str, Arg&& arg, Args&&... args) {
  str << "add(result \"";
  
  str << std::forward<Arg>(arg);
  ((str << std::forward<Args>(args)), ...);

  str << "\")\n";
}

void FunctionCall::to_meta(stringstream &str) {
  if (is_meta) {
    // if (function->returnType.name == "void")
    //   print(str);
    // else
      result_add_expr(str, this);
  }
  else {
    result_add_str(str, name, "(");
    for (auto &e : arguments) {
      e->to_meta(str);
    }
    result_add_str(str, ")");
  }
}
void FunctionRef::to_meta(stringstream &str) {
  if (is_meta)
    result_add_expr(str, this);
  else
    result_add_str(str, this->function->name);
}
void MetaFunctionRef::to_meta(stringstream &str) {
}
void Assignment::to_meta(stringstream &str) {
  if (is_meta)
    result_add_expr(str, this);
  else {
    result_add_str(str, var->name, " = ");
    expression->to_meta(str);
  }
}
void VariableRef::to_meta(stringstream &str) {
  if (is_meta)
    result_add_expr(str, this);
  else
    result_add_str(str, variable->name);
}
void CCall::to_meta(stringstream &str) {
  if (is_meta)
    result_add_expr(str, this);
  else
    result_add_str(str, "C (( ", value, " ))");
}
void Number::to_meta(stringstream &str) {
  if (is_meta)
    result_add_expr(str, this);
  else
    result_add_str(str, value);
}
void String::to_meta(stringstream &str) {
  if (is_meta)
    result_add_expr(str, this);
  else {
    string s(value);
    string::size_type n = 0;
    while ((n = s.find("\\", n+2)) != string::npos)
      s = s.replace(n, 1, "\\\\");
    while ((n = s.find("\"", n+2)) != string::npos)
      s = s.replace(n, 1, "\\\"");
      result_add_str(str, "\\\"", s, "\\\"");
  }
}