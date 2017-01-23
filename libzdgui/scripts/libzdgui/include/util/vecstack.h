#ifndef VECSTACK_H_INCLUDED
#define VECSTACK_H_INCLUDED

#include "util/vector.h"

typedef vector_t vecstack_t;

#define vecstack_init(elemSize) vector_new(elemSize)
#define vecstack_push(vecstack, element) vector_push_back(vecstack, element)
#define vecstack_pop(vecstack) vector_pop_back(vecstack)
#define vecstack_top(vecstack) vector_at(vecstack, vector_size(vecstack) - 1)
#define vecstack_size(vecstack) vector_size(vecstack)

#endif // VECSTACK_H_INCLUDED