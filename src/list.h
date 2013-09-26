
struct list {
	struct list *next;
	struct list *prev;
};

#define list_offsetof(type, member) ((size_t) &((type *) 0)->member)
#define list_containerof(ptr, type, member) ((void *) (((char *) ptr) - list_offsetof(type, member)))

int list_init (struct list *list);
int list_add (struct list *list, struct list *item);
int list_add_tail (struct list *list, struct list *item);
int list_del (struct list *list, struct list *item);
struct list * list_first (struct list *list);
int list_iterate (struct list *list, int (*iterator) (void *context, struct list *list, struct list *item), void *context);
int list_iterate_safe (struct list *list, int (*iterator) (void *context, struct list *list, struct list *item), void *context);
