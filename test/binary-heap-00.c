
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "binary-heap.h"

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
	struct binary_heap *binary_heap;
	binary_heap = binary_heap_create(0, compare, NULL);
	if (binary_heap == NULL) {
		return -1;
	}
	for (i = 1; i < argc; i++) {
		binary_heap_insert(binary_heap, argv[i]);
	}
	printf("%s ", (char *) binary_heap_pop(binary_heap));
	printf("%s ", (char *) binary_heap_pop(binary_heap));
	printf("%s ", (char *) binary_heap_pop(binary_heap));
	binary_heap_destroy(binary_heap);
	return 0;
}
