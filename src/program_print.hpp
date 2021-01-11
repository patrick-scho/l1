#pragma once

#include <iostream>

#include "program.hpp"

using namespace std;

void indent(int n) {
  for (int i = 0; i < n; i++)
    cout << " ";
}

namespace program {

// Forward Declarations
void print_expr(expression expr, int ind);
void print_stmt(statement stmt, int ind);

void print_var_decl(var_decl var, int ind) {
  cout << var.name.to_str();
  if (var.type.has_value())
    cout << " : " << var.type.value().name.to_str();
  if (var.value.has_value()) {
    cout << " = ";
    print_expr(var.value.value(), ind);
  }
}

void print_stmt_assignment(assignment asgn, int ind) {
  indent(ind);
  cout << asgn.name.to_str() << " = ";
  print_expr(asgn.expr, 0);
  cout << "\n";
}

void print_stmt_def(var_decl def, int ind) {
  indent(ind);
  cout << "def ";
  print_var_decl(def, ind);
  cout << "\n";
}

void print_stmt_var(var_decl var, int ind) {
  indent(ind);
  cout << "var ";
  print_var_decl(var, ind);
  cout << "\n";
}

void print_stmt_expression(expression expr, int ind) {
  indent(ind);
  print_expr(expr, 0);
  cout << "\n";
}

void print_stmt_if(if_stmt if_, int ind) {
  indent(ind);
  cout << "if ";
  print_expr(if_.condition, 0);
  cout << " {\n";

  for (int i = 0; i < if_.statements.size(); i++) {
    print_stmt(if_.statements[i], ind + 2);
  }
  indent(ind);
  cout << "}";

  for (int i = 0; i < if_.else_ifs.size(); i++) {
    cout << "\n";
    indent(ind);
    cout << "else if ";
    print_expr(if_.else_ifs[i].condition, 0);
    cout << " {\n";

    for (int i = 0; i < if_.else_ifs[i].statements.size(); i++) {
      print_stmt(if_.else_ifs[i].statements[i], ind + 2);
    }
    indent(ind);
    cout << "}";
  }

  if (if_.else_statements.has_value()) {
    cout << "\n";
    indent(ind);
    cout << "else {\n";
    for (int i = 0; i < if_.else_statements.value().size(); i++) {
      print_stmt(if_.else_statements.value()[i], ind + 2);
    }
    indent(ind);
    cout << "}";
  }

  cout << "\n";
}

void print_stmt_for(for_stmt for_, int ind) {
  indent(ind);
  cout << "for ";
  print_expr(for_.condition, 0);
  cout << " {\n";

  for (int i = 0; i < for_.statements.size(); i++) {
    print_stmt(for_.statements[i], ind + 2);
  }
  indent(ind);
  cout << "}";
}

void print_stmt_while(while_stmt while_, int ind) {
  indent(ind);
  cout << "while ";
  print_expr(while_.condition, 0);
  cout << " {\n";

  for (int i = 0; i < while_.statements.size(); i++) {
    print_stmt(while_.statements[i], ind + 2);
  }
  indent(ind);
  cout << "}";
}

void print_stmt_return(expression return_, int ind) {
  indent(ind);
  cout << "return ";
  print_expr(return_, ind);
  cout << "\n";
}

void print_stmt(statement stmt, int ind) {
  CALL_STMT(print, stmt, ind)
}


void print_expr_fn_decl(fn_decl fn, int ind) {
  cout << "Function ";
  if (fn.name.has_value())
    cout << fn.name.value().to_str() << " ";
  cout << "(";
  for (int i = 0; i < fn.parameters.size(); i++) {
    if (i != 0) cout << ", ";
    print_var_decl(fn.parameters[i], 0);
  } 
  cout << ")";
  if (fn.return_type.has_value())
    cout << " : " << fn.return_type.value().name.to_str();
  cout << " {\n";
  for (int i = 0; i < fn.statements.size(); i++) {
    print_stmt(fn.statements[i], ind + 2);
  }
  indent(ind);
  cout << "}";
}

void print_expr_fn_call(fn_call fn, int ind) {
  cout << fn.name.to_str() << "(";
  for (int i = 0; i < fn.arguments.size(); i++) {
    if (i != 0) cout << ", ";
    print_expr(fn.arguments[i], ind);
  }
  cout << ")";
}

void print_expr_binary_op(binary_op op, int ind) {
  print_expr(op.left, ind);
  cout << " " << op.op.to_str() << " ";
  print_expr(op.right, ind);
}

void print_expr_literal(literal lit, int ind) {
  if (lit.negative)
    cout << "-";
  cout << lit.value.to_str();
}

void print_expr_name(view::view name, int ind) {
  cout << name.to_str();
}

void print_expr(expression expr, int ind) {
  CALL_EXPR(print, expr, ind)
}

void print_program(program prg) {
  print_expr_fn_decl(prg.main, 0);
}

}