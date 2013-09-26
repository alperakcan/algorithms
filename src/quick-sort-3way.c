
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shuffle.h"
#include "insertion-sort.h"
#include "quick-sort-3way.h"

/* complexity
 *
 *   nlog(n)
 *
 *   handles better, when there are equal keys to sort
 *   is entropy-optimal: number of compares it uses is equal to lower bound with in a constant factor
 *     whatever the distribution of equal keys it does not matter for this algorithm
 *
 *   randomized reduces time from linearithmetic to linear in broad class of applications
 *
 *   Tukey's ninther may be used for chooosing the partitioning element
 *     take 9 elements from elements and use the median of the medians
 *     this gets closer to the median of the array, better partitioning than random shuffle and less costly
 *
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

static int sort (void *base, ssize_t nmemb, ssize_t size, int (*compare) (const void *a, const void *b, void *context), void *context, ssize_t lo, ssize_t hi)
{
	int cmp;
	ssize_t i;
	ssize_t lt;
	ssize_t gt;
	if (hi <= lo + CUTOFF - 1) {
		/* increase performance by 20% */
		insertion_sort(base, nmemb, size, compare, context);
		return 0;
	}
	i = lo;
	lt = lo;
	gt = hi;
	while (i <= gt) {
		cmp = comp(base, nmemb, size, compare, context, i, lo);
		if (cmp < 0) {
			exch(base, nmemb, size, lt++, i++);
		} else if (cmp > 0) {
			exch(base, nmemb, size, i, gt--);
		} else {
			i += 1;
		}
	}
	sort(base, nmemb, size, compare, context, lo, lt - 1);
	sort(base, nmemb, size, compare, context, gt + 1, hi);
	return 0;
}

void quick_sort_3way (void *base, size_t nmemb, size_t size, int (*compare) (const void *a, const void *b, void *context), void *context)
{
	/* shuffle needed for performance guarantee */
	shuffle(base, nmemb, size);
	sort(base, nmemb, size, compare, context, 0, nmemb - 1);
}
