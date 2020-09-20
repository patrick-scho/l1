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
