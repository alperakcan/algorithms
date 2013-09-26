
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

int main (int argc, char *argv[])
{
	int i;
	struct stack *stack;
	stack = stack_create();
	if (stack == NULL) {
		return -1;
	}
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-") == 0) {
			printf("%s ", (char *) stack_pop(stack));
		} else {
			stack_push(stack, argv[i]);
		}
	}
	stack_destroy(stack);
	return 0;
}
