
struct stacka;

struct stacka * stacka_create (unsigned int capacity);
int stacka_destroy (struct stacka *stacka);
int stacka_push (struct stacka *stacka, void *data);
void * stacka_pop (struct stacka *stacka);
unsigned int stacka_size (struct stacka *stacka);
unsigned int stacka_capacity (struct stacka *stacka);
int stacka_iterate (struct stacka *stacka, int (*iterator) (void *context, struct stacka *stacka, void *item), void *context);
int stacka_sort (struct stacka *stacka, void (*sort) (void *base, size_t nmemb, size_t size, int (*compare) (const void *a, const void *b, void *context), void *context), int (*compare) (const void *a, const void *b, void *context), void *context);
void * stacka_search (struct stacka *stacka, void * (*search) (const void *key, const void *base, size_t nmemb, size_t size, int (*compare) (const void *a, const void *b, void *context), void *context), int (*compare) (const void *a, const void *b, void *context), void *context, void *key);
void * stacka_select (struct stacka *stacka, void * (*select) (void *base, size_t nmemb, size_t size, int (*compare) (const void *a, const void *b, void *context), void *context, size_t k), int (*compare) (const void *a, const void *b, void *context), void *context, size_t k);
