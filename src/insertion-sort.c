
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* complexity
 *   n^2 / 4
 *   if is already sorted it does not any exchanges
 *   if it is descending order 1/2 n^2 compares and 1/2 n^2 exchanges
 *   number of exchanges is equal to number of inversions
 *
 *   is a stable sort
 *     we never move equal items past each other
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

void insertion_sort (void *base, size_t nmemb, size_t size, int (*compare) (const void *a, const void *b, void *context), void *context)
{
	size_t i;
	ssize_t j;
	if (base == NULL) {
		return;
	}
	if (compare == NULL) {
		return;
	}
	for (i = 0; i < nmemb; i++) {
		for (j = i; j > 0; j--) {
			if (comp(base, nmemb, size, compare, context, j, j - 1) < 0) {
				exch(base, nmemb, size, j, j - 1);
			} else {
				break;
			}
		}
	}
}
