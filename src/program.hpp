#pragma once

#include <string>
#include <vector>

#include "lex.hpp"

using namespace std;

namespace program {

struct function;
struct context;

struct expression {
  view::location loc;
};

struct statement {
  view::location loc;
};

struct type {
  view::view name;
  vector<type> sub_types;
};

struct variable {
  view::view name;
  type type;

  expression * definition;
};

struct variable_ref {
  view::view name;
};

struct function_ref {
  function * function;
};

struct function {
  view::view name;
  string appendix;
  type return_type;
  vector<variable_ref> arguments;
  vector<statement> statements;

  context * ctx;
};

struct function_call {
  view::view name;
  function * function = nullptr;
  vector<expression> arguments;
};

struct assignment {
  variable_ref * var;
  expression expression;
};

struct c_call {
  view::view value;
};

struct number {
  long value;
};

struct string {
  view::view value;
};

struct context {
  context *parent = nullptr;

  vector<variable> variables;
  vector<function> functions;
  vector<type> types;

  function *get_function(view::view name, vector<type>& arg_types, bool recursive = false);
  variable *get_variable(view::view name, bool recursive = false);
  variable *get_type(view::view name, bool recursive = false);
};

}
