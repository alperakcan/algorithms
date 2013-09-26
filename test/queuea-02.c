
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queuea.h"

static int queuea_iterator (void *context, struct queuea *queuea, void *data)
{
	(void) context;
	(void) queuea;
	printf("%s ", (char *) data);
	return 0;
}

int main (int argc, char *argv[])
{
	int i;
	struct queuea *queuea;
	queuea = queuea_create(0);
	if (queuea == NULL) {
		return -1;
	}
	for (i = 1; i < argc; i++) {
		queuea_enqueue(queuea, argv[i]);
	}
	queuea_iterate(queuea, queuea_iterator, NULL);
	queuea_destroy(queuea);
	return 0;
}
