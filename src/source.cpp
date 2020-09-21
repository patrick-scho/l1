#include "source.h"

#include "parse.h"
#include "util.h"

char Source::get(size_t n) {
  return str[n];
}
void Source::adv(size_t n) {
  for (int i = 0; i < n; i++) {
    location.index++;
    location.col++;
    if (str[i] == '\n')
      location.line++;
    if (str[i] == '\n' || str[i] == '\r')
      location.col = 0;
  }
  str = str.substr(n);
}

void Source::skip(string_view str) {
  while (this->str.size() > 0 && is_whitespace(get()))
    adv(1);
  if (cmp(str)) {
    adv(str.length());
    while (this->str.size() > 0 && is_whitespace(get()))
      adv(1);
  }
}

bool Source::cmp(string_view str) {
  for (int i = 0; i < str.length(); i++) {
    if (this->str[i] != str[i])
      return false;
  }
  return true;
}

Source Source::peekToken() {
  skip();
  Source result;
  if (is_letter(get())) {
    int len = 1;
    while (is_word_char(get(len))) {
      len++;
    }
    result.str = str.substr(0, len);

    return result;
  } else if (is_digit(get())) {
    int len = 1;
    while (is_digit(get(len))) {
      len++;
    }
    result.str = str.substr(0, len);

    return result;
  } else if (get() == '"') {
    int len = 1;
    while (get(len) != '"' || get(len - 1) == '\\') {
      len++;
    }
    result.str = str.substr(0, len + 1);

    return result;
  } else {
    result.str = str.substr(0, 1);

    return result;
  }
}

Source Source::peekToken(int n) {
  Source copy = *this;
  Source result;
  for (int i = 0; i < n; i++) {
    result = copy.getToken();
  }
  return result;
}

Source Source::getToken() {
  skip();
  Source result = peekToken();
  adv(result.str.length());
  return result;
}

Source Source::getToken(int n) {
  for (int i = 0; i < n - 1; i++)
    getToken();
  return getToken();
}
