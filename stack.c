/**
 * @mainpage structures
 * 
 * This contains some basic data structures
 * that you will often need when working with
 * medium sized data.
 * 
 * - @subpage Stack
 * - @subpage Set
 */

/**
 * @page Stack
 * Simple stack structure.
 * the stack is needed to store not only
 * the data, but also their storage order
 * 
 * @see {@link stackLib} library
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static const size_t STACK_STARTING_CAPACITY = 32;
static const size_t STACK_EXPAND_FACTOR = 2;

/**
 * @addtogroup stackLib
 * @{
 * @param noErr 'OK' status of stack
 * @param error any error in execution process
 */
typedef enum {
	noErr,
	error,
} StackStatus;

/**
 * @brief main structure of stack
 * @param data elements in stack
 * @param elemSize size of element in stack
 * @param size current size of stack
 * @param capacity current capacity of stack
 */
typedef struct {
	void *data;
	size_t elemSize;
	size_t size;
	size_t capacity;
} Stack;

/**
 * @breif constructor of new stack
 * @oaram elemSize size of element in this stack
 * @see {@link stackDtor} function
 */
Stack *stackCtor(const size_t elemSize) {
	Stack *s = (Stack *)calloc(1, sizeof(Stack));
	assert(s != NULL);
	s->elemSize = elemSize;
	s->size = 0;
	s->data = (void *)calloc(STACK_STARTING_CAPACITY, sizeof(char));
	s->capacity = STACK_STARTING_CAPACITY;
	return s;
}

/**
 * @breif destructor of new stack
 * @oaram s stack example to destroy
 * @see {@link stackCtor} function
 */
Stack *stackDtor(Stack *s) {
	if (s == NULL)
		return s;
	free(s->data);
	free(s);
	return NULL;
}

/**
 * @brief function to push element in stack
 * @param s stack example
 * @param elem new element in stack
 * @see {@link stackPop} function
 */
StackStatus stackPush(Stack *s, const void *const elem) {
	if (s == NULL || elem == NULL)
		return error;
	if (s->size == s->capacity) {
		void *tryAlloc = (void *)realloc((char *)s->data, s->capacity * STACK_EXPAND_FACTOR * sizeof(s->elemSize));
		if (s->data == NULL)
			return error;
		s->data = tryAlloc;
		s->capacity *= STACK_EXPAND_FACTOR;
	}
	memcpy((char *)s->data + s->elemSize * s->size++, elem, s->elemSize);
	return noErr;
}

/**
 * @brief function to delete element from stack
 * @param s stack example
 * @param elem container to save old element
 * @see {@link stackPush} function
 */
StackStatus stackPop(Stack *s, void *elem) {
	if (s == NULL || elem == NULL || s->size == 0)
		return error;
	memcpy(elem, (char *)s->data + s->elemSize * s->size--, s->elemSize);
	if (s->size < s->capacity / (STACK_EXPAND_FACTOR * 3 / 2) )
		s->capacity /= (STACK_EXPAND_FACTOR * 3 / 2);
	return noErr;
}

/**
 * @breif determines if the stack is empty
 * @param s stack example
 * @return true if stack is empty
 */
bool stackEmpty(Stack *s) {
	return s->size == 0? false : true;
}

/**
 * @brief function to get top-element value
 * @param s stack example
 * @param elem container to save element's value in the top of the stack 
 */
StackStatus stackTop(Stack *s, void *elem) {
	if (s == NULL || elem == NULL || s->size == 0)
		return error;
	memcpy(elem, (char *)s->data + s->elemSize * s->size, s->elemSize);
	return noErr;
}

/**@} */

/**
 * @addtogroup auxilary
 * @{
 */
/**
 * @brief print function for stack memory
 * @param s {@link Stack} structure example
 * @param printFunction special print function that prints any element in stack by it's coordinate
 * @see {@link printInt} function
 */
StackStatus stackPrint(Stack *s, void (*printFunction)(const void *const elem)) {
	if (s == NULL || printFunction == NULL)
		return error;
	printf("[");
	if (s->size != 0) {
		for (size_t index = 0; index < (s->size - 1); index++) {
			printFunction((char *)s->data + s->elemSize * index);
			printf(", ");
		}
		printFunction((char *)s->data + s->elemSize * (s->size - 1));
	}
	printf("]");
	return noErr;
}

/**
 * @brief prints element of stack by it's address
 * @param elem ptr to element to print
 */
static void printInt(const void *const elem) {
	printf("%c", *(char *)elem);
}

/**@} */

int main() {
	Stack *s = stackCtor(1);
	char element = 'e';
	stackPush(s, &element);
	stackPrint(s, printInt);
	stackPop(s, &element);
	stackPrint(s, printInt);
	return 0;
}
