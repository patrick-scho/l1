#include "array.h"

#include "alloc.h"

const int N = 3;

void* array_new(size_t size, size_t num) {
  void* ptr = (char*)a_malloc(size * num + (N * sizeof(int)));
  int* ip = (int*)ptr;
  ip[0] = size;
  ip[1] = 0;
  ip[2] = num;
  void* result = (char*)ptr + (N * sizeof(int));
  int a = array_get_size(result);
  return result;
}

void* array_get_ptr(void* array) {
  return (char*)array - (N * sizeof(int));
}

int array_get_size(void* array) {
  int* ptr = array_get_ptr(array);
  return ptr[0];
}

int array_get_length(void* array) {
  int* ptr = array_get_ptr(array);
  return ptr[1];
}

int array_get_capacity(void* array) {
  int* ptr = array_get_ptr(array);
  return ptr[2];
}

void array_free(void* array) {
  a_free(array_get_ptr(array));
}

void mmm(void* a, void* b, size_t c) {
  for (int i = 0; i < c; i++)
    ((char*)a)[i] = ((char*)b)[i];
}

void* array_add(void* array, void* add) {
  void* result = array;
  int size = array_get_size(array);
  int length = array_get_length(array);
  int capacity = array_get_capacity(array);

  if (length == capacity) {
    void* ptr = array_get_ptr(array);
    ptr = a_realloc(ptr, size * capacity * 2);
    result = (char*)ptr + (N * sizeof(int));
  }

  void* dest = (char*)array + (length * size);
  memcpy(dest, &add, size);

  ((int*)array_get_ptr(array))[1]++;

  return result;
}

void* array_get(void* array, int i) {
  int size = array_get_size(array);
  return (char*)array + (i * size);
}
