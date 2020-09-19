#include "parse.h"

#include <list>
#include <string_view>

#include <fmt/core.h>

#include "program.h"
#include "util.h"

bool is_whitespace(char c) {
  return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

bool is_letter(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool is_word_char(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
         (c >= '0' && c <= '9');
}

bool is_digit(char c) { return (c >= '0' && c <= '9'); }

void parse_arg_list(Source &source, Context &context,
                    list<Variable *> &vars) {
  source.skip("(");

  while (!source.peekToken().cmp(")")) {
    Source name = source.getToken();

    auto var = make_unique<Variable>();
    var->name = name.str;
    var->location = source.location;

    vars.push_back(var.get());

    if (source.peekToken().cmp(":")) {
      source.getToken();
      var->type.name = source.getToken().str;
    }
    
    context.variables.push_back(move(var));

    source.skip();
  }

  source.skip(")");
}

void parse_expr_list(Source &source, Context &context,
                     list<unique_ptr<Expression>> &exprs) {
  source.skip("(");

  while (!source.peekToken().cmp(")")) {
    auto e = parse_expr(source, context);
    exprs.push_back(move(e));
  }

  source.skip(")");
}

unique_ptr<FunctionRef> parse_fn_decl(Source &source, Context &context) {
  source.skip("fn");

  Source name = source.getToken();

  auto func = make_unique<Function>();
  func->name = name.str;
  func->context.parent = &context;

  parse_arg_list(source, func->context, func->arguments);

  if (source.peekToken().cmp(":")) {
    source.getToken();
    func->returnType.name = source.getToken().str;
  }

  parse_expr_list(source, func->context, func->expressions);

  context.functions.push_back(move(func));

  auto result = make_unique<FunctionRef>();
  result->function = context.functions.back().get();
  result->location = source.location;

  return result;
}

unique_ptr<FunctionCall> parse_fn_call(Source &source, Context &context) {
  Source name = source.getToken();

  auto result = make_unique<FunctionCall>();
  result->name = name.str;
  result->location = source.location;

  parse_expr_list(source, context, result->arguments);

  return result;
}

unique_ptr<Variable> parse_var(Source &source, Context &context) {
  Source val = source.getToken();

  auto result = make_unique<Variable>();
  result->name = val.str;
  result->location = source.location;

  auto var = context.getVariable(result->name);
  if (var == nullptr)
    throw ERROR(result, "Undefined variable {}", result->name);
  else
    result->type = var->type;

  return result;
}

unique_ptr<String> parse_string(Source &source) {
  Source val = source.getToken();

  auto result = make_unique<String>();
  result->value = val.str;
  result->location = source.location;

  return result;
}

unique_ptr<Number> parse_number(Source &source) {
  Source val = source.getToken();

  auto result = make_unique<Number>();
  result->value = to_long(val);
  result->location = source.location;

  return result;
}

unique_ptr<Assignment> parse_assign(Source &source, Context &context) {
  Source var = source.getToken();

  auto result = make_unique<Assignment>();
  result->location = source.location;

  if (context.getVariable(var.str) == nullptr) {
    auto v = make_unique<Variable>();
    v->name = var.str;
    context.variables.push_back(move(v));
  }

  result->var = context.getVariable(var.str);

  source.skip("=");

  result->expression = parse_expr(source, context);

  return result;
}

unique_ptr<Expression> parse_expr(Source &source, Context &context) {
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

void completeReturnTypes(Context &context) {
  for (auto &f : context.functions) {
    if (f->returnType.name.empty()) {
      f->returnType = f->expressions.back()->getType(context);
    }
    
    completeReturnTypes(f->context);
  }
}

unique_ptr<Function> parse_file(Source &source) {
  try {
    auto result = make_unique<Function>();

    while (source.str.size() > 0) {
      source.skip("");
      auto expr = parse_expr(source, result->context);
      result->expressions.push_back(move(expr));
      source.skip("");
    }

    completeReturnTypes(result->context);

    return result;
  } catch (const std::exception &ex) {
    fmt::print(ex.what());
  }

  return nullptr;
}