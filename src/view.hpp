#pragma once

#include <string>

#include "util.hpp"

using namespace std;

namespace view {

struct location {
  string filename = "";
  u64 index = 0, line = 1, column = 1;
};

bool is_whitespace(char32_t c) {
  return
    c == ' '  ||
    c == '\t' ||
    c == '\n';
}

struct view {
  view() { }

  view(const string &filename, const u32string * str) {
    loc.filename = filename;
    this->str = str;
    len = str->size();
  }

  s64 length() {
    return this->len;
  }
  location get_loc() {
    return loc;
  }
  char32_t at(s64 i) {
    if (loc.index + i < 0)
      util::crash(loc, "Trying to access index < 0");
    if (i >= len)
      util::crash(loc, "Trying to access index > length");
    return str->at(loc.index + i);
  }
  view sub(s64 off, u64 len) {
    if (loc.index + off < 0)
      util::crash(loc, "Trying to sub index < 0");
    if (off + len >= this->len)
      util::crash(loc, "Trying to sub index > length");
    view result = *this;
    result.adv(off);
    result.len = len;
    return result;
  }
  bool end() {
    bool result = peek_skip().length() <= 0;
    return result;
  }
  void adv(s64 n) {
    if (n > len)
      util::crash(loc, "Trying to advance past end of view");
    for (s64 i = 0; i < n; i++) {
      loc.index++;
      loc.column++;
      len--;
      if (loc.index > 0 && (i-1) < len && at(i - 1) == '\n') {
        loc.column = 0;
        loc.line++;
      }
    }
  }

  view peek_skip() {
    view result = *this;
    result.skip();
    return result;
  }
  void skip() {
    while (len > 0 && is_whitespace(at(0))) adv(1);
  }
private:
  const u32string * str = nullptr;
  location loc;
  s64 len = 0;
};

}
