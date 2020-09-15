#include "source.h"

#include "parse.h"

void source_skip(Source* source, const char* str) {
  while (is_whitespace(source->data[source->index]))
    source->index++;
  if (source_cmp(*source, str)) {
    for (int i = 0; str[i] != 0; i++)
      source->index++;
    while (is_whitespace(source->data[source->index]))
      source->index++;
  }
}

bool source_cmp(Source source, const char* str) {
  for (int i = 0; str[i] != 0; i++) {
    if (source.data[source.index + i] != str[i])
      return false;
  }
  return true;
}

Source source_peek_token(Source source) {
  source_skip(&source, "");
  if (is_letter(source.data[source.index])) {
    Source result;
    result.data = source.data + source.index;
    result.index = 0;
    result.size = 1;

    while (is_word_char(source.data[source.index + result.size])) {
      result.size++;
    }

    return result;
  }
  else if (is_digit(source.data[source.index])) {
    Source result;
    result.data = source.data + source.index;
    result.index = 0;
    result.size = 1;

    while (is_digit(source.data[source.index + result.size])) {
      result.size++;
    }

    return result;
  }
  else if (source.data[source.index] == '"') {
    Source result;
    result.data = source.data + source.index;
    result.index = 0;
    result.size = 1;

    while (source.data[source.index + result.size] != '"') {
      result.size++;
    }
    result.size++;

    return result;
  }
  else {
    Source result;
    result.data = source.data + source.index;
    result.index = 0;
    result.size = 1;

    return result;
  }
}

Source source_peek_token_n(Source source, int n) {
  Source result;
  for (int i = 0; i < n; i++) {
    result = source_get_token(&source);
  }
  return result;
}

Source source_get_token(Source* source) {
  source_skip(source, "");
  Source result = source_peek_token(*source);
  source->index += result.size;
  return result;
}