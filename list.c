#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct List {
	int data;
	struct List *next;
} List;

List *list_new(int elem) {
	List *l = (List *)calloc(1, sizeof(List));
	assert(l != NULL);
	l->data = elem;
	l->next = NULL;
	return l;
}

List *list_delete(List *l) {
	if (l == NULL)
		return NULL;
	while (l->next != NULL) {
		List *prev = l;
		l = l->next;
		free(prev);
	}
	free(l);
	return NULL;
}

List *list_insert(List *l, int elem) {
	if (l == NULL)
		return NULL;
	List *save_node = l;
	while (l->next != NULL)
		l = l->next;
	l->next = list_new(elem);
	l = save_node;
	return save_node;
}

List *list_find(List *l, int elem) {
	if (l == NULL)
		return NULL;
	while (true) {
		if (l->data == elem)
			return l;
		
		if (l->next != NULL)
			l = l->next;
		else
			break;
	}
	return NULL;
}

List *list_erase(List *l, int elem) {
	if (l == NULL)
		return NULL;
	List *save_node = l;
	if (l->data == elem) {
		l = l->next;		
		free(save_node);
		return l;
	}
	while (l->next != NULL) {
		if (l->next->data == elem) {
			List *save_next_next = l->next->next;
			free(l->next);
			l->next = save_next_next;
			return save_node;
		}
		l = l->next;
	}
	return save_node;
}

List *list_insert_after(List *l, List *after, List *what) {
	if (l == NULL || after == NULL || what == NULL)
		return l;
	List *save_node = l;
	while (true) {
		if (after == l) {
			what->next = l->next;
			l->next = what;
			return save_node;
		}
		if (l->next != NULL)
			l = l->next;
		else
			break;
	}
	return NULL;
}

List *list_insert_before(List *l, List *whire, List *what) {
	if (whire == NULL || what == NULL)
		return l;
	if (l == NULL)
		return what;
	if (whire == l) {
        what->next = whire;
        return what;
    }
	List *save_node = l;
	while (l->next != NULL) {
		if (whire == l->next) {
			what->next = l->next;
			l->next = what;
			return save_node;
		}
		l = l->next;
	}
	return NULL;
}

void list_print(const List *l) {
    printf("[");
    if (l != NULL) {
        while (l->next != NULL) {
            printf("%d, ", l->data);
            l = l->next;
        }
        printf("%d", l->data);
    }
    printf("]\n");
}

int main() {
    List *l = list_new(0);
    l = list_insert(l, 1);
    l = list_insert(l, 2);
    l = list_insert(l, 4);
    l = list_erase(l, 4);
    l = list_erase(l, 2);

    l = list_insert(l, 2);
    l = list_insert(l, 2);
    list_print(l);
    l = list_insert_before(l, list_find(l, 2), list_new(666));

    l = list_delete(l);
    list_print(l);
}