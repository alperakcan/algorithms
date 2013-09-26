
struct tree {
	struct tree *left;
	struct tree *right;
	size_t count;
};

#define tree_offsetof(type, member) ((size_t) &((type *) 0)->member)
#define tree_containerof(ptr, type, member) ((void *) (((char *) ptr) - tree_offsetof(type, member)))

int tree_init (struct tree *tree);
struct tree * tree_del (struct tree *tree, void *key, int (*compare) (const void *a, const void *b, void *context), void *context);
struct tree * tree_del_min (struct tree *tree);
struct tree * tree_del_max (struct tree *tree);
struct tree * tree_add (struct tree *tree, struct tree *item, int (*compare) (const void *a, const void *b, void *context), void *context);
int tree_size (struct tree *tree);
int tree_range_size (struct tree *tree, void *lo, void *hi, int (*compare) (const void *a, const void *b, void *context), void *context);
int tree_rank (struct tree *tree, void *key, int (*compare) (const void *a, const void *b, void *context), void *context);
struct tree * tree_find (struct tree *tree, void *key, int (*compare) (const void *a, const void *b, void *context), void *context);
struct tree * tree_max (struct tree *tree);
struct tree * tree_min (struct tree *tree);
struct tree * tree_floor (struct tree *tree, void *key, int (*compare) (const void *a, const void *b, void *context), void *context);
struct tree * tree_ceiling (struct tree *tree, void *key, int (*compare) (const void *a, const void *b, void *context), void *context);
int tree_iterate (struct tree *tree, int (*iterator) (void *context, struct tree *item), void *context);
int tree_iterate_range (struct tree *tree, void *lo, void *hi, int (*compare) (const void *a, const void *b, void *context), int (*iterator) (void *context, struct tree *item), void *context);
int tree_iterate_safe (struct tree *tree, int (*iterator) (void *context, struct tree *item), void *context);
