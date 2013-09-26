
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

static int iterator (void *context, struct stack *stack, void *data)
{
	(void) context;
	(void) stack;
	printf("%s ", (char *) data);
	return 0;
}

int main (int argc, char *argv[])
{
	int i;
	struct stack *stack;
	stack = stack_create();
	if (stack == NULL) {
		return -1;
	}
	for (i = 1; i < argc; i++) {
		stack_push(stack, argv[i]);
	}
	stack_iterate(stack, iterator, NULL);
	stack_destroy(stack);
	return 0;
}
