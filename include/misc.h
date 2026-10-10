#ifndef MISC_H
#define MISC_H
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "vector.h"
// The misc file for MISCELLANEOUS functions that have no specific category.
int is_number(char* str);
Vector* split_str(char* str, char* a_delim);
#endif