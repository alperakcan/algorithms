
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "insertion-sort.h"
#include "merge-sort.h"

/* complexity
 *   nlog(n)
 *
 * not an in-place algorithm, an algorithm is in-place if it uses <= clog(n) extra memory
 */

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

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

void bottom_up_merge_sort (void *base, size_t nmemb, size_t size, int (*compare) (const void *a, const void *b, void *context), void *context)
{
	void *aux;
	size_t sz;
	size_t lo;
	aux = malloc(nmemb * size);
	if (aux == NULL) {
		return;
	}
	for (sz = 1; sz < nmemb; sz *= 2) {
		for (lo = 0; lo < nmemb - sz; lo += (sz * 2)) {
			merge(base, nmemb, size, compare, context, aux, lo, lo + sz - 1, MIN(lo + (sz * 2) - 1, nmemb - 1));
		}
	}
	free(aux);
}
