
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

static int queue_iterator (void *context, struct queue *queue, void *data)
{
	(void) context;
	(void) queue;
	printf("%s ", (char *) data);
	return 0;
}

int main (int argc, char *argv[])
{
	int i;
	struct queue *queue;
	queue = queue_create();
	if (queue == NULL) {
		return -1;
	}
	for (i = 1; i < argc; i++) {
		queue_enqueue(queue, argv[i]);
	}
	queue_iterate(queue, queue_iterator, NULL);
	queue_destroy(queue);
	return 0;
}
