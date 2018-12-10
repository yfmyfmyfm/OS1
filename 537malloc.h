#ifndef _537malloc_H_
#define _537malloc_H_
#include "range_tree.h"

void printEverything();


void *malloc537(int size);
void free537(void *ptr);
void *realloc537(void *ptr, int size);
void memcheck537(void *ptr, int size);

#endif
