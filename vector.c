/**
 * @page vector library
 * @brief implementation of simple structure vector.
 * @addtogroup VectorLib
 * @brief implementation of simple structure vector
 * 
 * @{
 	* In contrast to {@link stackLib} in <b>vector<b>
 	* you can control all elements in it.
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// @brief used as coefficient in {@link vector_resize} to resize vector capacity
const size_t INCREASE_FACTOR = 2;

/**
 * @brief @struct Vector structure of vector
 * @param data <em> elements in vector </em>
 * @param elem_size size of element in vector-example
 * @param capacity max size of size in particular moment
 * @param size current size of vector
 */
typedef struct {
	void *data;
	size_t elem_size;
	size_t capacity;
	size_t size;
} Vector;

/**
 * @brief Creates new {@link Vector} \a structure.
 * @param elems start capacity of vector
 * @param elem_size size of element in vector
 * @returns {@link Vector}
 */
Vector *vector_new(size_t elems, size_t elem_size) {
	Vector *v = (Vector *)calloc(1, sizeof(Vector));
	assert(v != NULL);
	v->elem_size = elem_size;
	v->size = elems;
	v->data = calloc(elems, elem_size);
	assert(v->data);
	v->capacity = elems;
	return v;
}

Vector *vector_delete(Vector *v) {
	free(v->data);
	free(v);
	return NULL;
}

static int vector_resize(Vector *v, const size_t new_size) {
	void *try_alloc = realloc(v->data, new_size * v->elem_size);
	if (try_alloc == NULL)
		return 1;
	v->data = try_alloc;
	v->capacity = new_size;
	return 0;
}

int vector_push(Vector *v, const void *elem) {
	memcpy((char *)v->data + v->size++ * v->elem_size, elem ,v->elem_size);
	if (v->size == v->capacity)
		vector_resize(v, INCREASE_FACTOR * v->capacity + 1);
	return 0;
}

int vector_pop(Vector *v, void *elem) {
	if (v->size == 0)
		return 1;
	memcpy(elem, (char *)v->data + --v->size * v->elem_size, v->elem_size);
	if (v->size >= v->capacity / 4)
		vector_resize(v, v->size);
	return 0;
}

int vector_empty(Vector *v) {
	return (v->size == 0);
}

int vector_print(Vector *v, void (*printfunction)(const void *data)) {
	printf("[");
	if (v->size) {
		for (size_t index = 0; index < v->size - 1; index++) {
			printfunction((char *)v->data + index * v->elem_size);
			printf(", ");
		}
		printfunction((char *)v->data + v->elem_size * (v->size - 1));
	}
	printf("]\n");
	return 0;
}

/**
 * @brief set value in vector to value \a index
 * @param[in] index coordinate in vector data of particular element
 * @param[in] elem new value.
 * @param[out] v
 * 
 * @retval 0 - succsess
 * @retval 1 - error 
 */
int vector_set(Vector *v, size_t index, void *elem) {
	if (index >= v->capacity)
		return 1;
	memcpy((char *)v->data + index * v->elem_size, elem, v->elem_size);
	return 0;
}

/**
 * @brief get value from vector
 * @param[in] index coordinate in vector data of particular element
 * @param[out] elem new value.
 * @param[out] v
 * 
 * @retval 0 - succsess
 * @retval 1 - error 
 */
int vector_get(Vector *v, size_t index, void *elem) {
	if (v->size >= v->capacity)
		return 1;
	memcpy(elem, (char *)v->data + index * v->elem_size, v->elem_size);
	return 0;
}
///@}

static void print_int(void const *data) {
   printf("%d", *(int *)data);
}

int main() {
	Vector *v = vector_new(10, sizeof(int));
	for (int index = 0; index < 10; index++) {
	    vector_set(v, index, &index);
	}
	vector_print(v, print_int);
	v = vector_delete(v);
}