
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* complexity
 *   with 3x + 1 increments n^(3/2)
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

void shell_sort (void *base, size_t nmemb, size_t size, int (*compare) (const void *a, const void *b, void *context), void *context)
{
	size_t i;
	size_t j;
	size_t h;
	if (base == NULL) {
		return;
	}
	if (compare == NULL) {
		return;
	}
	h = 1;
	while (h < (nmemb / 3)) {
		/* 3x + 1 increment sequence */
		h = 3 * h + 1;
	}
	while (h >= 1) {
		for (i = h; i < nmemb; i++) {
			for (j = i; j >= h; j -= h) {
				if (comp(base, nmemb, size, compare, context, j, j - h) < 0) {
					exch(base, nmemb, size, j, j - h);
				} else {
					break;
				}
			}
		}
		h /= 3;
	}
}
