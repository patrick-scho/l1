#ifndef ALLOC__H
#define ALLOC__H

void* a_malloc(size_t size);

void* a_realloc(void* block, size_t size);

void a_free(void* block);

#endif