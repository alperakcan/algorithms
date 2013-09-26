
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

enum operator {
	operator_unknown,
	operator_plus,
	operator_minus,
	operator_multiply,
	operator_divide,
};

static int is_operator (const char *string)
{
	if (strcmp(string, "+") == 0) {
		return 1;
	} else if (strcmp(string, "-") == 0) {
		return 1;
	} else if (strcmp(string, "*") == 0) {
		return 1;
	} else if (strcmp(string, "\\*") == 0) {
		return 1;
	} else if (strcmp(string, "/") == 0) {
		return 1;
	}
	return 0;
}

static int is_numeric (const char *string)
{
	unsigned int i;
	unsigned int l;
	l = strlen(string);
	for (i = 0; i < l; i++) {
		if ((string[i] < '0') ||
		    (string[i] > '9')) {
			return 0;
		}
	}
	return 1;
}

static enum operator operator (const char *string)
{
	if (strcmp(string, "+") == 0) {
		return operator_plus;
	} else if (strcmp(string, "-") == 0) {
		return operator_minus;
	} else if (strcmp(string, "*") == 0) {
		return operator_multiply;
	} else if (strcmp(string, "\\*") == 0) {
		return operator_multiply;
	} else if (strcmp(string, "/") == 0) {
		return operator_divide;
	}
	return operator_unknown;
}

static int number (const char *string)
{
	return strtol(string, NULL, 10);
}

int main (int argc, char *argv[])
{
	int i;
	int *num;
	int *lnum;
	int *rnum;
	enum operator *op;
	struct stack *numbers;
	struct stack *operators;
	numbers = NULL;
	operators = NULL;
	numbers = stack_create();
	if (numbers == NULL) {
		goto error;
	}
	operators = stack_create();
	if (operators == NULL) {
		goto error;
	}
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "(") == 0) {
			continue;
		} else if (strcmp(argv[i], ")") == 0) {
			lnum = stack_pop(numbers);
			rnum = stack_pop(numbers);
			op = stack_pop(operators);
			num = malloc(sizeof(*num));
			if (num == NULL) {
				goto error;
			}
			if (*op == operator_plus) {
				*num = *lnum + *rnum;
			} else if (*op == operator_minus) {
				*num = *lnum - *rnum;
			} else if (*op == operator_multiply) {
				*num = *lnum * *rnum;
			} else if (*op == operator_divide) {
				*num = *lnum / *rnum;
			} else {
				fprintf(stderr, "unknown operator: %d\n", *op);
				free(num);
				free(rnum);
				free(lnum);
				free(op);
				goto error;
			}
			stack_push(numbers, num);
			free(rnum);
			free(lnum);
			free(op);
		} else if (is_numeric(argv[i])) {
			num = malloc(sizeof(*num));
			if (num == NULL) {
				goto error;
			}
			*num = number(argv[i]);
			stack_push(numbers, num);
		} else if (is_operator(argv[i])) {
			op = malloc(sizeof(*op));
			if (op == NULL) {
				goto error;
			}
			*op = operator(argv[i]);
			stack_push(operators, op);
		} else {
			fprintf(stderr, "unknown string: %s\n", argv[i]);
			goto error;
		}
	}
	num = stack_pop(numbers);
	printf("%d", *num);
	free(num);
	stack_destroy(numbers);
	stack_destroy(operators);
	return 0;
error:
	if (numbers != NULL) {
		stack_destroy(numbers);
	}
	if (operators != NULL) {
		stack_destroy(operators);
	}
	return -1;
}
