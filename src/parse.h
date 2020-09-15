#ifndef PARSE__H
#define PARSE__H

#include <stdbool.h>

#include "source.h"
#include "program.h"

bool is_whitespace(char c);
bool is_letter(char c);
bool is_word_char(char c);
bool is_digit(char c);
bool char_in_str(char c, const char* str);

void parse_arg_list(Source* source, Context* context, Variable*** vars);
void parse_expr_list(Source* source, Context* context, Expression*** exprs);
Function* parse_fn_decl(Source* source, Context* context);
FunctionCall* parse_fn_call(Source* source, Context* context);
char* parse_var(Source* source, Context* context);
char* parse_string(Source* source);
long parse_number(Source* source);
Expression* parse_expr(Source* source, Context* context);

Function* parse_file(Source* source);

#endif
