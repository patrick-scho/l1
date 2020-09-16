#include "util.h"

#include <fstream>

string read_file(string filename) {
  ifstream ifs(filename, ios::in | ios::binary);
  ifs.seekg(0, ios::end);
  size_t size = ifs.tellg();
  ifs.seekg(0, ios::beg);
  string result;
  result.reserve(size);
  char* buffer = new char[size + 1];
  ifs.read(buffer, size);
  buffer[size] = 0;
  result.assign(buffer);
  delete[] buffer;
  return result;
}

long to_long(Source source) {
  long result = 0;
  for (int i = 0; i < source.str.length(); i++) {
    result *= 10;
    result += source.get(i) - '0';
  }
  return result;
}
