  void infer_stmt_assignment(assignment stmt, context *ctx) {

  }

  void infer_stmt_def(vector<var_decl> stmt, context *ctx) {

  }
  
  void infer_stmt_var(vector<var_decl> stmt, context *ctx) {

  }
  
  void infer_stmt_expression(expression stmt, context *ctx) {

  }
  
  void infer_stmt_if(if_stmt stmt, context *ctx) {

  }
  
  void infer_stmt_for(for_stmt stmt, context *ctx) {

  }
  
  void infer_stmt_while(while_stmt stmt, context *ctx) {

  }
  
  void infer_stmt_return(expression stmt, context *ctx) {

  }

  void infer_stmt(statement stmt, context *ctx) {
    CALL_STMT(infer, stmt, ctx)
  }


  void infer_expr_fn_decl(fn_decl expr, context *ctx) {

  }

  void infer_expr_fn_call(fn_call expr, context *ctx) {

  }

  void infer_expr_binary_op(binary_op expr, context *ctx) {

  }

  void infer_expr_literal(literal expr, context *ctx) {

  }

  void infer_expr_name(view::view expr, context *ctx) {

  }

  void infer_expr(expression expr, context *ctx) {
    CALL_EXPR(infer, expr, ctx)
  }