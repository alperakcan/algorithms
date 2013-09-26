

#include <stdlib.h>
#include <string.h>

#include "binary-heap.h"

/* complexity
 *
 *  logN
 *
 *  insert logn, del max logn
 *
 *  d-ary heap insert logd(n), del max logdn
 *  fibonacci insert 1, del lonn
 *
 *  item 0 starts from index 1, to easy-up the calculations
 */

struct binary_heap {
	size_t capacity;
	size_t nitems;
	int (*compare) (const void *a, const void *b, void *context);
	void *context;
	void **data;
};

struct binary_heap * binary_heap_create (size_t capacity, int (*compare) (const void *a, const void *b, void *context), void *context)
{
	struct binary_heap *binary_heap;
	binary_heap = malloc(sizeof(*binary_heap));
	if (binary_heap == NULL) {
		return NULL;
	}
	binary_heap->capacity = capacity;
	binary_heap->nitems = 0;
	binary_heap->compare = compare;
	binary_heap->context = context;
	if (capacity == 0) {
		binary_heap->data = NULL;
		return binary_heap;
	}
	binary_heap->data = malloc(sizeof(void *) * capacity);
	if (binary_heap->data == NULL) {
		free(binary_heap);
		return NULL;
	}
	return binary_heap;
}

int binary_heap_destroy (struct binary_heap *binary_heap)
{
	if (binary_heap == NULL) {
		return -1;
	}
	if (binary_heap->data != NULL) {
		free(binary_heap->data);
	}
	free(binary_heap);
	return 0;
}

static int binary_heap_resize (struct binary_heap *binary_heap, size_t capacity)
{
	void **data;
	if (capacity <= 0) {
		capacity = 1;
	}
	data = realloc(binary_heap->data, sizeof(void *) * capacity);
	if (data == NULL) {
		data = malloc(sizeof(void *) * capacity);
		if (data == NULL) {
			return -1;
		}
		if (binary_heap->nitems > 0) {
			memcpy(data, binary_heap->data, sizeof(void *) * binary_heap->nitems);
		}
		if (binary_heap->data != NULL) {
			free(binary_heap->data);
		}
	}
	binary_heap->data = data;
	binary_heap->capacity = capacity;
	return 0;
}

static int exch (void *base, ssize_t nmemb, ssize_t size, ssize_t a, ssize_t b)
{
	void *ap;
	void *bp;
	void *tp;
	(void) nmemb;
	tp = malloc(size);
	if (tp == NULL) {
		return -1;
	}
	a -= 1;
	b -= 1;
	ap = ((char *) base) + (a * size);
	bp = ((char *) base) + (b * size);
	memcpy(tp, ap, size);
	memmove(ap, bp, size);
	memcpy(bp, tp, size);
	free(tp);
	return 0;
}

static int comp (void *base, ssize_t nmemb, ssize_t size, int (*compare) (const void *a, const void *b, void *context), void *context, ssize_t a, ssize_t b)
{
	void *ap;
	void *bp;
	(void) nmemb;
	a -= 1;
	b -= 1;
	ap = ((char *) base) + (a * size);
	bp = ((char *) base) + (b * size);
	return compare(ap, bp, context);
}

static int swim (struct binary_heap *binary_heap, size_t k)
{
	while ((k > 1) &&
	       (comp(binary_heap->data, binary_heap->nitems, sizeof(void *), binary_heap->compare, binary_heap->context, k / 2, k) < 0)) {
		exch(binary_heap->data, binary_heap->nitems, sizeof(void *), k, (k / 2));
		k = k / 2;
	}
	return 0;
}

static int sink (struct binary_heap *binary_heap, size_t k)
{
	size_t j;
	while (k * 2 <= binary_heap->nitems) {
		j = k * 2;
		if ((j < binary_heap->nitems) &&
				(comp(binary_heap->data, binary_heap->nitems, sizeof(void *), binary_heap->compare, binary_heap->context, j, j + 1) < 0)) {
			j++;
		}
		if (comp(binary_heap->data, binary_heap->nitems, sizeof(void *), binary_heap->compare, binary_heap->context, k, j) >= 0) {
			break;
		}
		exch(binary_heap->data, binary_heap->nitems, sizeof(void *), k, j);
		k = j;
	}
	return 0;
}

int binary_heap_insert (struct binary_heap *binary_heap, void *item)
{
	int rc;
	if (binary_heap == NULL) {
		return -1;
	}
	if (binary_heap->nitems + 2 > binary_heap->capacity) {
		rc = binary_heap_resize(binary_heap, binary_heap->capacity * 2);
		if (rc != 0) {
			return -1;
		}
	}
	binary_heap->data[binary_heap->nitems] = item;
	binary_heap->nitems += 1;
	swim(binary_heap, binary_heap->nitems);
	return 0;
}

void * binary_heap_pop (struct binary_heap *binary_heap)
{
	void *data;
	if (binary_heap == NULL) {
		return NULL;
	}
	if (binary_heap->nitems <= 0) {
		return NULL;
	}
	data = binary_heap->data[0];
	exch(binary_heap->data, binary_heap->nitems, sizeof(void *), 1, binary_heap->nitems);
	binary_heap->nitems -= 1;
	sink(binary_heap, 1);
	if ((binary_heap->nitems > 0) &&
	    (binary_heap->nitems == binary_heap->capacity / 4)) {
		binary_heap_resize(binary_heap, binary_heap->capacity / 4);
	}
	return data;
}
