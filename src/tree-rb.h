
enum tree_rb_color {
	tree_rb_color_red,
	tree_rb_color_black,
};

struct tree_rb {
	struct tree_rb *left;
	struct tree_rb *right;
	size_t count;
	enum tree_rb_color color;
};

#define tree_rb_offsetof(type, member) ((size_t) &((type *) 0)->member)
#define tree_rb_containerof(ptr, type, member) ((void *) (((char *) ptr) - tree_rb_offsetof(type, member)))

int tree_rb_init (struct tree_rb *tree_rb);
struct tree_rb * tree_rb_del (struct tree_rb *tree_rb, void *key, int (*compare) (const void *a, const void *b, void *context), void *context);
struct tree_rb * tree_rb_del_min (struct tree_rb *tree_rb);
struct tree_rb * tree_rb_del_max (struct tree_rb *tree_rb);
struct tree_rb * tree_rb_add (struct tree_rb *tree_rb, struct tree_rb *item, int (*compare) (const void *a, const void *b, void *context), void *context);
int tree_rb_size (struct tree_rb *tree_rb);
int tree_rb_range_size (struct tree_rb *tree_rb, void *lo, void *hi, int (*compare) (const void *a, const void *b, void *context), void *context);
int tree_rb_rank (struct tree_rb *tree_rb, void *key, int (*compare) (const void *a, const void *b, void *context), void *context);
struct tree_rb * tree_rb_find (struct tree_rb *tree_rb, void *key, int (*compare) (const void *a, const void *b, void *context), void *context);
struct tree_rb * tree_rb_max (struct tree_rb *tree_rb);
struct tree_rb * tree_rb_min (struct tree_rb *tree_rb);
struct tree_rb * tree_rb_floor (struct tree_rb *tree_rb, void *key, int (*compare) (const void *a, const void *b, void *context), void *context);
struct tree_rb * tree_rb_ceiling (struct tree_rb *tree_rb, void *key, int (*compare) (const void *a, const void *b, void *context), void *context);
int tree_rb_iterate (struct tree_rb *tree_rb, int (*iterator) (void *context, struct tree_rb *item), void *context);
int tree_rb_iterate_range (struct tree_rb *tree_rb, void *lo, void *hi, int (*compare) (const void *a, const void *b, void *context), int (*iterator) (void *context, struct tree_rb *item), void *context);
int tree_rb_iterate_safe (struct tree_rb *tree_rb, int (*iterator) (void *context, struct tree_rb *item), void *context);
