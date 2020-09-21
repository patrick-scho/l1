#include "parse.h"

#include <vector>
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
                    vector<unique_ptr<VariableRef>> &vars) {
  source.skip("(");

  while (!source.peekToken().cmp(")")) {
    Source name = source.getToken();

    auto var = make_unique<Variable>();
    var->name = name.str;

    auto ref = make_unique<VariableRef>();
    ref->variable = var.get();
    ref->location = source.location;

    var->definition = ref.get();

    vars.push_back(move(ref));

    if (source.peekToken().cmp(":")) {
      source.getToken();
      var->type.name = source.getToken().str;
    }

    context.variables.push_back(move(var));

    source.skip();
  }

  int typeless = 0;
  for (int i = 0; i < vars.size(); i++) {
    auto var = vars[i]->variable;
    if (var->type.name.empty()) {
      typeless++;
    }
    else {
      for (int j = 1; j <= typeless; j++) {
        vars[i - j]->variable->type = var->type;
      }
    }
  }

  source.skip(")");
}

void parse_expr_list(Source &source, Context &context,
                     vector<unique_ptr<Expression>> &exprs) {
  source.skip("(");

  while (!source.peekToken().cmp(")")) {
    auto e = parse_expr(source, context);
    exprs.push_back(move(e));
  }

  source.skip(")");
}

unique_ptr<FunctionRef> parse_fn_decl(Source &source, Context &context) {
  source.skip("func");

  Source name = source.getToken();

  auto func = make_unique<Function>();
  func->name = name.str;
  func->context.parent = &context;

  int n = lists::count(context.functions,
    [&func](unique_ptr<Function>& f) { return (f->name == func->name); });
  if (n > 0)
    func->appendix = '0' + n;

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

  context.functions.back().get()->definition = result.get();

  return result;
}

unique_ptr<MetaFunctionRef> parse_meta_decl(Source &source, Context &context) {
  source.skip("meta");

  Source name = source.getToken();

  auto func = make_unique<MetaFunction>();
  func->name = name.str;
  func->location = source.location;
  func->context.parent = &context;
  func->context.is_meta = true;

  parse_arg_list(source, func->context, func->arguments);

  parse_expr_list(source, func->context, func->expressions);

  context.meta_functions.push_back(move(func));

  auto result = make_unique<MetaFunctionRef>();
  result->meta_function = context.meta_functions.back().get();
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

unique_ptr<Assignment> parse_assign(Source &source, Context &context) {
  Source var = source.getToken();

  auto result = make_unique<Assignment>();
  result->location = source.location;

  if (context.get_variable(var.str) == nullptr) {
    auto v = make_unique<Variable>();
    v->name = var.str;

    context.variables.push_back(move(v));
  }

  result->var = context.get_variable(var.str);

  source.skip("=");

  result->expression = parse_expr(source, context);
  result->var->definition = result->expression.get();

  return result;
}

unique_ptr<VariableRef> parse_var(Source &source, Context &context) {
  Source val = source.getToken();

  auto result = make_unique<VariableRef>();
  result->location = source.location;
  result->name = val.str;

  if (!context.is_meta_context()) {
    auto var = context.get_variable(val.str);
    if (var == nullptr)
      throw ERROR(result->location, "Undefined variable {}", val.str);
      
    result->variable = var;
  }

  return result;
}

unique_ptr<CCall> parse_c_call(Source &source, Context &context) {
  auto result = make_unique<CCall>();
  source.getToken(3);
  int length = 0;
  while (source.get(length + 0) != ')' || source.get(length + 1) != ')')
    length++;
  result->value = source.str.substr(0, length);
  source.adv(length + 2);
  return result;
}

unique_ptr<Number> parse_number(Source &source) {
  Source val = source.getToken();

  auto result = make_unique<Number>();
  result->value = to_long(val);
  result->location = source.location;

  return result;
}

unique_ptr<String> parse_string(Source &source) {
  Source val = source.getToken();

  auto result = make_unique<String>();
  result->value = val.str.substr(1, val.str.size() - 2);
  result->location = source.location;

  return result;
}

unique_ptr<Expression> parse_expr(Source &source, Context &context) {
  bool is_meta = false;
  if (source.cmp("@")) {
    is_meta = true;
    source.getToken();
  }

  unique_ptr<Expression> result;

  while (source.cmp("#")) {
    if (source.cmp("##")) {
      source.adv(2);
      while (source.get() != '#' || source.get(1) != '#')
        source.adv(1);
      source.adv(2);
    }
    else {
      source.adv(1);
      while (source.get() != '\n')
        source.adv(1);
    }
    source.skip();
  }

  if (source.cmp("func"))
    result = parse_fn_decl(source, context);
    
  else if (source.cmp("meta"))
    result = parse_meta_decl(source, context);

  else if (source.cmp("C") && source.peekToken(2).cmp("(") && source.peekToken(3).cmp("("))
    result = parse_c_call(source, context);

  else if (source.peekToken(2).cmp("("))
    result = parse_fn_call(source, context);

  else if (source.peekToken(2).cmp("="))
    result = parse_assign(source, context);

  else if (is_digit(source.peekToken().get()))
    result = parse_number(source);
    
  else if (source.peekToken().get() == '"')
    result = parse_string(source);

  else
    result = parse_var(source, context);

  if (is_meta)
    result->is_meta = true;

  return result;
}

void inferTypes(Context &context) {
  for (auto &f : context.functions) {
    if (f->returnType.name.empty()) {
      f->returnType = f->get_type(context);
    }
    
    inferTypes(f->context);
  }

  for (auto &v : context.variables) {
    if (v->type.name.empty()) {
      v->type = v->get_type(context);
    }
  }
}

void checkDuplicates(Context &context) {
  for (int i1 = 0; i1 < context.functions.size(); i1++) {
    for (int i2 = i1 + 1; i2 < context.functions.size(); i2++) {
      auto& f1 = context.functions[i1];
      auto& f2 = context.functions[i2];
      if (f1->name == f2->name && f1->arguments.size() == f2->arguments.size()) {
        for (int i = 0; i < f1->arguments.size(); i++) {
          if (f1->arguments[i]->variable->type.name == f2->arguments[i]->variable->type.name)
            throw ERROR(f2->definition->location, "Redefinition of Function {}", f1->name);
        }
      }
    }
    checkDuplicates(context.functions[i1]->context);
  }
  
  for (int i1 = 0; i1 < context.variables.size(); i1++) {
    for (int i2 = i1 + 1; i2 < context.variables.size(); i2++) {
      auto& var1 = context.variables[i1];
      auto& var2 = context.variables[i2];
      if (var1->name == var2->name)
        throw ERROR(var2->definition->location, "Redefinition of Variable {}", var1->name);
    }
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

    inferTypes(result->context);
    checkDuplicates(result->context);

    return result;
  } catch (const std::exception &ex) {
    fmt::print(ex.what());
  }

  return nullptr;
}