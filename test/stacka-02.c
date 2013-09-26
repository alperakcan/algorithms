
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stacka.h"

static int iterator (void *context, struct stacka *stacka, void *data)
{
	(void) context;
	(void) stacka;
	printf("%s ", (char *) data);
	return 0;
}

int main (int argc, char *argv[])
{
	int i;
	struct stacka *stacka;
	stacka = stacka_create(0);
	if (stacka == NULL) {
		return -1;
	}
	for (i = 1; i < argc; i++) {
		stacka_push(stacka, argv[i]);
	}
	stacka_iterate(stacka, iterator, NULL);
	stacka_destroy(stacka);
	return 0;
}
