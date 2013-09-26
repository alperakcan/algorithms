
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stacka.h"
#include "quick-select.h"

static int compare (const void *a, const void *b, void *context)
{
	int i1;
	int i2;
	const char *s1 = *(char **) a;
	const char *s2 = *(char **) b;
	(void) context;
	i1 = strtol(s1, NULL, 10);
	i2 = strtol(s2, NULL, 10);
	if (i1 < i2) {
		return 1;
	}
	if (i1 > i2) {
		return -1;
	}
	return 0;
}

int main (int argc, char *argv[])
{
	int i;
	char *value;
	struct stacka *stacka;
	stacka = stacka_create(0);
	if (stacka == NULL) {
		return -1;
	}
	for (i = 1; i < argc; i++) {
		stacka_push(stacka, argv[i]);
	}
	value = (char *) stacka_select(stacka, quick_select, compare, NULL, 5);
	printf("%s ", value);
	stacka_destroy(stacka);
	return 0;
}
