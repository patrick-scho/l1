#ifndef ARRAY__H
#define ARRAY__H

#include <string.h>

#define ARRAY_FREE(arr, typ) \
  for (int i = 0; i < array_get_length(arr); i++) typ##_free(*(typ**)array_get(arr, i));

//#define ARRAY_FIND(var, cond) \

void* array_new(size_t size, size_t num);

void* array_get_ptr(void* array);

int array_get_size(void* array);

int array_get_length(void* array);

int array_get_capacity(void* array);

void array_free(void* array);

void* array_add(void* array, void* add);

void* array_get(void* array, int i);

void array_set(void* array, int i, void* set);

#endif