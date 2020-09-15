#ifndef UTIL__H
#define UTIL__H

#include "source.h"

Source read_file(const char* filename);

void print(const char* fmt, ...);

long to_long(Source source);

void copy_str(char** dest, Source src);

#endif
