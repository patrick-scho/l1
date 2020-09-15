#include "alloc.h"

#include <stdlib.h>

void* a_malloc(size_t size) {
  return malloc(size);
}

void* a_realloc(void* block, size_t size) {
  return realloc(block, size);
}

void a_free(void* block) {
  free(block);
}