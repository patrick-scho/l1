#pragma once

#include <stdexcept>

#include <fmt/core.h>

#include "source.h"

using namespace std;

#define ERROR(loc, ...)                                                       \
  std::runtime_error {                                                         \
    fmt::format("[{}:{},{}] {}", loc.filename,                    \
                loc.line, loc.col,                   \
                fmt::format(__VA_ARGS__))                                      \
  }

string read_file(string filename);

long to_long(Source source);

namespace lists {

template<typename T, typename F>
bool find(T& ts, F f) {
  for (auto& t: ts) {
    if (f(t))
      return true;
  }
  return false;
}

template<typename T, typename F>
int count(T& ts, F f) {
  int result = 0;
  for (auto& t: ts) {
    if (f(t))
      result++;
  }
  return result;
}

}