#pragma once

#include <stdexcept>
#include <fstream>

#include <fmt/core.h>

using namespace std;

using s8  = int8_t;
using s16 = int16_t;
using s32 = int32_t;
using s64 = int64_t;

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

namespace util {

template<typename T, typename ... Args>
void crash(T t, Args... args) {
  throw std::runtime_error {
    fmt::format("[{}:{},{}] {}", t.filename, t.line, t.column, fmt::format(args...))
  };
}

#define ERROR(f, l, c, ...) \
  std::runtime_error { \
    fmt::format("({}:{})[{}:{},{}] {}", __FILE__, __LINE__, f, l, c, \
                fmt::format(__VA_ARGS__)) \
  }

u32string read_file(const string& filename) {
  ifstream ifs (filename, ios::binary);

  u32string result;
  char c;
  while (ifs.get(c)) {
    if ((c & 0b10000000) == 0b00000000) { // ascii
      if (c != '\r') result += c;
    } else
    if ((c & 0b11000000) == 0b11000000) { // start
        if ((c & 0b11100000) == 0b11000000) {
          char32_t nc = (c & 0b00011111);
          for (int i = 0; i < 1; i++)
            nc = (nc << 6) + (ifs.get() & 0b00111111);
          result += nc;
        } else
        if ((c & 0b11110000) == 0b11100000) {
          char32_t nc = (c & 0b00001111);
          for (int i = 0; i < 2; i++)
            nc = (nc << 6) + (ifs.get() & 0b00111111);
          result += nc;
        } else
        if ((c & 0b11111000) == 0b11110000) {
          char32_t nc = (c & 0b00000111);
          for (int i = 0; i < 3; i++)
            nc = (nc << 6) + (ifs.get() & 0b00111111);
          result += nc;
        }
    }
  }

  return result;
}

void write_file(const string& filename, const string& file) {
  ofstream ofs(filename, ios::out | ios::binary);
  ofs.write(file.c_str(), file.size());
  ofs.close();
}

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