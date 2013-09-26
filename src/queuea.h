
struct queuea;

struct queuea * queuea_create (unsigned int capacity);
int queuea_destroy (struct queuea *queuea);
int queuea_enqueue (struct queuea *queuea, void *data);
void * queuea_dequeue (struct queuea *queuea);
unsigned int queuea_size (struct queuea *queuea);
unsigned int queuea_capacity (struct queuea *queuea);
int queuea_iterate (struct queuea *queuea, int (*iterator) (void *context, struct queuea *queuea, void *item), void *context);
