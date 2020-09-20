#include "util.h"

#include <fstream>

#include <fmt/core.h>

string read_file(const string& filename) {
  ifstream ifs(filename, ios::in | ios::binary);
  ifs.seekg(0, ios::end);
  size_t size = ifs.tellg();
  ifs.seekg(0, ios::beg);
  string result;
  result.reserve(size);
  char *buffer = new char[size + 1];
  ifs.read(buffer, size);
  buffer[size] = 0;
  result.assign(buffer);
  delete[] buffer;
  return result;
}

void write_file(const string& filename, const string& file) {
  ofstream ofs(filename, ios::binary | ios::out);
  ofs.write(file.c_str(), file.size());
  ofs.close();
}

long to_long(Source source) {
  long result = 0;
  for (int i = 0; i < source.str.length(); i++) {
    result *= 10;
    result += source.get(i) - '0';
  }
  return result;
}
