#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "alloc.h"

Source read_file(const char* filename) {
  FILE* f = fopen(filename, "rb+");

  fseek(f, 0, SEEK_END);
  long size = ftell(f);
  fseek(f, 0, SEEK_SET);

  Source result;
  result.index = 0;
  result.size = size;
  
  result.data = malloc(size + 1);
  fread_s(result.data, size + 1, 1, size, f);
  result.data[size] = 0;

  fclose(f);

  return result;
}

void print(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
}

long to_long(Source source) {
  long result = 0;
  for (int i = source.index; i < source.size; i++) {
    result *= 10;
    result += source.data[i] - '0';
  }
  return result;
}

void copy_str(char** dest, Source src) {
  *dest = a_malloc(src.size + 1);
  strncpy(*dest, src.data, src.size);
  (*dest)[src.size] = 0;
}