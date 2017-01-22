#include "system.h"

#include "util/vector.h"

vector_t* vector_new()
{
	vector_t *vector = (vector_t*)malloc(sizeof(vector_t)); 
	vector->capacity = VECTOR_CAPACITY;
	vector->size = 0;
	vector->data = malloc(sizeof(void *) * vector->capacity);
	return vector;
}

inline size_t vector_size(vector_t* vector)
{
	if (!vector) {
		return 0;
	}
	return vector->size;
}

size_t vector_capacity(vector_t* vector)
{
	if (!vector) {
		return 0;
	}
	return vector->capacity;
}

void vector_resize(vector_t* vector, size_t newSize)
{
	if (newSize > vector->capacity) {
		void **newData = realloc(vector->data, sizeof(void *) * newSize);
		if (newData) {
			vector->data = newData;
			vector->capacity = newSize;
		}
	} else if (newSize < vector->capacity) {
		free(vector->data + newSize*sizeof(void *));
		vector->capacity = newSize;
		if (vector->size > vector->capacity) {
			vector->size = vector->capacity;
		}
	}
}

inline void* vector_at(vector_t* vector, size_t index)
{
	if (index >= vector->size) {
		return NULL;
	}
	return vector->data[index];
}

void vector_push_back(vector_t* vector, void* data)
{
	if (vector->size == vector->capacity) {
		vector_resize(vector, vector->size * 2);
	}
	vector->data[vector->size] = data;
	++vector->size;
}

void vector_pop_back(vector_t* vector)
{
	free(vector->data[vector->size - 1]);
	--vector->size;
}

void vector_insert(vector_t* vector, size_t index, void* data)
{
	if (index >= vector->size) { // special case
		vector_push_back(vector, data);
	}
	if (vector->size == vector->capacity) {
		vector_resize(vector, vector->size * 2);
	}
	for (size_t i = vector->size; i >= index; --i) {
		vector->data[i] = vector->data[i - 1];
	}
	vector->data[index] = data;
	++vector->size;
}

void vector_erase(vector_t* vector, size_t index)
{
	if (index >= vector->size) { // special case
		vector_pop_back(vector);
	}
	free(vector->data[index]);
	for (size_t i = index; i < vector->size - 2; ++i) {
		vector->data[i] = vector->data[i + 1];
	}
	--vector->size;
}

void vector_clear(vector_t* vector)
{
	for (size_t i = 0; i < vector->size; ++i) {
		free(vector->data[i]);
	}
	vector->size = 0;
}

