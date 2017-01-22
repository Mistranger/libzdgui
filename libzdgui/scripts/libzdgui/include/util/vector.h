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
	void **data;
	size_t capacity;
	size_t size;
} vector_t;

/*----------------------------------------------------------------------------
--  Functions
----------------------------------------------------------------------------*/

vector_t* vector_new();

#define vector_get(vector, index, type) ((type*)vector_at(vector, index) == NULL ? NULL : (type)(*(type*)vector_at(vector, index)))
inline void* vector_at(vector_t *vector, size_t index);

inline size_t vector_size(vector_t *vector);
size_t vector_capacity(vector_t *vector);
void vector_resize(vector_t *vector, size_t newSize);

void vector_push_back(vector_t *vector, void *data);
void vector_pop_back(vector_t *vector);
void vector_insert(vector_t *vector, size_t index, void *data);
void vector_erase(vector_t *vector, size_t index);
void vector_clear(vector_t *vector);

#endif // UTIL_VECTOR_H_INCLUDED
