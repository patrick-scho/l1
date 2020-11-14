#pragma once

#include <string>

#include "view.hpp"

using namespace std;

namespace lex {

enum struct token_type {
  integer, decimal, string, identifier, symbol,
  equals, comma, bracket_open, bracket_close, colon,
  def, fn, if_, else_, for_, while_, return_
};

struct token {
  token_type type;
  
  view::view view;

  int64_t integer;
  double decimal;
  bool boolean;
};

bool is_digit(char32_t c) {
  return c >= '0' && c <= '9';
}

bool is_word_char(char32_t c) {
  return
    c >= '0' && c <= '9' ||
    c >= 'a' && c <= 'z' ||
    c >= 'A' && c <= 'Z' ||
    c == '_';
}

token get_number(view::view &v) {
  token result;
  result.view = v;

  bool neg = false;
  if (v.at(0) == '+') {
    v.adv(1);
  }
  else if (v.at(0) == '-') {
    neg = true;
    v.adv(1);
  }

  int i = 0;
  while (is_digit(v.at(0))) {
    i *= 10;
    i += v.at(0) - '0';
    v.adv(1);
  }
  if (v.at(0) == '.') {
    v.adv(1);
    double d = 0;
    int decPlace = 1;
    while (is_digit(v.at(0))) {
      d += (double)(v.at(0) - '0') / pow(10, decPlace);
      v.adv(1);
      decPlace++;
    }
    result.type = token_type::decimal;
    result.decimal = (d + i);
    if (neg) result.decimal = -result.decimal;
    return result;
  }
  result.type = token_type::integer;
  result.integer = i;
  if (neg) result.integer = -result.integer;

  result.view = result.view.sub(
    0,
    v.get_loc().index - result.view.get_loc().index
  );

  return result;
}

token get_string(view::view &v) {
  token result;
  result.view = v;
  result.type = token_type::string;

  v.adv(1);

  while (v.at(0) != '"' || v.at(-1) == '\\') v.adv(1);

  result.view = result.view.sub(
    0,
    v.get_loc().index - result.view.get_loc().index
  );

  v.adv(1);

  return result;
}

token get_identifier(view::view &v) {
  token result;
  result.view = v;
  result.type = token_type::identifier;

  while (is_word_char(v.at(0))) v.adv(1);

  result.view = result.view.sub(
    0,
    v.get_loc().index - result.view.get_loc().index
  );

  string s = result.view.to_str();
       if (s == "def")   result.type = token_type::def;
  else if (s == "fn")    result.type = token_type::fn;
  else if (s == "if")    result.type = token_type::if_;
  else if (s == "else")  result.type = token_type::else_;
  else if (s == "for")   result.type = token_type::for_;
  else if (s == "while") result.type = token_type::while_;
  else if (s == "return") result.type = token_type::return_;

  return result;
}

token get_symbol(view::view &v) {
  token result;
  result.view = v.sub(0, 1);
  result.type = token_type::symbol;

  v.adv(1);

  return result;
}

token get_builtin(view::view &v, token_type type) {
  token result;
  result.view = v.sub(0, 1);
  result.type = type;

  v.adv(1);

  return result;
}

vector<token> lex(view::view v) {
  vector<token> result;

  while (!v.end()) {
    v.skip();

    char32_t c = v.at(0);

    if (c == '/' && v.at(1) == '/') {
      while (v.at(0) != '\n') v.adv(1);
    }
    else if (c == '/' && v.at(1) == '*') {
      v.adv(2);
      int depth = 1;
      while (depth != 0) {
        if (v.at(0) == '/' && v.at(1) == '*') depth++;
        if (v.at(0) == '*' && v.at(1) == '/') depth--;

        v.adv(1);
      }
      v.adv(1);
    }
    else if (is_digit(c) || ((c == '+' || c == '-') && is_digit(v.at(1))))
      result.push_back(get_number(v));
    else if (c == '"')
      result.push_back(get_string(v));
    else if (is_word_char(c))
      result.push_back(get_identifier(v));
    else if (c == '=')
      result.push_back(get_builtin(v, token_type::equals));
    else if (c == ',')
      result.push_back(get_builtin(v, token_type::comma));
    else if (c == ':')
      result.push_back(get_builtin(v, token_type::colon));
    else if (c == '(')
      result.push_back(get_builtin(v, token_type::bracket_open));
    else if (c == ')')
      result.push_back(get_builtin(v, token_type::bracket_close));
    else if (c == '{')
      result.push_back(get_builtin(v, token_type::bracket_open));
    else if (c == '}')
      result.push_back(get_builtin(v, token_type::bracket_close));
    else
      result.push_back(get_symbol(v));
  }

  return result;
}

}
