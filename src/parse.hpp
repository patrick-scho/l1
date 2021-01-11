#pragma once

#include <string>
#include <vector>

#include "program.hpp"

using namespace std;

namespace parse {

// Forward Declarations
vector<program::statement> parse_statement(const vector<lex::token> &tokens, int &index);
program::expression parse_expression(const vector<lex::token> &tokens, int &index);
vector<program::statement> parse_block(const vector<lex::token> &tokens, int &index);
vector<program::var_decl> parse_var_decls(const vector<lex::token> &tokens, int &index);

program::fn_decl parse_fn_decl(const vector<lex::token> &tokens, int &index) {
  program::fn_decl result;

  lex::token t = tokens[index];
  if (t.view.to_str() != "(")
    util::crash(t.view.get_loc(), "Expected '('");

  index++;
  
  result.parameters = parse_var_decls(tokens, index);

  t = tokens[index];
  if (t.view.to_str() != ")")
    util::crash(t.view.get_loc(), "Expected ')'");
    
  index++;

  t = tokens[index];
  if (t.type == lex::token_type::colon) {
    index++;
    result.return_type = program::type { tokens[index].view };
    index++;
  }

  result.statements = parse_block(tokens, index);

  return result;
}

program::fn_call parse_fn_call(const vector<lex::token> &tokens, int &index) {
  program::fn_call result;
  
  lex::token t = tokens[index];

  if (t.type != lex::token_type::identifier)
    util::crash(t.view.get_loc(), "Expected function name");

  result.name = t.view;
    
  index++;

  t = tokens[index];

  if (t.view.to_str() != "(")
    util::crash(t.view.get_loc(), "Expected '('");

  index++;

  while (true) {
    result.arguments.push_back(parse_expression(tokens, index));
    t = tokens[index];
    if (t.type == lex::token_type::comma)
      index++;
    else
      break;
  }

  if (t.view.to_str() != ")")
    util::crash(t.view.get_loc(), "Expected ')'");

  index++;

  return result;
}

program::expression parse_expression(const vector<lex::token> &tokens, int &index) {
  program::expression result;

  lex::token t = tokens[index];

  if (t.type == lex::token_type::fn) {
    index++;
    result.type = program::expression_type::fn_decl;
    result.fn_decl = parse_fn_decl(tokens, index);
  } else if (t.type == lex::token_type::identifier) {
    if (((lex::token)tokens[index+1]).view.to_str() == "(") {
      result.type = program::expression_type::fn_call;
      result.fn_call = parse_fn_call(tokens, index);
    } else {
      result.type = program::expression_type::name;
      result.name = t.view;
      index++;
    }
  } else if (t.type == lex::token_type::integer ||
             t.view.to_str() == "-" && tokens[index+1].type == lex::token_type::integer ||
             t.view.to_str() == "+" && tokens[index+1].type == lex::token_type::integer) {
    if (t.type == lex::token_type::symbol) {
      if (t.view.to_str() == "-")
        result.literal.negative = true;
      index++;
      t = tokens[index];
    }

    result.type = program::expression_type::literal;
    result.literal.type = program::literal_type::integer;
    result.literal.value = t.view;
    index++;
  } else if (t.type == lex::token_type::decimal) {
    result.type = program::expression_type::literal;
    result.literal.type = program::literal_type::decimal;
    result.literal.value = t.view;
    index++;
  } else if (t.type == lex::token_type::string) {
    result.type = program::expression_type::literal;
    result.literal.type = program::literal_type::string;
    result.literal.value = t.view;
    index++;
  } else {
    util::crash(t.view.get_loc(), "Expected expression");
  }

  t = tokens[index];
  if (t.type == lex::token_type::symbol) {
    program::expression tmp = result;
    result.type = program::expression_type::binary_op;
    result.binary_op.left = tmp;
    result.binary_op.op = t.view;
    index++;
    result.binary_op.right = parse_expression(tokens, index);
  }

  return result;
}

vector<program::statement> parse_block(const vector<lex::token> &tokens, int &index) {
  vector<program::statement> result;

  lex::token t = tokens[index];

  if (t.view.to_str() != "{")
    util::crash(t.view.get_loc(), "Expected '{'");
    
  index++;

  t = tokens[index];

  while (t.view.to_str() != "}") {
    vector<program::statement> stmts = parse_statement(tokens, index);
    for (int i = 0; i < stmts.size(); i++)
      result.push_back(stmts[i]);

    t = tokens[index];
  }

  index++;

  return result;
}

vector<program::var_decl> parse_var_decls(const vector<lex::token> &tokens, int &index) {
  vector<program::var_decl> result;

  bool is_def = true;

  if (tokens[index-1].type == lex::token_type::var)
    is_def = false;

  while (true) {
    program::var_decl v;

    lex::token t = tokens[index];

    if (t.type != lex::token_type::identifier)
      util::crash(t.view.get_loc(), "Expected definition name");

    v.name = t.view;
      
    index++;

    t = tokens[index];

    if (t.type == lex::token_type::colon) {
      index++;
      t = tokens[index];
      v.type = program::type { t.view };
      index++;
    }

    t = tokens[index];

    if (t.type == lex::token_type::equals) {
      index++;
      program::expression expr = parse_expression(tokens, index);
      if (expr.type == program::expression_type::fn_decl) {
        expr.fn_decl.name = v.name;
      }
      v.value = util::ref(expr);
    }

    result.push_back(v);

    t = tokens[index];

    if (t.type != lex::token_type::comma)
      break;
      
    index++;
  }

  return result;
}

program::if_stmt parse_if_stmt(const vector<lex::token> &tokens, int &index) {
  program::if_stmt result;

  result.condition = parse_expression(tokens, index);
  result.statements = parse_block(tokens, index);

  while (true) {
    lex::token t = tokens[index];

    if (t.type != lex::token_type::else_)
      break;

    index++;
    t = tokens[index];

    if (t.type == lex::token_type::if_) {
      index++;
      program::if_stmt else_if;
      else_if.condition = parse_expression(tokens, index);
      else_if.statements = parse_block(tokens, index);
      result.else_ifs.push_back(else_if);
    } else {
      result.else_statements = parse_block(tokens, index);
      break;
    }
  }

  return result;
}

program::for_stmt parse_for_stmt(const vector<lex::token> &tokens, int &index) {
  program::for_stmt result;

  result.condition = parse_expression(tokens, index);
  result.statements = parse_block(tokens, index);

  return result;
}

program::while_stmt parse_while_stmt(const vector<lex::token> &tokens, int &index) {
  program::while_stmt result;

  result.condition = parse_expression(tokens, index);
  result.statements = parse_block(tokens, index);

  return result;
}

program::assignment parse_assignment(const vector<lex::token> &tokens, int &index) {
  program::assignment result;

  lex::token t = tokens[index];

  if (t.type != lex::token_type::identifier)
    util::crash(t.view.get_loc(), "Expected identifier");

  result.name = t.view;

  index += 2;

  t = tokens[index];

  result.expr = parse_expression(tokens, index);

  return result;
}

vector<program::statement> parse_statement(const vector<lex::token> &tokens, int &index) {
  vector<program::statement> results;
  program::statement result;

  lex::token t = tokens[index];

  if (t.type == lex::token_type::def) {
    index++;
    vector<program::var_decl> decls = parse_var_decls(tokens, index);
    for (int i = 0; i < decls.size(); i++) {
      result.type = program::statement_type::def;
      result.def = decls[i];
      results.push_back(result);
    }
  } else if (t.type == lex::token_type::var) {
    index++;
    vector<program::var_decl> decls = parse_var_decls(tokens, index);
    for (int i = 0; i < decls.size(); i++) {
      result.type = program::statement_type::var;
      result.var = decls[i];
      results.push_back(result);
    }
  } else if (t.type == lex::token_type::if_) {
    index++;
    result.type = program::statement_type::if_;
    result.if_ = parse_if_stmt(tokens, index);
    results.push_back(result);
  } else if (t.type == lex::token_type::for_) {
    index++;
    result.type = program::statement_type::for_;
    result.for_ = parse_for_stmt(tokens, index);
    results.push_back(result);
  } else if (t.type == lex::token_type::while_) {
    index++;
    result.type = program::statement_type::while_;
    result.while_ = parse_while_stmt(tokens, index);
    results.push_back(result);
  } else if (t.type == lex::token_type::return_) {
    index++;
    result.type = program::statement_type::return_;
    result.return_ = parse_expression(tokens, index);
    results.push_back(result);
  } else if (tokens[index+1].type == lex::token_type::equals) {
    result.type = program::statement_type::assignment;
    result.assignment = parse_assignment(tokens, index);
    results.push_back(result);
  } else {
    result.type = program::statement_type::expression;
    result.expression = parse_expression(tokens, index);
    results.push_back(result);
  }

  return results;
}

program::program parse(const vector<lex::token> &tokens) {
  program::program result;

  int index = 0;

  while (index < tokens.size()) {
    vector<program::statement> stmts = parse_statement(tokens, index);
    for (int i = 0; i < stmts.size(); i++)
      result.main.statements.push_back(stmts[i]);
  }

  return result;
}

} // namespace parse
