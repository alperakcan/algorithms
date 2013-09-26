
#include <stdio.h>
#include <stdlib.h>

#include "list.h"

struct item {
	struct list list;
	const char *string;
};

static int list_iterator (void *context, struct list *list, struct list *item)
{
	struct item *i;
	(void) context;
	(void) list;
	i = list_containerof(item, struct item, list);
	printf("%s ", i->string);
	return 0;
}

static int list_iterator_destroy (void *context, struct list *list, struct list *item)
{
	struct item *i;
	(void) context;
	i = list_containerof(item, struct item, list);
	list_del(list, &i->list);
	free(i);
	return 0;
}

int main (int argc, char *argv[])
{
	int i;
	int rc;
	struct list list;
	struct item *item;
	rc = list_init(&list);
	if (rc != 0) {
		return -1;
	}
	for (i = 1; i < argc; i++) {
		item = malloc(sizeof(*item));
		if (item == NULL) {
			continue;
		}
		item->string = argv[i];
		list_add(&list, &item->list);
	}
	list_iterate(&list, list_iterator, NULL);
	list_iterate_safe(&list, list_iterator_destroy, NULL);
	return 0;
}
