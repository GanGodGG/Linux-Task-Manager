#include "misc.h"

int is_number(char* str){
    while(*str){ // checks if the string null
        if(!isdigit(*str))
            return 0; // return 0 if at least one character is not a digit
        str++; // loop through the string
    }
    return 1; // yo
}
Vector* split_str(char* str, char* a_delim){
    Vector* vec      = create_vec_cap(sizeof(char*), 512);
    char* str_copy = strdup(str); // !!doing copy!!
    if(!str_copy) return vec; // fuck
    char* token = strtok(str_copy, a_delim); // Get first token from delim
    while(token != NULL){
        char* token_copy = strdup(token);
        push_back(vec, &token_copy); // push token to the vector
        token = strtok(NULL, a_delim); // iterate next token
    }
    free(str_copy);
    return vec;
}