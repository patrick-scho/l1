#include "parse.h"

#include <string_view>

#include "program.h"
#include "util.h"

bool is_whitespace(char c) {
  return
    c == ' ' ||
    c == '\t' ||
    c == '\r' ||
    c == '\n';
}

bool is_letter(char c) {
  return
    (c >= 'a' && c <= 'z') ||
    (c >= 'A' && c <= 'Z');
}

bool is_word_char(char c) {
  return
    (c >= 'a' && c <= 'z') ||
    (c >= 'A' && c <= 'Z') ||
    (c >= '0' && c <= '9');
}

bool is_digit(char c) {
  return
    (c >= '0' && c <= '9');
}

bool char_in_str(char c, const char* str) {
  for (int i = 0; str[i] != 0; i++) {
    if (c == str[i])
      return true;
  }
  return false;
}



void parse_arg_list(Source& source, Context& context, list<Variable>& vars) {
  source.skip("(");
  while (!source.peekToken().cmp(")")) {
    Source name = source.getToken();
    Source type;
    type.str = "i32";
    if (source.peekToken().cmp(":")) {
      source.getToken();
      type = source.getToken();
    }
    Variable var;
    var.name = name.str;
    var.type = type.str;
    vars.push_back(var);
    source.skip();
  }
  source.skip(")");
}

void parse_expr_list(Source& source, Context& context, list<Expression>& exprs) {
  source.skip("(");
  while (!source.peekToken().cmp(")")) {
    Expression e = parse_expr(source, context);
    exprs.push_back(e);
  }
  source.skip(")");
}

Function parse_fn_decl(Source& source, Context& context) {
  source.skip("fn");
  
  Source name = source.getToken();
  Function result;
  result.name = name.str;
  
  parse_arg_list(source, context, result.arguments);

  parse_expr_list(source, context, result.expressions);

  return result;
}

FunctionCall parse_fn_call(Source& source, Context& context) {
  Source name = source.getToken();
  FunctionCall result;
  result.function = name.str;
  parse_expr_list(source, context, result.arguments);
  return result;
}

// FunctionCall* parse_op_call(Source& source, Context& context) {
//   Source name = source.getToken();
//   parse_expr_list(source);
// }

string_view parse_var(Source& source, Context& context) {
  Source val = source.getToken();
  return val.str;
}

string_view parse_string(Source& source) {
  Source val = source.getToken();
  return val.str;
}

long parse_number(Source& source) {
  Source val = source.getToken();
  return to_long(val);
}

Expression parse_expr(Source& source, Context& context) {
  Expression result;

  if (source.cmp("fn")){
    result.type = ExpressionType::FnDecl;
    Function fn = parse_fn_decl(source, context);
    result.functionDeclaration = fn;
  }
  else if(source.peekToken(2).cmp("(")) {
    result.type = ExpressionType::FnCall;
    FunctionCall fc = parse_fn_call(source, context);
    result.functionCall = fc;
  }
  // else if(source_cmp(source.peekToken_n(*source, 2), "=")) {
  //   result->type = EXPR_FN_CALL;
  //   FunctionCall* fc = parse_op_call(source);
  //   result->functionCall = fc;
  // }
  else {
    Source val = source.peekToken();
    if (is_digit(val.get())) {
      result.type = ExpressionType::Number;
      result.number = parse_number(source);
    }
    else if (val.get() == '"') {
      result.type = ExpressionType::String;
      result.string = parse_string(source);
    }
    else {
      result.type = ExpressionType::Var;
      result.variable = parse_var(source, context);
    }
  }

  return result;
}

Function parse_file(Source& source) {
  Function result;

  while (source.str.size() > 0) {
    source.skip("");
    Expression expr = parse_expr(source, result.context);
    result.expressions.push_back(expr);
    source.skip("");
  }

  return result;
}