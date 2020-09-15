#include "parse.h"

#include <stdlib.h>

#include "source.h"
#include "util.h"
#include "array.h"

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

void parse_arg_list(Source* source, Context* context, Variable*** vars) {
  source_skip(source, "(");
  while (!source_cmp(source_peek_token(*source), ")")) {
    Source name = source_get_token(source);
    Source type = { .data="i32", .index=0, .size=3 };
    if (source_cmp(source_peek_token(*source), ":")) {
      source_get_token(source);
      type = source_get_token(source);
    }
    *vars = array_add(*vars, Variable_new(name, type));
    source_skip(source, "");
  }
  source_skip(source, ")");
}

void parse_expr_list(Source* source, Context* context, Expression*** exprs) {
  source_skip(source, "(");
  while (!source_cmp(source_peek_token(*source), ")")) {
    Expression* e = parse_expr(source, context);
    *exprs = array_add(*exprs, e);
  }
  source_skip(source, ")");
}

Function* parse_fn_decl(Source* source, Context* context) {
  source_skip(source, "fn");
  
  Source name = source_get_token(source);
  Function* result = Function_new(name);
  
  parse_arg_list(source, context, &result->arguments);

  parse_expr_list(source, context, &result->expressions);

  return result;
}

FunctionCall* parse_fn_call(Source* source, Context* context) {
  Source name = source_get_token(source);
  FunctionCall* result = FunctionCall_new(name);
  parse_expr_list(source, context, &result->arguments);
  return result;
}

// FunctionCall* parse_op_call(Source* source, Context* context) {
//   Source name = source_get_token(source);
//   parse_expr_list(source);
// }

char* parse_var(Source* source, Context* context) {
  Source val = source_get_token(source);
  char* result;
  copy_str(&result, val);
  return result;
}

char* parse_string(Source* source) {
  Source val = source_get_token(source);
  char* result;
  copy_str(&result, val);
  return result;
}

long parse_number(Source* source) {
  Source val = source_get_token(source);
  return to_long(val);
}

Expression* parse_expr(Source* source, Context* context) {
  Expression* result = Expression_new();

  if (source_cmp(*source, "fn")){
    result->type = EXPR_FN_DECL;
    Function* fn = parse_fn_decl(source, context);
    result->functionDeclaration = fn;
  }
  else if(source_cmp(source_peek_token_n(*source, 2), "(")) {
    result->type = EXPR_FN_CALL;
    FunctionCall* fc = parse_fn_call(source, context);
    result->functionCall = fc;
  }
  // else if(source_cmp(source_peek_token_n(*source, 2), "=")) {
  //   result->type = EXPR_FN_CALL;
  //   FunctionCall* fc = parse_op_call(source);
  //   result->functionCall = fc;
  // }
  else {
    Source val = source_peek_token(*source);
    if (is_digit(val.data[0])) {
      result->type = EXPR_NUMBER;
      result->number = parse_number(source);
    }
    else if (val.data[0] == '"') {
      result->type = EXPR_STRING;
      result->string = parse_string(source);
    }
    else {
      result->type = EXPR_VAR;
      result->variable = parse_var(source, context);
    }
  }

  return result;
}

Function* parse_file(Source* source) {
  Source name = { .data="main", .index=0, .size=4 };
  Function* result = Function_new(name);

  while (source->index < source->size) {
    source_skip(source, "");
    Expression* expr = parse_expr(source, result->context);
    result->expressions = array_add(result->expressions, expr);
    source_skip(source, "");
  }

  return result;
}