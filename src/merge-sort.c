
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "insertion-sort.h"
#include "merge-sort.h"

/* complexity
 *   nlog(n)
 *
 * not an in-place algorithm, an algorithm is in-place if it uses <= clog(n) extra memory
 *
 * is a stable sort
 * stable as long as the merge operation is stable
 * in this implementation will always preserve the relative order
 */

#define CUTOFF 7

static int copy (void *dst, void *src, ssize_t nmemb, ssize_t size, ssize_t d, ssize_t s)
{
	void *ap;
	void *bp;
	(void) nmemb;
	ap = ((char *) dst) + (d * size);
	bp = ((char *) src) + (s * size);
	memcpy(ap, bp, size);
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

static int merge (void *base, ssize_t nmemb, ssize_t size, int (*compare) (const void *a, const void *b, void *context), void *context, void *aux, ssize_t lo, ssize_t mid, ssize_t hi)
{
	ssize_t i;
	ssize_t j;
	ssize_t k;
	(void) nmemb;
	for (k = lo; k <= hi; k++) {
		copy(aux, base, nmemb, size, k, k);
	}
	i = lo;
	j = mid + 1;
	for (k = lo; k <= hi; k++) {
		if (i > mid) {
			copy(base, aux, nmemb, size, k, j);
			j++;
		} else if (j > hi) {
			copy(base, aux, nmemb, size, k, i);
			i++;
		} else if (comp(aux, nmemb, size, compare, context, j, i) < 0) {
			copy(base, aux, nmemb, size, k, j);
			j++;
		} else {
			copy(base, aux, nmemb, size, k, i);
			i++;
		}
	}
	return 0;
}
static int sort (void *base, ssize_t nmemb, ssize_t size, int (*compare) (const void *a, const void *b, void *context), void *context, void *aux, ssize_t lo, ssize_t hi)
{
	ssize_t mid;
	if (hi <= lo + CUTOFF - 1) {
		/* using insertion sort for small arrays make is even 20% faster */
		insertion_sort(base, nmemb, size, compare, context);
		return 0;
	}
	mid = lo + (hi - lo) / 2;
	sort(base, nmemb, size, compare, context, aux, lo, mid);
	sort(base, nmemb, size, compare, context, aux, mid + 1, hi);
	if (comp(base, nmemb, size, compare, context, mid + 1, mid) >= 0) {
		/* stop if already sorted
		 * is biggest item in first half <= smallest item in second half
		 * helps for partially-ordered arrays
		 */
		return 0;
	}
	merge(base, nmemb, size, compare, context, aux, lo, mid, hi);
	return 0;
}

void merge_sort (void *base, size_t nmemb, size_t size, int (*compare) (const void *a, const void *b, void *context), void *context)
{
	void *aux;
	aux = malloc(nmemb * size);
	if (aux == NULL) {
		return;
	}
	sort(base, nmemb, size, compare, context, aux, 0, nmemb - 1);
	free(aux);
}
