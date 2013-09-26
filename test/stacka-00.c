
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stacka.h"

int main (int argc, char *argv[])
{
	int i;
	struct stacka *stacka;
	stacka = stacka_create(argc - 1);
	if (stacka == NULL) {
		return -1;
	}
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-") == 0) {
			printf("%s ", (char *) stacka_pop(stacka));
		} else {
			stacka_push(stacka, argv[i]);
		}
	}
	stacka_destroy(stacka);
	return 0;
}
