
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* complexity
 *
 *   construction < 2N compares and exchanges
 *   heap sort uses 2nlogn compares and exchanges
 *
 *   significant because, in-place sorting algorithm with nlogn worst-case
 *
 *   merge-sort: linear extra space, in-place merge is possible but not practical, too complex to develop
 *   quick-sort: quadratic time in worst case, nlogn worst case quick sort is possible but not practical
 *
 *   but,
 *     inner loop longer than quicksort's
 *     make poor use of cache memory
 *     not stable
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

static int sink (void *base, size_t nmemb, ssize_t size, int (*compare) (const void *a, const void *b, void *context), void *context, size_t k)
{
	size_t j;
	while (k * 2 <= nmemb) {
		j = k * 2;
		if ((j < nmemb) &&
		    (comp(base, nmemb, size, compare, context, j, j + 1) < 0)) {
			j++;
		}
		if (comp(base, nmemb, size, compare, context, k, j) >= 0) {
			break;
		}
		exch(base, nmemb, size, k, j);
		k = j;
	}
	return 0;
}

void heap_sort (void *base, size_t nmemb, size_t size, int (*compare) (const void *a, const void *b, void *context), void *context)
{
	size_t k;
	size_t n;
	n = nmemb;
	for (k = n / 2; k >= 1; k--) {
		sink(base, nmemb, size, compare, context, k);
	}
	while (n > 1) {
		exch(base, nmemb, size, 1, n);
		sink(base, --n, size, compare, context, 1);
	}
}
