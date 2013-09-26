
struct binary_heap;

struct binary_heap * binary_heap_create (size_t capacity, int (*compare) (const void *a, const void *b, void *context), void *context);
int binary_heap_destroy (struct binary_heap *binary_heap);
int binary_heap_insert (struct binary_heap *binary_heap, void *item);
void * binary_heap_pop (struct binary_heap *binary_heap);
