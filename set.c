/**
 * @page Set
 * Set structure
 * 
 * The structure of the set is needed to
 * quickly determine the presence of certain
 * numerical values in the information flow
 * @see {@link SetLib} library
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef size_t set_t;
const size_t PACK_SIZE = sizeof(set_t) * 8;
const size_t FACTOR = 6;

/**
 * @addtogroup SetLib
 * @{
 	* @emoji :speech_balloon:
 	* The set provides several functions that
 	* provide a standard abstraction.
 	* 
 	* @arg {@link setCtor}
 	* @arg {@link setDtor}
 	* @arg {@link insert}
 	* @arg {@link erase}
 	* @arg {@link find}
 * @brief Set library group
 */

/**
 * @brief main <em> enumeration of stack </em>
 * Each status refers to a specific function
 * with the same name
 */
typedef enum {
	noErr,
	insertErr,
	eraseErr,
	findErr,

	sizeErr,
	emptyErr,

	firstErr,
	printErr,
}SetStatus;

/**
 * @brief main @em structure of set
 * main characteristics
 * @arg Set does not contain the number of numbers
 * @arg Set shows only the presence of value in it
 */
typedef struct {
	void *data;
	size_t cardinality;
	size_t size;
} Set;

/**
 * @brief constructor of Set
 * @arg cardinality capacity of new Set.
 */
Set *setCtor(size_t cardinality) {
	Set *s = (Set *)calloc(1, sizeof(Set));
	assert(s != NULL);
	s->data = calloc(cardinality / PACK_SIZE + 1, sizeof(set_t));
	if (s->data == NULL) {
		free(s);
		return NULL;
	}
	s->cardinality = cardinality;
	return s;
}

/**
 * @brief set destructor
 * @param s <b>{@link Set} structure </b>
 */
Set *setDtor(Set *s) {
	if (s == NULL)
		return NULL;
	free(s->data);
	free(s);
	return NULL;
}

/**
 * @brief insert function to add <p> new value in set</p>
 * @copydetails setDtor
 */
SetStatus insert(Set *s, set_t elem) {
	if (elem > s->cardinality)
		return insertErr;
	size_t neededBlock = elem >> FACTOR;
	set_t neededBitInBlock = elem - (neededBlock << FACTOR);
	*((set_t *)s->data + neededBlock) |= (set_t)1 << neededBitInBlock;
	s->size++;
	return noErr;
}
/**
 * @}
 */

SetStatus erase(Set *s, size_t elem) {
	if (elem > s->cardinality)
		return eraseErr;
	size_t neededBlock = elem >> FACTOR;
	set_t neededBitInBlock = elem - (neededBlock << FACTOR);
	*((set_t *)s->data + neededBlock) &= UINT64_MAX - ((set_t)1 << neededBitInBlock);
	s->size--;
	return noErr;
}

bool find(Set *s, size_t elem) {
	if (elem > s->cardinality)
		return 0;
	return *((set_t *)s->data + (elem / PACK_SIZE)) & (1 << (elem % PACK_SIZE));
}

static set_t mylog(size_t elem) {
	for (size_t degree = 0; degree < PACK_SIZE; degree++)
		if ((size_t)1 << degree == elem)
			return degree;
	return 0;
}

ssize_t set_findfirst(Set *s, size_t elem) {
	size_t originPackNumber = elem >> 6;
	size_t mod  = elem - (originPackNumber << 6);
	size_t byteToCheck = (*((set_t *)s->data + originPackNumber) >> mod) << mod;
	size_t theleast = (byteToCheck ^ (byteToCheck - 1)) & byteToCheck;
	if (theleast != 0)
		return originPackNumber + mylog(theleast);
	for (size_t *pack = (set_t *)s->data + originPackNumber + 1; pack < (set_t *)s->data + s->cardinality / PACK_SIZE + 1; pack++)
		if ((*pack ^ (*pack - 1)) & *pack)
			return ((set_t)((pack - ((set_t *)s->data + originPackNumber)) + originPackNumber) << 6) + mylog((*pack ^ (*pack - 1)) & *pack);
	return -1;
}

SetStatus print(Set *s) {
	if (s == NULL)
		return printErr;
	printf("[");
	if (s->size != 0) {
		bool flag = 0;
		for (size_t index = 1; index < s->cardinality; index++) {
			if (*((set_t *)s->data + (index / PACK_SIZE)) & ((set_t)1 << (index % PACK_SIZE))) {
				if (flag)
					printf(", ");
				printf("%llu", index);
				if (!flag)
					flag = true;
			}
		}
	}
	printf("]");
	return noErr;
}

int main() {
    Set *s = setCtor(10);
    assert(insert(s, 1) == 0);
    assert(insert(s, 2) == 0);
    assert(insert(s, 100) != 0);
    assert(find(s,1) != 0);
    assert(find(s,100) == 0);
    assert(insert(s, 5) == 0);
    print(s);
    assert(erase(s,2) == 0);
    assert(set_findfirst(s, 2) == 5);
    assert(set_findfirst(s, 10) == -1);
    assert(setDtor(s) == NULL);
}