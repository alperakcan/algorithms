
#include <stdlib.h>
#include <string.h>

#include "stacka.h"

struct stacka {
	unsigned int capacity;
	unsigned int nitems;
	void **data;
};

struct stacka * stacka_create (unsigned int capacity)
{
	struct stacka *stacka;
	stacka = malloc(sizeof(*stacka));
	if (stacka == NULL) {
		return NULL;
	}
	stacka->capacity = capacity;
	stacka->nitems = 0;
	stacka->data = NULL;
	if (stacka->capacity == 0) {
		return stacka;
	}
	stacka->data = malloc(sizeof(void *) * stacka->capacity);
	if (stacka->data == NULL) {
		free(stacka);
		return NULL;
	}
	return stacka;
}

int stacka_destroy (struct stacka *stacka)
{
	if (stacka == NULL) {
		return -1;
	}
	if (stacka->data != NULL) {
		free(stacka->data);
	}
	free(stacka);
	return 0;
}

static int stacka_resize (struct stacka *stacka, unsigned int capacity)
{
	void **d;
	if (stacka == NULL) {
		return -1;
	}
	if (capacity == 0) {
		capacity = 1;
	}
	d = realloc(stacka->data, sizeof(void *) * capacity);
	if (d == NULL) {
		d = malloc(sizeof(void *) * capacity);
		if (d == NULL) {
			return -1;
		}
		if (stacka->nitems > 0) {
			memcpy(d, stacka->data, sizeof(void *) * stacka->nitems);
		}
		if (stacka->data != NULL) {
			free(stacka->data);
		}
	}
	stacka->data = d;
	stacka->capacity = capacity;
	return 0;
}

int stacka_push (struct stacka *stacka, void *data)
{
	int rc;
	if (stacka == NULL) {
		return 0;
	}
	if (stacka->nitems + 1 > stacka->capacity) {
		/* by multiplying by 2, complexity is decreased from
		 *   n^2: 1 + 2 + ... + N ~ n^2 / 2 ~ n^2
		 *   to
		 *   n : n + (2 + 4 + 8 + ... + n) ~ 3n ~ n
		 */
		rc = stacka_resize(stacka, stacka->capacity * 2);
		if (rc != 0) {
			return -1;
		}
	}
	stacka->data[stacka->nitems] = data;
	stacka->nitems += 1;
	return 0;
}

void * stacka_pop (struct stacka *stacka)
{
	void *data;
	if (stacka == NULL) {
		return NULL;
	}
	if (stacka->nitems <= 0) {
		return NULL;
	}
	data = stacka->data[stacka->nitems - 1];
	stacka->nitems -= 1;
	if ((stacka->nitems > 0) &&
	    (stacka->nitems == stacka->capacity / 4)) {
		/* by halving when array is one-quarter full complexity is decreased from
		 *   n^2 to n
		 * array is between 25% and 100% full
		 */
		stacka_resize(stacka, stacka->capacity / 4);
	}
	return data;
}

unsigned int stacka_size (struct stacka *stacka)
{
	if (stacka == NULL) {
		return 0;
	}
	return stacka->nitems;
}

unsigned int stacka_capacity (struct stacka *stacka)
{
	if (stacka == NULL) {
		return 0;
	}
	return stacka->capacity;
}

int stacka_iterate (struct stacka *stacka, int (*iterator) (void *context, struct stacka *stacka, void *item), void *context)
{
	int i;
	if (stacka == NULL) {
		return -1;
	}
	if (iterator == NULL) {
		return -1;
	}
	if (stacka->nitems <= 0) {
		return 0;
	}
	for (i = stacka->nitems - 1; i >= 0; i--) {
		iterator(context, stacka, stacka->data[i]);
	}
	return 0;
}

int stacka_sort (struct stacka *stacka, void (*sort) (void *base, size_t nmemb, size_t size, int (*compare) (const void *a, const void *b, void *context), void *context), int (*compare) (const void *a, const void *b, void *context), void *context)
{
	if (stacka == NULL) {
		return -1;
	}
	if (sort == NULL) {
		return -1;
	}
	if (compare == NULL) {
		return -1;
	}
	if (stacka->nitems <= 0) {
		return -1;
	}
	sort(stacka->data, stacka->nitems, sizeof(void *), compare, context);
	return 0;
}

void * stacka_search (struct stacka *stacka, void * (*search) (const void *key, const void *base, size_t nmemb, size_t size, int (*compare) (const void *a, const void *b, void *context), void *context), int (*compare) (const void *a, const void *b, void *context), void *context, void *key)
{
	void **value;
	if (stacka == NULL) {
		return NULL;
	}
	if (search == NULL) {
		return NULL;
	}
	if (compare == NULL) {
		return NULL;
	}
	if (stacka->nitems <= 0) {
		return NULL;
	}
	value = search(&key, stacka->data, stacka->nitems, sizeof(void *), compare, context);
	if (value == NULL) {
		return NULL;
	}
	return *value;
}

void * stacka_select (struct stacka *stacka, void * (*select) (void *base, size_t nmemb, size_t size, int (*compare) (const void *a, const void *b, void *context), void *context, size_t k), int (*compare) (const void *a, const void *b, void *context), void *context, size_t k)
{
	if (stacka == NULL) {
		return NULL;
	}
	if (select == NULL) {
		return NULL;
	}
	if (compare == NULL) {
		return NULL;
	}
	if (stacka->nitems <= 0) {
		return NULL;
	}
	return *(void **) select(stacka->data, stacka->nitems, sizeof(void *), compare, context, k);
}
