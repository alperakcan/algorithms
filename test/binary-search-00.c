
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stacka.h"
#include "quick-sort.h"
#include "binary-search.h"

static int compare (const void *a, const void *b, void *context)
{
	const char *s1 = *(char **) a;
	const char *s2 = *(char **) b;
	(void) context;
	return strcmp(s2, s1);
}

int main (int argc, char *argv[])
{
	int i;
	char *value;
	struct stacka *stacka;
	if (argc <= 1) {
		return -1;
	}
	stacka = stacka_create(0);
	if (stacka == NULL) {
		return -1;
	}
	for (i = 1; i < argc; i++) {
		stacka_push(stacka, argv[i]);
	}
	stacka_sort(stacka, quick_sort, compare, NULL);
	value = stacka_search(stacka, binary_search, compare, NULL, argv[0]);
	if (value != NULL) {
		stacka_destroy(stacka);
		return -1;
	}
	value = stacka_search(stacka, binary_search, compare, NULL, argv[1]);
	if (value == NULL) {
		stacka_destroy(stacka);
		return -1;
	}
	printf("%s ", value);
	value = stacka_search(stacka, binary_search, compare, NULL, argv[3]);
	if (value == NULL) {
		stacka_destroy(stacka);
		return -1;
	}
	printf("%s ", value);
	stacka_destroy(stacka);
	return 0;
}
