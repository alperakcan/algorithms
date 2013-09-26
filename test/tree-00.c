
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"

struct item {
	struct tree tree;
	const char *string;
};

static int iterator (void *context, struct tree *item)
{
	struct item *i;
	(void) context;
	i = tree_containerof(item, struct item, tree);
	printf("%s ", i->string);
	return 0;
}

static int destroy (void *context, struct tree *item)
{
	struct item *i;
	(void) context;
	i = tree_containerof(item, struct item, tree);
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
	i1 = tree_containerof(a, struct item, tree);
	i2 = tree_containerof(b, struct item, tree);
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
	struct tree *tree;
	struct tree *node;
	struct item *item;
	struct item key;
	struct item lo;
	struct item hi;
	tree = NULL;
	for (i = 1; i < argc; i++) {
		key.string = argv[i];
		node = tree_find(tree, &key, compare, NULL);
		if (node != NULL) {
			continue;
		}
		item = malloc(sizeof(*item));
		if (item == NULL) {
			continue;
		}
		tree_init(&item->tree);
		item->string = argv[i];
		if (i == 1) {
			tree = &item->tree;
		} else {
			tree = tree_add(tree, &item->tree, compare, NULL);
		}
	}

	node = tree_min(tree);
	if (node != NULL) {
		printf("%s ", ((struct item *) tree_containerof(node, struct item, tree))->string);
	}

	node = tree_max(tree);
	if (node != NULL) {
		printf("%s ", ((struct item *) tree_containerof(node, struct item, tree))->string);
	}

	key.string = argv[2];
	rank = tree_rank(tree, &key, compare, NULL);
	printf("rank %s: %d ", key.string, rank);

	key.string = argv[10];
	rank = tree_rank(tree, &key, compare, NULL);
	printf("rank %s: %d ", key.string, rank);

	tree_iterate(tree, iterator, NULL);

	lo.string = argv[2];
	hi.string = argv[10];
	range = tree_range_size(tree, &lo, &hi, compare, NULL);
	printf("range %s, %s: %d ", lo.string, hi.string, range);

	tree_iterate_range(tree, &lo, &hi, compare, iterator, NULL);

	key.string = argv[2];
	node = tree_find(tree, &key, compare, NULL);
	tree = tree_del(tree, &key, compare, NULL);
	free(tree_containerof(node, struct item, tree));
	tree_iterate(tree, iterator, NULL);

	node = tree_min(tree);
	tree = tree_del_min(tree);
	free(tree_containerof(node, struct item, tree));
	tree_iterate(tree, iterator, NULL);

	node = tree_max(tree);
	tree = tree_del_max(tree);
	free(tree_containerof(node, struct item, tree));
	tree_iterate(tree, iterator, NULL);

	tree_iterate_safe(tree, destroy, NULL);
	return 0;
}
