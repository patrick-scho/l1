#pragma once

#include <string>

#include "view.hpp"

using namespace std;

namespace lex {

enum struct token_type {
  integer, decimal, string, identifier, symbol
};

struct token {
  token_type type;
  view::location loc;
  
  string s;
  int64_t i;
  double d;
  bool b;
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
  result.loc = v.get_loc();

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
    result.d = (d + i);
    if (neg) result.d = -result.d;
    return result;
  }
  result.type = token_type::integer;
  result.i = i;
  if (neg) result.i = -result.i;
  return result;
}

token get_string(view::view &v) {
  token result;
  result.loc = v.get_loc();
  result.type = token_type::string;

  v.adv(1);

  while (v.at(0) != '"' || v.at(-1) == '\\') {
    result.s += v.at(0);
    v.adv(1);
  }

  v.adv(1);

  return result;
}

token get_identifier(view::view &v) {
  token result;
  result.loc = v.get_loc();
  result.type = token_type::identifier;

  while (is_word_char(v.at(0))) {
    result.s += v.at(0);
    v.adv(1);
  }

  return result;
}

token get_symbol(view::view &v) {
  token result;
  result.loc = v.get_loc();
  result.type = token_type::symbol;

  result.s = v.at(0);
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
      while (v.at(0) != '*' || v.at(1) != '/') v.adv(1);
      v.adv(2);
    }
    else if (is_digit(c) || ((c == '+' || c == '-') && is_digit(v.at(1))))
      result.push_back(get_number(v));
    else if (c == '"')
      result.push_back(get_string(v));
    else if (is_word_char(c))
      result.push_back(get_identifier(v));
    else
      result.push_back(get_symbol(v));
  }

  return result;
}

}
