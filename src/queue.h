
struct queue;

struct queue * queue_create (void);
int queue_destroy (struct queue *queue);
int queue_enqueue (struct queue *queue, void *data);
void * queue_dequeue (struct queue *queue);
unsigned int queue_size (struct queue *queue);
int queue_iterate (struct queue *queue, int (*iterator) (void *context, struct queue *queue, void *item), void *context);
