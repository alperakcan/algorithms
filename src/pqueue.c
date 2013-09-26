

#include <stdlib.h>
#include <string.h>

#include "pqueue.h"

/* complexity
 *  MN
 *
 *  for an unordered insert is 1, pop is N
 *  for an ordered insert in N, pop is 1
 */

struct pqueue {
	size_t capacity;
	size_t nitems;
	int (*compare) (const void *a, const void *b, void *context);
	void *context;
	void **data;
};

struct pqueue * pqueue_create (size_t capacity, int (*compare) (const void *a, const void *b, void *context), void *context)
{
	struct pqueue *pqueue;
	pqueue = malloc(sizeof(*pqueue));
	if (pqueue == NULL) {
		return NULL;
	}
	pqueue->capacity = capacity;
	pqueue->nitems = 0;
	pqueue->compare = compare;
	pqueue->context = context;
	if (capacity == 0) {
		pqueue->data = NULL;
		return pqueue;
	}
	pqueue->data = malloc(sizeof(void *) * capacity);
	if (pqueue->data == NULL) {
		free(pqueue);
		return NULL;
	}
	return pqueue;
}

int pqueue_destroy (struct pqueue *pqueue)
{
	if (pqueue == NULL) {
		return -1;
	}
	if (pqueue->data != NULL) {
		free(pqueue->data);
	}
	free(pqueue);
	return 0;
}

static int pqueue_resize (struct pqueue *pqueue, size_t capacity)
{
	void **data;
	if (capacity == 0) {
		capacity = 1;
	}
	data = realloc(pqueue->data, sizeof(void *) * capacity);
	if (data == NULL) {
		data = malloc(sizeof(void *) * capacity);
		if (data == NULL) {
			return -1;
		}
		if (pqueue->nitems > 0) {
			memcpy(data, pqueue->data, sizeof(void *) * pqueue->nitems);
		}
		if (pqueue->data != NULL) {
			free(pqueue->data);
		}
	}
	pqueue->data = data;
	pqueue->capacity = capacity;
	return 0;
}

int pqueue_insert (struct pqueue *pqueue, void *item)
{
	int rc;
	if (pqueue == NULL) {
		return -1;
	}
	if (pqueue->nitems + 1 > pqueue->capacity) {
		rc = pqueue_resize(pqueue, pqueue->capacity * 2);
		if (rc != 0) {
			return -1;
		}
	}
	pqueue->data[pqueue->nitems] = item;
	pqueue->nitems += 1;
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
	ap = ((char *) base) + (a * size);
	bp = ((char *) base) + (b * size);
	memcpy(tp, ap, size);
	memmove(ap, bp, size);
	memcpy(bp, tp, size);
	free(tp);
	return 0;
}

void * pqueue_pop (struct pqueue *pqueue)
{
	size_t i;
	size_t m;
	void *data;
	if (pqueue == NULL) {
		return NULL;
	}
	if (pqueue->nitems <= 0) {
		return NULL;
	}
	m = 0;
	for (i = 1; i < pqueue->nitems; i++) {
		if (pqueue->compare((void *) &pqueue->data[i], (void *) &pqueue->data[m], pqueue->context) > 0) {
			m = i;
		}
	}
	exch(pqueue->data, pqueue->nitems, sizeof(void *), m, pqueue->nitems - 1);
	data = pqueue->data[pqueue->nitems - 1];
	pqueue->nitems -= 1;
	if ((pqueue->nitems > 0) &&
	    (pqueue->nitems == pqueue->capacity / 4)) {
		pqueue_resize(pqueue, pqueue->capacity / 4);
	}
	return data;
}
