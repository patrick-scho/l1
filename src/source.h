#ifndef SOURCE__H
#define SOURCE__H

#include <stdbool.h>

typedef struct Source {
  char* data;
  long size, index;
} Source;

void source_skip(Source* source, const char* str);
bool source_cmp(Source source, const char* str);
Source source_peek_token(Source source);
Source source_peek_token_n(Source source, int n);
Source source_get_token(Source* source);

#endif
