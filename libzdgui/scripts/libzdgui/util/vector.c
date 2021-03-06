#include "system.h"

#include "util/vector.h"

vector_t *vector_new(size_t elemSize)
{
	vector_t *vector = new (vector_t);
	vector->capacity = VECTOR_CAPACITY;
	vector->size = 0;
	vector->elemSize = elemSize;
	vector->data = malloc(elemSize * vector->capacity);
	return vector;
}

void vector_delete(vector_t *vector)
{
	if (vector) {
		free(vector->data);
		free(vector);
	}
}

size_t vector_capacity(vector_t *vector)
{
	if (!vector) {
		return 0;
	}
	return vector->capacity;
}

void vector_resize(vector_t *vector, size_t newSize)
{
	if (newSize > vector->capacity) {
		void **newData = realloc(vector->data, vector->elemSize * newSize);
		if (newData) {
			vector->data = newData;
			vector->capacity = newSize;
		}
	} else if (newSize < vector->capacity) {
		free(&(vector->data) + newSize * vector->elemSize);
		vector->capacity = newSize;
		if (vector->size > vector->capacity) {
			vector->size = vector->capacity;
		}
	}
}

void vector_push_back(vector_t *vector, void *data)
{
	if (vector->size == vector->capacity) {
		vector_resize(vector, vector->size * 2);
	}
	memcpy(vector_at(vector, vector->size), data, vector->elemSize);
	++vector->size;
}

void vector_pop_back(vector_t *vector)
{
	if (!vector->size) {
		guiError("vector out of range");
		return;
	}
	//memset(vector_at(vector, vector->size - 1), 0, vector->elemSize);
	--vector->size;
}

void vector_insert(vector_t *vector, size_t index, void *data)
{
	if (index >= vector->size) { // special case
		vector_push_back(vector, data);
	}
	if (vector->size == vector->capacity) {
		vector_resize(vector, vector->size * 2);
	}
	for (size_t i = vector->size; i >= index; --i) {
		memcpy(vector_at(vector, i), vector_at(vector, i - 1), vector->elemSize);
	}
	memcpy(vector_at(vector, index), data, vector->elemSize);
	++vector->size;
}

void vector_erase(vector_t *vector, size_t index)
{
	if (index >= vector->size) { // special case
		vector_pop_back(vector);
	}
	for (size_t i = index; i < vector->size - 2; ++i) {
		memcpy(vector_at(vector, i), vector_at(vector, i + 1), vector->elemSize);
	}
	--vector->size;
}

void vector_clear(vector_t *vector)
{
	vector->size = 0;
}

