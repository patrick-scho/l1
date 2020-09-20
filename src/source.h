#pragma once

#include <string_view>

using namespace std;

struct Source {
  struct Location {
    string filename;
    int line = 1, col = 1;
  };
  string_view str;

  Location location;

  char get(size_t n = 0);
  void adv(size_t n);

  void skip(string_view str = "");
  bool cmp(string_view str);
  Source peekToken();
  Source peekToken(int n);
  Source getToken();
  Source getToken(int n);
};

