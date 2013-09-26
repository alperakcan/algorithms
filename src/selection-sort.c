
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* complexity
 *   (n - 1) + (n - 2) + .... + 1 + 0 ~ n^2 / 2
 *   it is same event if it is already sorted
 *
 *   not a stable sort
 */

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

static int comp (void *base, ssize_t nmemb, ssize_t size, int (*compare) (const void *a, const void *b, void *context), void *context, ssize_t a, ssize_t b)
{
	void *ap;
	void *bp;
	(void) nmemb;
	ap = ((char *) base) + (a * size);
	bp = ((char *) base) + (b * size);
	return compare(ap, bp, context);
}

void selection_sort (void *base, size_t nmemb, size_t size, int (*compare) (const void *a, const void *b, void *context), void *context)
{
	size_t i;
	size_t j;
	size_t min;
	if (base == NULL) {
		return;
	}
	if (compare == NULL) {
		return;
	}
	for (i = 0; i < nmemb; i++) {
		min = i;
		for (j = i + 1; j < nmemb; j++) {
			if (comp(base, nmemb, size, compare, context, j, min) < 0) {
				min = j;
			}
		}
		exch(base, nmemb, size, i, min);
	}
}
