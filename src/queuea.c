
#include <stdlib.h>
#include <string.h>

#include "queuea.h"

struct queuea {
	unsigned int capacity;
	unsigned int nitems;
	void **data;
};

struct queuea * queuea_create (unsigned int capacity)
{
	struct queuea *queuea;
	queuea = malloc(sizeof(*queuea));
	if (queuea == NULL) {
		return NULL;
	}
	queuea->capacity = capacity;
	queuea->nitems = 0;
	queuea->data = NULL;
	if (queuea->capacity == 0) {
		return queuea;
	}
	queuea->data = malloc(sizeof(void *) * queuea->capacity);
	if (queuea->data == NULL) {
		free(queuea);
		return NULL;
	}
	return queuea;
}

int queuea_destroy (struct queuea *queuea)
{
	if (queuea == NULL) {
		return -1;
	}
	if (queuea->data != NULL) {
		free(queuea->data);
	}
	free(queuea);
	return 0;
}

static int queuea_resize (struct queuea *queuea, unsigned int capacity)
{
	void **d;
	if (queuea == NULL) {
		return -1;
	}
	if (capacity == 0) {
		capacity = 1;
	}
	d = realloc(queuea->data, sizeof(void *) * capacity);
	if (d == NULL) {
		d = malloc(sizeof(void *) * capacity);
		if (d == NULL) {
			return -1;
		}
		if (queuea->nitems > 0) {
			memcpy(d, queuea->data, sizeof(void *) * queuea->nitems);
		}
		if (queuea->data != NULL) {
			free(queuea->data);
		}
	}
	queuea->data = d;
	queuea->capacity = capacity;
	return 0;
}

int queuea_enqueue (struct queuea *queuea, void *data)
{
	int rc;
	if (queuea == NULL) {
		return 0;
	}
	if (queuea->nitems + 1 > queuea->capacity) {
		/* by multiplying by 2, complexity is decreased from
		 *   n^2: 1 + 2 + ... + N ~ n^2 / 2 ~ n^2
		 *   to
		 *   n : n + (2 + 4 + 8 + ... + n) ~ 3n ~ n
		 */
		rc = queuea_resize(queuea, queuea->capacity * 2);
		if (rc != 0) {
			return -1;
		}
	}
	queuea->data[queuea->nitems] = data;
	queuea->nitems += 1;
	return 0;
}

void * queuea_dequeue (struct queuea *queuea)
{
	void *data;
	if (queuea == NULL) {
		return NULL;
	}
	if (queuea->nitems <= 0) {
		return NULL;
	}
	data = queuea->data[0];
	if (queuea->nitems > 0) {
		memmove(&queuea->data[0], &queuea->data[1], sizeof(void *) * (queuea->nitems - 1));
	}
	queuea->nitems -= 1;
	if ((queuea->nitems > 0) &&
	    (queuea->nitems == queuea->capacity / 4)) {
		/* by halving when array is one-quarter full complexity is decreased from
		 *   n^2 to n
		 * array is between 25% and 100% full
		 */
		queuea_resize(queuea, queuea->capacity / 4);
	}
	return data;
}

unsigned int queuea_size (struct queuea *queuea)
{
	if (queuea == NULL) {
		return 0;
	}
	return queuea->nitems;
}

unsigned int queuea_capacity (struct queuea *queuea)
{
	if (queuea == NULL) {
		return 0;
	}
	return queuea->capacity;
}

int queuea_iterate (struct queuea *queuea, int (*iterator) (void *context, struct queuea *queuea, void *item), void *context)
{
	unsigned int i;
	if (queuea == NULL) {
		return -1;
	}
	if (iterator == NULL) {
		return -1;
	}
	if (queuea->nitems <= 0) {
		return 0;
	}
	for (i = 0; i < queuea->nitems; i++) {
		iterator(context, queuea, queuea->data[i]);
	}
	return 0;
}
