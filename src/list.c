
#include <stdlib.h>

#include "list.h"

int list_init (struct list *list)
{
	list->next = list;
	list->prev = list;
	return 0;
}

int list_add (struct list *list, struct list *item)
{
	if (list == NULL) {
		return -1;
	}
	item->next = list->next;
	item->prev = list;
	list->next->prev = item;
	list->next = item;
	return 0;
}

int list_add_tail (struct list *list, struct list *item)
{
	if (list == NULL) {
		return -1;
	}
	item->prev = list->prev;
	item->next = list;
	list->prev->next = item;
	list->prev = item;
	return 0;
}

int list_del (struct list *list, struct list *item)
{
	if (list == NULL) {
		return -1;
	}
	item->next->prev = item->prev;
	item->prev->next = item->next;
	item->next = NULL;
	item->prev = NULL;
	return 0;
}

struct list * list_first (struct list *list)
{
	if (list == NULL) {
		return NULL;
	}
	if ((list->next == list) &&
	    (list->prev == list)) {
		return NULL;
	}
	return list->next;
}

int list_iterate (struct list *list, int (*iterator) (void *context, struct list *list, struct list *item), void *context)
{
	struct list *i;
	if (list == NULL) {
		return -1;
	}
	if (iterator == NULL) {
		return -1;
	}
	for (i = list->next; i != list; i = i->next) {
		iterator(context, list, i);
	}
	return 0;
}

int list_iterate_safe (struct list *list, int (*iterator) (void *context, struct list *list, struct list *item), void *context)
{
	struct list *i;
	struct list *n;
	if (list == NULL) {
		return -1;
	}
	if (iterator == NULL) {
		return -1;
	}
	for (i = list->next, n = i->next; i != list; i = n, n = n->next) {
		iterator(context, list, i);
	}
	return 0;
}
