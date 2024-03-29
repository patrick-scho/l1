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

  view(const view &v) {
    this->str = v.str;
    this->loc = v.loc;
    this->len = v.len;
    this->repr = v.repr;
    this->owned_str = v.owned_str;
    
    if (v.str == &v.owned_str)
      this->str = &this->owned_str;
  }

  view& operator = (const view &v) {
    this->str = v.str;
    this->loc = v.loc;
    this->len = v.len;
    this->repr = v.repr;
    this->owned_str = v.owned_str;
    
    if (v.str == &v.owned_str)
      this->str = &this->owned_str;

    return *this;
  }

  view(const u32string &str) {
    this->owned_str = str;
    loc.filename = "";
    this->str = &this->owned_str;
    len = str.size();
    repr = to_str();
  }

  view(const string &filename, const u32string * str) {
    loc.filename = filename;
    this->str = str;
    len = str->size();
    repr = to_str();
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
    result.repr = result.to_str();
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
        loc.column = 1;
        loc.line++;
      }
    }
    repr = to_str();
  }

  view peek_skip() {
    view result = *this;
    result.skip();
    return result;
  }
  void skip() {
    while (len > 0 && is_whitespace(at(0))) adv(1);
  }

  const string to_str() {
    string result;
    for (int i = 0; i < len; i++) {
      result += str->at(loc.index + i);
    }
    return result;
  }
private:
  const u32string * str = nullptr;
  location loc;
  s64 len = 0;

  string repr;
  u32string owned_str;
};

}
