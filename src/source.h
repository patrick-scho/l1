#pragma once

#include <string_view>

using namespace std;

class Source {
public:
  string_view str;

  char get(int n = 0);
  void adv(int n);

  void skip(string_view str = "");
  bool cmp(string_view str);
  Source peekToken();
  Source peekToken(int n);
  Source getToken();
};


