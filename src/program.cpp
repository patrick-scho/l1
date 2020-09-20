#include "program.h"

#include <fmt/core.h>

#include "util.h"
#include "codegen.h"


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
