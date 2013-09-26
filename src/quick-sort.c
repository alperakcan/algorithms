
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shuffle.h"
#include "insertion-sort.h"
#include "quick-sort.h"

/* complexity
 *
 *   nlog(n)
 *
 *   worst case n^2 compares
 *   %40 compares than merge sort
 *   but faster than merge sort in practice because of less data movement
 *
 *   random shuffle is to guarantee the worst case wont happen
 *
 *   is an in-place algorithm
 *   is not stable
 *
 * it is also recursive
 * it does the recursion after it do the work
 * first shuffle
 * and then partition
 * sort each piece recursively
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

static int sort (void *base, ssize_t nmemb, ssize_t size, int (*compare) (const void *a, const void *b, void *context), void *context, ssize_t lo, ssize_t hi)
{
	ssize_t j;
	if (hi <= lo + CUTOFF - 1) {
		/* increase performance by 20% */
		insertion_sort(base, nmemb, size, compare, context);
		return 0;

	}
#if 0
	/* may improve the time by 10% */
	ssize_t m;
	m = medianof3(base, nmemb, size, lo, lo + (hi - lo) / 2, hi);
	swap(base, nmemb, size, lo, m);
#endif
	j = partition(base, nmemb, size, compare, context, lo, hi);
	sort(base, nmemb, size, compare, context, lo, j - 1);
	sort(base, nmemb, size, compare, context, j + 1, hi);
	return 0;
}

void quick_sort (void *base, size_t nmemb, size_t size, int (*compare) (const void *a, const void *b, void *context), void *context)
{
	/* shuffle needed for performance guarantee */
	shuffle(base, nmemb, size);
	sort(base, nmemb, size, compare, context, 0, nmemb - 1);
}
