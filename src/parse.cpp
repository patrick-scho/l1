#include "parse.h"

#include <string_view>

#include <fmt/core.h>

#include "program.h"
#include "util.h"

bool is_whitespace(char c)
{
  return c == ' ' ||
         c == '\t' ||
         c == '\r' ||
         c == '\n';
}

bool is_letter(char c)
{
  return (c >= 'a' && c <= 'z') ||
         (c >= 'A' && c <= 'Z');
}

bool is_word_char(char c)
{
  return (c >= 'a' && c <= 'z') ||
         (c >= 'A' && c <= 'Z') ||
         (c >= '0' && c <= '9');
}

bool is_digit(char c)
{
  return (c >= '0' && c <= '9');
}

bool char_in_str(char c, const char *str)
{
  for (int i = 0; str[i] != 0; i++)
  {
    if (c == str[i])
      return true;
  }
  return false;
}

void parse_arg_list(Source &source, Context &context, list<unique_ptr<Variable>> &vars)
{
  source.skip("(");
  list<Variable> args;
  while (!source.peekToken().cmp(")"))
  {
    Source name = source.getToken();
    Variable var;
    var.name = name.str;
    var.location = source.location;
    args.push_back(var);
    if (source.peekToken().cmp(":"))
    {
      source.getToken();
      Source type = source.getToken();
      args.back().type.name = type.str;
    }
    source.skip();
  }
  
  if (!args.empty()) {
    Type type = args.back().type;
    if (type.name.empty()) {
      throw ERROR(&args.back(), "Missing Type Declaration on Argument {}", args.back().name);
    } else {
      for (auto it = args.crbegin(); it != args.crend(); it++) {
        if (!it->type.name.empty())
          type = it->type;

          auto var = make_unique<Variable>();
          var->name = it->name;
          var->type = type;
          var->location = it->location;
          vars.push_front(move(var));
          context.variables.push_front(vars.front().get());
      }
    }
  }
  source.skip(")");
}

void parse_expr_list(Source &source, Context &context, list<unique_ptr<Expression>> &exprs)
{
  source.skip("(");
  while (!source.peekToken().cmp(")"))
  {
    auto e = parse_expr(source, context);
    exprs.push_back(move(e));
  }
  source.skip(")");
}

unique_ptr<Function> parse_fn_decl(Source &source, Context &context)
{
  source.skip("fn");

  Source name = source.getToken();
  auto result = make_unique<Function>();
  result->name = name.str;
  result->context.parent = &context;
  result->location = source.location;
  
  context.functions.push_back(result.get());

  parse_arg_list(source, result->context, result->arguments);

  parse_expr_list(source, result->context, result->expressions);

  result->returnType = result->expressions.back()->getType(context);

  return result;
}

unique_ptr<FunctionCall> parse_fn_call(Source &source, Context &context)
{
  Source name = source.getToken();
  auto result = make_unique<FunctionCall>();
  result->function = name.str;
  result->location = source.location;
  parse_expr_list(source, context, result->arguments);
  return result;
}

unique_ptr<Variable> parse_var(Source &source, Context &context)
{
  Source val = source.getToken();
  auto result = make_unique<Variable>();
  result->name = val.str;
  result->location = source.location;
  auto var = context.getVariable(result.get(), result->name);
  if (var != nullptr)
    result->type = var->type;
  return result;
}

unique_ptr<String> parse_string(Source &source)
{
  Source val = source.getToken();
  auto result = make_unique<String>();
  result->value = val.str;
  result->location = source.location;
  return result;
}

unique_ptr<Number> parse_number(Source &source)
{
  Source val = source.getToken();
  auto result = make_unique<Number>();
  result->value = to_long(val);
  result->location = source.location;
  return result;
}

unique_ptr<Assignment> parse_assign(Source &source, Context &context)
{
  Source var = source.getToken();
  auto result = make_unique<Assignment>();
  result->var = make_unique<Variable>();
  result->location = source.location;
  result->var->name = var.str;
  source.skip("=");
  result->expression = parse_expr(source, context);
  result->var->type = result->expression->getType(context);

  context.variables.push_back(result->var.get());

  return result;
}

unique_ptr<Expression> parse_expr(Source &source, Context &context)
{
  if (source.cmp("fn"))
    return parse_fn_decl(source, context);
  else if (source.peekToken(2).cmp("("))
    return parse_fn_call(source, context);
  else if (source.peekToken(2).cmp("="))
    return parse_assign(source, context);
  else if (is_digit(source.peekToken().get()))
    return parse_number(source);
  else if (source.peekToken().get() == '"')
    return parse_string(source);
  else
    return parse_var(source, context);
}

unique_ptr<Function> parse_file(Source &source, list<unique_ptr<Function>>& functions)
{
  try {
    auto result = make_unique<Function>();

    for (auto& f: functions) {
      result->context.functions.push_back(f.get());
    }

    while (source.str.size() > 0)
    {
      source.skip("");
      auto expr = parse_expr(source, result->context);
      result->expressions.push_back(move(expr));
      source.skip("");
    }

    return result;
  }
  catch (const std::exception& ex) {
    fmt::print(ex.what());
  }

  return nullptr;
}