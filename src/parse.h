#pragma once

#include <list>
#include <string_view>

#include "program.h"

using namespace std;

bool is_whitespace(char c);
bool is_letter(char c);
bool is_word_char(char c);
bool is_digit(char c);

void parse_arg_list(Source &source, Context &context,
                    list<Variable *> &vars);
void parse_expr_list(Source &source, Context &context,
                     list<unique_ptr<Expression>> &exprs);

unique_ptr<FunctionRef> parse_fn_decl(Source &source, Context &context);
unique_ptr<FunctionCall> parse_fn_call(Source &source, Context &context);
unique_ptr<Variable> parse_var(Source &source, Context &context);
unique_ptr<String> parse_string(Source &source);
unique_ptr<Number> parse_number(Source &source);
unique_ptr<Assignment> parse_assign(Source &source, Context &context);

unique_ptr<Expression> parse_expr(Source &source, Context &context);

unique_ptr<Function> parse_file(Source &source);
