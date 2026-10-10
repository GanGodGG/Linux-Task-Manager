#ifndef VEC_H
#define VEC_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
    void* data; // Pointer to the data stored in the vector
    size_t size; // Current number of elements in the vector
    size_t capacity; // Current capacity of the vector
    size_t data_size;
} Vector;
void push_back(Vector* vec, void* data);
void* get_from_vec(Vector* vec, size_t id);
Vector* create_vec(size_t _data_size);
Vector* create_vec_cap(size_t _data_size, size_t capacity);
void destroy_vec_all(Vector* vec, void(*free_func)(void*));
void destroy_vec(Vector* vec);
#endif