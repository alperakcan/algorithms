
#include <stdlib.h>

#include "list.h"
#include "stack.h"

struct item {
	struct list list;
	void *data;
};

struct stack {
	struct list list;
	unsigned int nitems;
};

struct stack * stack_create (void)
{
	int rc;
	struct stack *stack;
	stack = malloc(sizeof(*stack));
	if (stack == NULL) {
		return NULL;
	}
	rc = list_init(&stack->list);
	if (rc != 0) {
		free(stack);
		return NULL;
	}
	stack->nitems = 0;
	return stack;
}

static int stack_destroy_iterator (void *context, struct list *list, struct list *item)
{
	struct item *i;
	(void) context;
	i = list_containerof(item, struct item, list);
	list_del(list, &i->list);
	free(i);
	return 0;
}

int stack_destroy (struct stack *stack)
{
	if (stack == NULL) {
		return -1;
	}
	list_iterate_safe(&stack->list, stack_destroy_iterator, NULL);
	free(stack);
	return 0;
}

int stack_push (struct stack *stack, void *data)
{
	struct item *item;
	if (stack == NULL) {
		return -1;
	}
	item = malloc(sizeof(*item));
	if (item == NULL) {
		return -1;
	}
	item->data = data;
	list_add(&stack->list, &item->list);
	stack->nitems += 1;
	return 0;
}

void * stack_pop (struct stack *stack)
{
	void *data;
	struct list *l;
	struct item *i;
	if (stack == NULL) {
		return NULL;
	}
	if (stack->nitems <= 0) {
		return NULL;
	}
	l = list_first(&stack->list);
	if (l == NULL) {
		return NULL;
	}
	i = list_containerof(l, struct item, list);
	if (i == NULL) {
		return NULL;
	}
	data = i->data;
	list_del(&stack->list, &i->list);
	free(i);
	stack->nitems -= 1;
	return data;
}

unsigned int stack_size (struct stack *stack)
{
	if (stack == NULL) {
		return 0;
	}
	return stack->nitems;
}

struct stack_iterator_context {
	struct stack *stack;
	int (*iterator) (void *context, struct stack *stack, void *item);
	void *context;
};

static int stack_iterator (void *context, struct list *list, struct list *item)
{
	struct item *i;
	struct stack_iterator_context *c;
	(void) list;
	c = context;
	i = list_containerof(item, struct item, list);
	c->iterator(c->context, c->stack, i->data);
	return 0;
}

int stack_iterate (struct stack *stack, int (*iterator) (void *context, struct stack *stack, void *item), void *context)
{
	struct stack_iterator_context c;
	if (stack == NULL) {
		return -1;
	}
	if (iterator == NULL) {
		return -1;
	}
	c.iterator = iterator;
	c.context = context;
	c.stack = stack;
	list_iterate(&stack->list, stack_iterator, &c);
	return 0;
}
