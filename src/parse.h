#pragma once

#include <list>
#include <string_view>

#include "program.h"

using namespace std;

bool is_whitespace(char c);
bool is_letter(char c);
bool is_word_char(char c);
bool is_digit(char c);
bool char_in_str(char c, const char* str);

void parse_arg_list(Source& source, Context& context, list<Variable>& vars);
void parse_expr_list(Source& source, Context& context, list<Expression>& exprs);
Function parse_fn_decl(Source& source, Context& context);
FunctionCall parse_fn_call(Source& source, Context& context);
string_view parse_var(Source& source, Context& context);
string_view parse_string(Source& source);
long parse_number(Source& source);
Assignment parse_assign(Source& source, Context& context);
Expression parse_expr(Source& source, Context& context);

Function parse_file(Source& source);
