#ifndef UTIL_VECTOR_H_INCLUDED
#define UTIL_VECTOR_H_INCLUDED

/*----------------------------------------------------------------------------
--  Includes
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
-- Defines
----------------------------------------------------------------------------*/

#define VECTOR_CAPACITY 4

/*----------------------------------------------------------------------------
-- Types
----------------------------------------------------------------------------*/

typedef struct vector_s {
	void *data;
	size_t capacity;
	size_t size;
	size_t elemSize;
} vector_t;

/*----------------------------------------------------------------------------
--  Functions
----------------------------------------------------------------------------*/

vector_t* vector_new(size_t elemSize);

#define vector_at(_vector, _index) ((void*)((unsigned char*)((_vector)->data) + (_index)*((_vector)->elemSize)))
#define vector_get(_vector, _index, _type) ((_type)(*(_type*)vector_at(_vector, _index)))

//#define vector_get(vector, index, type) ((type*)vector_at(vector, index) == NULL ? NULL : (type)(*(type*)vector_at(vector, index)))

inline size_t vector_size(vector_t *vector);
size_t vector_capacity(vector_t *vector);
void vector_resize(vector_t *vector, size_t newSize);

void vector_push_back(vector_t *vector, void *data);
void vector_pop_back(vector_t *vector);
void vector_insert(vector_t *vector, size_t index, void *data);
void vector_erase(vector_t *vector, size_t index);
void vector_clear(vector_t *vector);

#endif // UTIL_VECTOR_H_INCLUDED
