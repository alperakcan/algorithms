
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shuffle.h"
#include "quick-select.h"

/* complexity
 *
 *  takes linear time on the average
 *  each partitioning step splits the array approximately in half
 *
 *  random shuffle, eliminates worst case. worst case is n^2
 *
 *  selecting kth maximum value
 */

#define CUTOFF 10

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

static int partition (void *base, ssize_t nmemb, ssize_t size, int (*compare) (const void *a, const void *b, void *context), void *context, ssize_t lo, ssize_t hi)
{
	ssize_t i;
	ssize_t j;
	i = lo;
	j = hi + 1;
	while (1) {
		/* find item on left to swap */
		while (comp(base, nmemb, size, compare, context, ++i, lo) < 0) {
			if (i == hi) {
				break;
			}
		}
		/* find item on right to swap */
		while (comp(base, nmemb, size, compare, context, lo, --j) < 0) {
			if (j == lo) {
				break;
			}
		}
		if (i >= j) {
			/* check if pointers cross */
			break;
		}
		/* swap */
		exch(base, nmemb, size, i, j);
	}
	/* swap with partitioning item */
	exch(base, nmemb, size, lo, j);
	/* return index of item now known to be in place */
	return j;
}

void * quick_select (void *base, size_t nmemb, size_t size, int (*compare) (const void *a, const void *b, void *context), void *context, size_t k)
{
	size_t j;
	ssize_t lo;
	ssize_t hi;
	/* shuffle needed for performance guarantee */
	shuffle(base, nmemb, size);
	lo = 0;
	hi = nmemb - 1;
	while (hi > lo) {
		j = partition(base, nmemb, size, compare, context, lo, hi);
		if (j < k) {
			lo = j + 1;
		} else if (j > k) {
			hi = j - 1;
		} else {
			return (((char *) base) + (k * size));
		}
	}
	return (((char *) base) + (k * size));
}
