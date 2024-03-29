#pragma once

#include <string>
#include <vector>

#include "lex.hpp"

using namespace std;
using util::opt;

// Macros

#define NOTHING

#define CALL_EXPR(fn, expr, args) _CALL_EXPR(fn, expr, args, NOTHING)
#define RETURN_EXPR(fn, expr, args) _CALL_EXPR(fn, expr, args, return)

#define _CALL_EXPR(fn, expr, args, ret) \
switch (expr.type) { \
case ::program::expression_type::fn_decl:   ret fn##_expr_fn_decl(expr.fn_decl,   args); break; \
case ::program::expression_type::fn_call:   ret fn##_expr_fn_call(expr.fn_call,   args); break; \
case ::program::expression_type::binary_op: ret fn##_expr_binary_op(expr.binary_op, args); break; \
case ::program::expression_type::literal:   ret fn##_expr_literal(expr.literal,   args); break; \
case ::program::expression_type::name:      ret fn##_expr_name(expr.name,      args); break; \
default: util::crash(expr.loc, "Expr is empty"); \
}

#define CALL_STMT(fn, stmt, args) _CALL_STMT(fn, stmt, args, NOTHING)
#define RETURN_STMT(fn, stmt, args) _CALL_STMT(fn, stmt, args, return)

#define _CALL_STMT(fn, stmt, args, ret) \
switch (stmt.type) { \
case ::program::statement_type::assignment: ret fn##_stmt_assignment(stmt.assignment, args); break; \
case ::program::statement_type::def:        ret fn##_stmt_def(stmt.def, args); break; \
case ::program::statement_type::var:        ret fn##_stmt_var(stmt.var, args); break; \
case ::program::statement_type::expression: ret fn##_stmt_expression(stmt.expression, args); break; \
case ::program::statement_type::if_:        ret fn##_stmt_if(stmt.if_,        args); break; \
case ::program::statement_type::for_:       ret fn##_stmt_for(stmt.for_,       args); break; \
case ::program::statement_type::while_:     ret fn##_stmt_while(stmt.while_,     args); break; \
case ::program::statement_type::return_:    ret fn##_stmt_return(stmt.return_,    args); break; \
default: util::crash(stmt.loc, "Stmt is empty"); \
}

namespace program {

// Forward Declarations

struct expression;
struct statement;
struct fn_decl;

// Misc

struct type {
  view::view name;
};

struct var_decl {
  view::view name;
  opt<type> type;
  opt<util::ref<expression>> value;
};

struct context {
  context *parent = nullptr;

  vector<var_decl> variables;
  vector<fn_decl> functions;
  vector<type> types;

  // function *get_function(view::view name, vector<type>& arg_types, bool recursive = false);
  // variable *get_variable(view::view name, bool recursive = false);
  // variable *get_type(view::view name, bool recursive = false);
};


// Expression Structs

struct fn_decl {
  vector<var_decl> parameters;
  opt<view::view> name;
  opt<type> return_type;

  vector<statement> statements;

  context ctx;
};

struct fn_call {
  view::view name;
  
  vector<expression> arguments;
};

struct binary_op {
  view::view op;
  util::ref<expression> left;
  util::ref<expression> right;
};

enum class literal_type {
  integer, decimal, string, boolean
};

struct literal {
  literal_type type;
  view::view value;
  bool negative = false;
};



enum class expression_type {
  fn_decl, fn_call, binary_op, literal, name
};

struct expression {
  expression_type type;

  fn_decl fn_decl;
  fn_call fn_call;
  binary_op binary_op;
  literal literal;
  view::view name;

  view::location loc;
};



// Statement Structs

struct assignment {
  view::view name;
  expression expr;
};

struct if_stmt {
  expression condition;
  vector<statement> statements;

  vector<if_stmt> else_ifs;
  opt<vector<statement>> else_statements;
};

struct for_stmt {
  expression condition;
  vector<statement> statements;
};

struct while_stmt {
  expression condition;
  vector<statement> statements;
};



enum class statement_type {
  def, var, assignment, if_, for_, while_, return_, expression
};

struct statement {
  statement_type type;

  var_decl def;
  var_decl var;
  assignment assignment;
  if_stmt if_;
  for_stmt for_;
  while_stmt while_;
  expression return_;
  expression expression;

  view::location loc;
};

struct program {
  fn_decl main;
};

}
