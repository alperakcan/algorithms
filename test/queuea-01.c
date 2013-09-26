
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queuea.h"

int main (int argc, char *argv[])
{
	int i;
	struct queuea *queuea;
	queuea = queuea_create(0);
	if (queuea == NULL) {
		return -1;
	}
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-") == 0) {
			printf("%s ", (char *) queuea_dequeue(queuea));
		} else {
			queuea_enqueue(queuea, argv[i]);
		}
	}
	queuea_destroy(queuea);
	return 0;
}
