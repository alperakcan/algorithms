
#include <stdlib.h>

#include "binary-search.h"

static int comp (const void *base, size_t nmemb, size_t size, int (*compare) (const void *a, const void *b, void *context), void *context, const void *key, size_t b)
{
	void *bp;
	(void) nmemb;
	bp = ((char *) base) + (b * size);
	return compare(key, bp, context);
}

void * binary_search (const void *key, const void *base, size_t nmemb, size_t size, int (*compare) (const void *a, const void *b, void *context), void *context)
{
	int cmp;
	size_t lo;
	size_t hi;
	size_t mid;
	lo = 0;
	hi = nmemb - 1;
	while (lo <= hi) {
		mid = lo + (hi - lo) / 2;
		cmp = comp(base, nmemb, size, compare, context, key, mid);
		if (cmp < 0) {
			hi = mid - 1;
		} else if (cmp > 0) {
			lo = mid + 1;
		} else {
			return (((char *) base) + (mid * size));
		}
	}
	return NULL;
}
