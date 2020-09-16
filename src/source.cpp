#include "source.h"

#include "parse.h"

char Source::get(int n) {
  return str[n];
}
void Source::adv(int n) {
  str = str.substr(n);
}

void Source::skip(string_view str) {
  while (is_whitespace(get()))
    adv(1);
  if (cmp(str)) {
    adv(str.length());
    while (is_whitespace(get()))
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
  }
  else if (is_digit(get())) {
    int len = 1;
    while (is_digit(get(len))) {
      len++;
    }
    result.str = str.substr(0, len);

    return result;
  }
  else if (get() == '"') {
    int len = 1;
    while (get(len) != '"') {
      len++;
    }
    result.str = str.substr(0, len + 1);

    return result;
  }
  else {
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