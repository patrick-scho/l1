#pragma once

#include <stdexcept>

#include <fmt/core.h>

#include "source.h"

using namespace std;

#define ERROR(expr, ...) std::runtime_error{ \
fmt::format("[{}:{},{}] {}", (expr)->location.filename, (expr)->location.line, (expr)->location.col, \
fmt::format(__VA_ARGS__))}
//#define ERROR(...) std::runtime_error{fmt::format(__VA_ARGS__)}

string read_file(string filename);

long to_long(Source source);

