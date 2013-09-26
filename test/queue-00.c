
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

int main (int argc, char *argv[])
{
	int i;
	struct queue *queue;
	queue = queue_create();
	if (queue == NULL) {
		return -1;
	}
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-") == 0) {
			printf("%s ", (char *) queue_dequeue(queue));
		} else {
			queue_enqueue(queue, argv[i]);
		}
	}
	queue_destroy(queue);
	return 0;
}
