
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree-rb.h"

struct item {
	struct tree_rb tree_rb;
	const char *string;
};

static int iterator (void *context, struct tree_rb *item)
{
	struct item *i;
	(void) context;
	i = tree_rb_containerof(item, struct item, tree_rb);
	printf("%s ", i->string);
	return 0;
}

static int destroy (void *context, struct tree_rb *item)
{
	struct item *i;
	(void) context;
	i = tree_rb_containerof(item, struct item, tree_rb);
	free(i);
	return 0;
}

static int compare (const void *a, const void *b, void *context)
{
	int v1;
	int v2;
	const struct item *i1;
	const struct item *i2;
	(void) context;
	i1 = tree_rb_containerof(a, struct item, tree_rb);
	i2 = tree_rb_containerof(b, struct item, tree_rb);
	v1 = strtol(i1->string, NULL, 10);
	v2 = strtol(i2->string, NULL, 10);
	if (v1 < v2) {
		return -1;
	}
	if (v1 > v2) {
		return 1;
	}
	return 0;
}

int main (int argc, char *argv[])
{
	int i;
	int rank;
	int range;
	struct tree_rb *tree_rb;
	struct tree_rb *node;
	struct item *item;
	struct item key;
	struct item lo;
	struct item hi;
	tree_rb = NULL;
	for (i = 1; i < argc; i++) {
		key.string = argv[i];
		node = tree_rb_find(tree_rb, &key, compare, NULL);
		if (node != NULL) {
			continue;
		}
		item = malloc(sizeof(*item));
		if (item == NULL) {
			continue;
		}
		tree_rb_init(&item->tree_rb);
		item->string = argv[i];
		if (i == 1) {
			tree_rb = &item->tree_rb;
		} else {
			tree_rb = tree_rb_add(tree_rb, &item->tree_rb, compare, NULL);
		}
	}

	node = tree_rb_min(tree_rb);
	if (node != NULL) {
		printf("%s ", ((struct item *) tree_rb_containerof(node, struct item, tree_rb))->string);
	}

	node = tree_rb_max(tree_rb);
	if (node != NULL) {
		printf("%s ", ((struct item *) tree_rb_containerof(node, struct item, tree_rb))->string);
	}

	key.string = argv[2];
	rank = tree_rb_rank(tree_rb, &key, compare, NULL);
	printf("rank %s: %d ", key.string, rank);

	key.string = argv[10];
	rank = tree_rb_rank(tree_rb, &key, compare, NULL);
	printf("rank %s: %d ", key.string, rank);

	tree_rb_iterate(tree_rb, iterator, NULL);

	lo.string = argv[2];
	hi.string = argv[10];
	range = tree_rb_range_size(tree_rb, &lo, &hi, compare, NULL);
	printf("range %s, %s: %d ", lo.string, hi.string, range);

	tree_rb_iterate_range(tree_rb, &lo, &hi, compare, iterator, NULL);

	key.string = argv[2];
	node = tree_rb_find(tree_rb, &key, compare, NULL);
	tree_rb = tree_rb_del(tree_rb, &key, compare, NULL);
	free(tree_rb_containerof(node, struct item, tree_rb));
	tree_rb_iterate(tree_rb, iterator, NULL);

	node = tree_rb_min(tree_rb);
	tree_rb = tree_rb_del_min(tree_rb);
	free(tree_rb_containerof(node, struct item, tree_rb));
	tree_rb_iterate(tree_rb, iterator, NULL);

	node = tree_rb_max(tree_rb);
	tree_rb = tree_rb_del_max(tree_rb);
	free(tree_rb_containerof(node, struct item, tree_rb));
	tree_rb_iterate(tree_rb, iterator, NULL);

	tree_rb_iterate_safe(tree_rb, destroy, NULL);
	return 0;
}
