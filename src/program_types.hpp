#pragma once

#include "program.hpp"

namespace program {
  void check_stmt(statement &stmt, context &ctx);


  type check_expr_fn_decl(fn_decl &expr, context &ctx) {
    int typeless = 0;
    for (int i = 0; i < expr.parameters.size(); i++) {
      if (! expr.parameters[i].type.has_value()) {
        typeless++;
      } else {
        for (int j = 1; j <= typeless; j++) {
          expr.parameters[i - j].type = expr.parameters[i].type;
        }
        typeless = 0;
      }
    }
    if (typeless != 0)
      util::crash(expr.parameters.back().name.get_loc(), "Missing type annotation");

    // TODO infer return type

    for (int i = 0; i < expr.statements.size(); i++) {
      check_stmt(expr.statements[i], expr.ctx);
    }

    type result;
    result.name = view::view(U"fn");
    return result;
  }

  type check_expr_fn_call(fn_call &expr, context &ctx) {
    type result;
    return result;
  }

  type check_expr_binary_op(binary_op &expr, context &ctx) {
    type result;
    return result;
  }

  type check_expr_literal(literal &expr, context &ctx) {
    type result;
    return result;
  }

  type check_expr_name(view::view &expr, context &ctx) {
    type result;
    return result;
  }

  type check_expr(expression &expr, context &ctx) {
    RETURN_EXPR(check, expr, ctx)
    return type{};
  }


  void check_stmt_assignment(assignment &stmt, context &ctx) {

  }

  void check_stmt_def(var_decl &stmt, context &ctx) {
    if (! stmt.type.has_value() && stmt.value.has_value()) {
      stmt.type = check_expr(stmt.value.value(), ctx);
    }
  }
  
  void check_stmt_var(var_decl &stmt, context &ctx) {
    if (! stmt.type.has_value() && stmt.value.has_value()) {
      stmt.type = check_expr(stmt.value.value(), ctx);
    }
  }
  
  void check_stmt_expression(expression &stmt, context &ctx) {

  }
  
  void check_stmt_if(if_stmt &stmt, context &ctx) {

  }
  
  void check_stmt_for(for_stmt &stmt, context &ctx) {

  }
  
  void check_stmt_while(while_stmt &stmt, context &ctx) {

  }
  
  void check_stmt_return(expression &stmt, context &ctx) {

  }

  void check_stmt(statement &stmt, context &ctx) {
    CALL_STMT(check, stmt, ctx)
  }

  void check_types(program &prg) {
    for (int i = 0; i < prg.main.statements.size(); i++) {
      check_stmt(prg.main.statements[i], prg.main.ctx);
    }
  }
}