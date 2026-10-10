#include "vector.h"


void push_back(Vector* vec, void* data){
    if(vec->size == vec->capacity){
        vec->capacity *= 2;
        void* temp = vec->data;
        vec->data = realloc(temp, vec->capacity * vec->data_size);
    }
    memcpy((char*)vec->data + (vec->data_size * vec->size), data, vec->data_size);
    vec->size++;
}

void* get_from_vec(Vector* vec, size_t id){
    if(id < vec->size)
        return vec->data + (vec->data_size * id);
    perror("OOB");
    return NULL;
}

Vector* create_vec(size_t _data_size){
    Vector* temp = malloc(sizeof(Vector));
    temp->data_size = _data_size; // apply data size

    temp->capacity = 1;
    temp->size = 0;
    temp->data = malloc(temp->capacity * _data_size);
    return temp;
}
Vector* create_vec_cap(size_t _data_size, size_t capacity){
    Vector* temp = malloc(sizeof(Vector));
    temp->data_size = _data_size; // apply data size

    temp->capacity = capacity;
    temp->size = 0;
    temp->data = malloc(temp->capacity * _data_size);
    return temp;
}
void destroy_vec(Vector* vec){ 
    free(vec->data);
    free(vec);
}

void destroy_vec_all(Vector* vec, void(*free_func)(void*)){
    for(size_t i = 0; i < vec->size; ++i){
        free_func((char*)vec->data + vec->data_size * i);
    }
    destroy_vec(vec);
}