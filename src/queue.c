
#include <stdlib.h>

#include "list.h"
#include "queue.h"

struct item {
	struct list list;
	void *data;
};

struct queue {
	struct list list;
	unsigned int nitems;
};

struct queue * queue_create (void)
{
	int rc;
	struct queue *queue;
	queue = malloc(sizeof(*queue));
	if (queue == NULL) {
		return NULL;
	}
	rc = list_init(&queue->list);
	if (rc != 0) {
		free(queue);
		return NULL;
	}
	queue->nitems = 0;
	return queue;
}

static int queue_destroy_iterator (void *context, struct list *list, struct list *item)
{
	struct item *i;
	(void) context;
	i = list_containerof(item, struct item, list);
	list_del(list, &i->list);
	free(i);
	return 0;
}

int queue_destroy (struct queue *queue)
{
	if (queue == NULL) {
		return -1;
	}
	list_iterate_safe(&queue->list, queue_destroy_iterator, NULL);
	free(queue);
	return 0;
}

int queue_enqueue (struct queue *queue, void *data)
{
	struct item *item;
	if (queue == NULL) {
		return -1;
	}
	item = malloc(sizeof(*item));
	if (item == NULL) {
		return -1;
	}
	item->data = data;
	list_add_tail(&queue->list, &item->list);
	queue->nitems += 1;
	return 0;
}

void * queue_dequeue (struct queue *queue)
{
	void *data;
	struct list *l;
	struct item *i;
	if (queue == NULL) {
		return NULL;
	}
	if (queue->nitems <= 0) {
		return NULL;
	}
	l = list_first(&queue->list);
	if (l == NULL) {
		return NULL;
	}
	i = list_containerof(l, struct item, list);
	if (i == NULL) {
		return NULL;
	}
	data = i->data;
	list_del(&queue->list, &i->list);
	free(i);
	queue->nitems -= 1;
	return data;
}

unsigned int queue_size (struct queue *queue)
{
	if (queue == NULL) {
		return 0;
	}
	return queue->nitems;
}

struct queue_iterator_context {
	struct queue *queue;
	int (*iterator) (void *context, struct queue *queue, void *item);
	void *context;
};

static int queue_iterator (void *context, struct list *list, struct list *item)
{
	struct item *i;
	struct queue_iterator_context *c;
	(void) list;
	c = context;
	i = list_containerof(item, struct item, list);
	c->iterator(c->context, c->queue, i->data);
	return 0;
}

int queue_iterate (struct queue *queue, int (*iterator) (void *context, struct queue *queue, void *item), void *context)
{
	struct queue_iterator_context c;
	if (queue == NULL) {
		return -1;
	}
	if (iterator == NULL) {
		return -1;
	}
	c.iterator = iterator;
	c.context = context;
	c.queue = queue;
	list_iterate(&queue->list, queue_iterator, &c);
	return 0;
}
