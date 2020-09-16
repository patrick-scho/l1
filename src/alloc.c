#include "alloc.h"

#include <stdlib.h>
#include <stdio.h>

void* a_malloc(size_t size) {
  void* result = malloc(size);
  if (result == NULL)
    puts("");
  return result;
}

void* a_realloc(void* block, size_t size) {
  void* result = realloc(block, size);
  if (result == NULL)
    puts("");
  return result;
}

void a_free(void* block) {
  free(block);
}