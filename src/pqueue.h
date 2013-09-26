
struct pqueue;

struct pqueue * pqueue_create (size_t capacity, int (*compare) (const void *a, const void *b, void *context), void *context);
int pqueue_destroy (struct pqueue *pqueue);
int pqueue_insert (struct pqueue *pqueue, void *item);
void * pqueue_pop (struct pqueue *pqueue);
