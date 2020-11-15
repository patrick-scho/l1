#pragma once

#include <sstream>

#include "program.hpp"
#include "parse.hpp"

using namespace std;

namespace codegen {

// Forward Declarations
void to_c_expr(program::expression expr, stringstream& sstr);
void to_c_stmt(program::statement stmt, stringstream& sstr);
void to_c_expr_fn_decl(program::fn_decl fn, stringstream& sstr);

void to_c_var_decl(program::var_decl var, stringstream& sstr) {
  if (var.type.has_value())
    sstr << var.type.value().to_str() << " ";
  else
    sstr << "TODO ";

  sstr << var.name.to_str();

  if (var.value.has_value()) {
    sstr << " = ";
    to_c_expr(var.value.value(), sstr);
  }
}

void to_c_stmt_assignment(program::assignment asgn, stringstream& sstr) {
  sstr << asgn.name.to_str() << " = ";
  to_c_expr(asgn.expr, sstr);
  sstr << "\n";
}

void to_c_stmt_def(vector<program::var_decl> def, stringstream &sstr) {
  for (int i = 0; i < def.size(); i++) {
    if (def[i].value.has_value() &&
       ((program::expression)def[i].value.value()).type == program::expression_type::fn_decl) {
      to_c_expr_fn_decl(((program::expression)def[i].value.value()).fn_decl, sstr);
    } else {
      sstr << "const ";
      to_c_var_decl(def[i], sstr);
      sstr << ";\n";
    }
  }
}

void to_c_stmt_var(vector<program::var_decl> var, stringstream& sstr) {
  for (int i = 0; i < var.size(); i++) {
    to_c_var_decl(var[i], sstr);
    sstr << ";\n";
  }
}

void to_c_stmt_expression(program::expression expr, stringstream& sstr) {
  to_c_expr(expr, sstr);
  sstr << ";\n";
}

void to_c_stmt_if(program::if_stmt if_, stringstream& sstr) {
  sstr << "if (";
  to_c_expr(if_.condition, sstr);
  sstr << ") {\n";

  for (int i = 0; i < if_.statements.size(); i++) {
    to_c_stmt(if_.statements[i], sstr);
  }
  sstr << "}";

  for (int i = 0; i < if_.else_ifs.size(); i++) {
    sstr << "\n";
    sstr << "else if (";
    to_c_expr(if_.else_ifs[i].condition, sstr);
    sstr << ") {\n";

    for (int i = 0; i < if_.else_ifs[i].statements.size(); i++) {
      to_c_stmt(if_.else_ifs[i].statements[i], sstr);
    }
    sstr << "}";
  }

  if (if_.else_statements.has_value()) {
    sstr << "\n";
    sstr << "else {\n";
    for (int i = 0; i < if_.else_statements.value().size(); i++) {
      to_c_stmt(if_.else_statements.value()[i], sstr);
    }
    sstr << "}";
  }

  sstr << "\n";
}

void to_c_stmt_for(program::for_stmt for_, stringstream& sstr) {
  sstr << "for (";
  to_c_expr(for_.condition, sstr);
  sstr << ") {\n";

  for (int i = 0; i < for_.statements.size(); i++) {
    to_c_stmt(for_.statements[i], sstr);
  }
  sstr << "}";
}

void to_c_stmt_while(program::while_stmt while_, stringstream& sstr) {
  sstr << "while (";
  to_c_expr(while_.condition, sstr);
  sstr << ") {\n";

  for (int i = 0; i < while_.statements.size(); i++) {
    to_c_stmt(while_.statements[i], sstr);
  }
  sstr << "}";
}

void to_c_stmt_return(program::expression return_, stringstream& sstr) {
  sstr << "return ";
  to_c_expr(return_, sstr);
  sstr << ";\n";
}

void to_c_stmt(program::statement stmt, stringstream& sstr) {
  CALL_STMT(to_c, stmt, sstr)
}

void to_c_expr_fn_decl(program::fn_decl fn, stringstream& sstr) {
  if (fn.return_type.has_value())
    sstr << fn.return_type.value().to_str() << " ";
  else
    sstr << "void ";
  if (fn.name.has_value())
    sstr << fn.name.value().to_str();
  else
    sstr << "TODO";
  sstr << " (";
  for (int i = 0; i < fn.parameters.size(); i++) {
    if (i != 0) sstr << ", ";

    program::var_decl var = fn.parameters[i];
    if (var.type.has_value())
      sstr << var.type.value().to_str() << " ";
    else
      sstr << "TODO ";

    sstr << var.name.to_str();
  } 
  sstr << ")";
  sstr << " {\n";
  for (int i = 0; i < fn.parameters.size(); i++) {
    program::var_decl var = fn.parameters[i];
    if (var.value.has_value()) {
      sstr << var.name.to_str() << " = ";
      to_c_expr(var.value.value(), sstr);
      sstr << ";\n";
    }
  } 
  for (int i = 0; i < fn.statements.size(); i++) {
    to_c_stmt(fn.statements[i], sstr);
  }
  sstr << "}\n";
}

void to_c_expr_fn_call(program::fn_call fn, stringstream& sstr) {
  sstr << fn.name.to_str() << "(";
  for (int i = 0; i < fn.arguments.size(); i++) {
    if (i != 0) sstr << ", ";
    to_c_expr(fn.arguments[i], sstr);
  }
  sstr << ")";
}

void to_c_expr_binary_op(program::binary_op op, stringstream& sstr) {
  to_c_expr(op.left, sstr);
  sstr << " " << op.op.to_str() << " ";
  to_c_expr(op.right, sstr);
}

void to_c_expr_literal(program::literal lit, stringstream& sstr) {
  if (lit.negative)
    sstr << "-";
  sstr << lit.value.to_str();
}

void to_c_expr_name(view::view name, stringstream& sstr) {
  sstr << name.to_str();
}

void to_c_expr(program::expression expr, stringstream& sstr) {
  CALL_EXPR(to_c, expr, sstr)
}

string to_c_program(program::program prg) {
  stringstream sstr;
  to_c_expr_fn_decl(prg.main, sstr);
  return sstr.str();
}

}