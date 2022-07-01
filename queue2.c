#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

const size_t QUEUE_BEGINNING_CAPACITY = 100;

typedef struct {
	void *data;
	size_t elem_size;
	size_t capacity;
	size_t used;
	size_t tail;
	size_t front;
} Queue;

Queue *queue_new(const size_t elem_size) {
	Queue *q = calloc(1, sizeof(Queue));
	assert(q != NULL);
	q->data = (char *)calloc(QUEUE_BEGINNING_CAPACITY, sizeof(char));
	assert(q->data != NULL);
	q->elem_size = elem_size;
	q->capacity = QUEUE_BEGINNING_CAPACITY;
	q->used  = 0;
	q->tail  = 0;
	q->front = 0;
	return q;
}

Queue *queue_delete(Queue *q) {
	assert(q != NULL);
	free(q->data);
	free(q);
	return NULL;
}

int queue_push(Queue *const q, const void *elem) {
	assert(q != NULL);
	if (q->capacity - 1 != q->used) {
		if (memcpy((char *)q->data + (q->tail * q->elem_size), elem, q->elem_size) == 0)
			return 1;
		q->tail = (q->tail + 1) % q->capacity;
		q->used++;
		return 0;
	}
	size_t last_capacity = q->capacity;
	q->capacity *= 2;
	q->data = realloc((char *)q->data, q->capacity * q->elem_size);

	if (q->data == NULL)
		return 1;
	if (q->tail - q->front == last_capacity - 1) {
		if (memcpy((char *)q->data + (q->tail++ * q->elem_size), elem, q->elem_size) == 0)
			return 1;
		q->used++;
		return 0;
	}

	if (q->tail == 0) {
		q->tail = last_capacity;
		if (memcpy((char *)q->data + (q->tail * q->elem_size), elem, q->elem_size) == 0)
			return 1;
		q->used++;
		q->tail++;
		return 0;
	}

	if (q->front >= last_capacity) {
		if (
			memcpy(
				(char *)q->data + (q->front + q->capacity - last_capacity) * q->elem_size,
				(char *)q->data +  q->front                                * q->elem_size,
				(last_capacity  -  q->front)                               * q->elem_size
			) == 0)
			return 1;
		if (memcpy((char *)q->data + q->tail* q->elem_size, elem, q->elem_size) == 0)
			return 1;
		q->tail++;
		q->used++;
		q->front = q->capacity - last_capacity + q->front;
		return 0;
	}

	if (memcpy((char *)q->data + last_capacity * q->elem_size, q->data, q->tail * q->elem_size) == 0)
		return 1;
	if (memcpy((char *)q->data + (last_capacity + q->tail) * q->elem_size, elem, q->elem_size) == 0)
		return 1;
	q->tail = last_capacity + q->tail + 1;
	q->used++;
	return 0;
}

int queue_pop(Queue *const q, void *const elem) {
	if (q->used == 0)
		return 1;
	if (!memcpy(elem, (char *)q->data + (q->front * q->elem_size), q->elem_size))
		return 1;
	q->used--;
	q->front = (q->front + 1) % q->capacity;
	if (q->used == 0) {
		q->front = 0;
		q->tail = 0;
	}
	return 0;
}

int queue_empty(const Queue *q) {
	assert(q != NULL);
	if (q == NULL || q->used == 0)
		return 1;
	return 0;
}

void queue_print(const Queue *q, void (*pf) (const void *data)) {
	assert(q != NULL);
	printf("[");
	for(size_t index = 0; index < q->used; index++) {
		pf((char *)q->data + ((q->tail - 1 - index) % q->capacity)* q->elem_size);
		if (index != q->used - 1)
			printf(", ");
	}
	printf("]\n");
}

static void print_double(const void *q) {
   printf("%lg", *(double *)q);
}

int main() {
	Queue *q = queue_new(sizeof(double));
	for (size_t i = 0; i < 10; i++) {
		double tmp = i;
		queue_push(q, &tmp);
	}
	queue_print(q, print_double);
	while (!queue_empty(q)) {
		double tmp;
		queue_pop(q, &tmp);
		printf("%lf\n", tmp);
	}
	q = queue_delete(q);
}