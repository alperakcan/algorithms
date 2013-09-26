
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pqueue.h"

static int compare (const void *a, const void *b, void *context)
{
	const char *s1 = *(char **) a;
	const char *s2 = *(char **) b;
	(void) context;
	return strcmp(s1, s2);
}

int main (int argc, char *argv[])
{
	int i;
	struct pqueue *pqueue;
	pqueue = pqueue_create(0, compare, NULL);
	if (pqueue == NULL) {
		return -1;
	}
	for (i = 1; i < argc; i++) {
		pqueue_insert(pqueue, argv[i]);
	}
	printf("%s ", (char *) pqueue_pop(pqueue));
	printf("%s ", (char *) pqueue_pop(pqueue));
	printf("%s ", (char *) pqueue_pop(pqueue));
	pqueue_destroy(pqueue);
	return 0;
}
