
#include <stdlib.h>
#include <string.h>

/* complexity
 *   linear time
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

int shuffle (void *base, ssize_t nmemb, ssize_t size)
{
	ssize_t i;
	ssize_t r;
	if (base == NULL) {
		return -1;
	}
	if (nmemb <= 0) {
		return -1;
	}
	if (size <= 0) {
		return -1;
	}
	for (i = 0; i < nmemb; i++) {
		r = rand() % (i + 1);
		exch(base, nmemb, size, i, r);
	}
	return 0;
}
