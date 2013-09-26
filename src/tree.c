
#include <stdlib.h>

#include "tree.h"

int tree_init (struct tree *tree)
{
	if (tree == NULL) {
		return -1;
	}
	tree->left = NULL;
	tree->right = NULL;
	tree->count = 1;
	return 0;
}

int tree_size (struct tree *tree)
{
	if (tree == NULL) {
		return 0;
	}
	return tree->count;
}

int tree_range_size (struct tree *tree, void *lo, void *hi, int (*compare) (const void *a, const void *b, void *context), void *context)
{
	struct tree *node;
	node = tree_find(tree, hi, compare, context);
	if (node != NULL) {
		return tree_rank(tree, hi, compare, context) - tree_rank(tree, lo, compare, context) + 1;
	} else {
		return tree_rank(tree, hi, compare, context) - tree_rank(tree, lo, compare, context);
	}
}

struct tree * tree_add (struct tree *tree, struct tree *item, int (*compare) (const void *a, const void *b, void *context), void *context)
{
	int cmp;
	if (tree == NULL) {
		return item;
	}
	if (compare == NULL) {
		return NULL;
	}
	tree_init(item);
	cmp = compare(item, tree, context);
	if (cmp < 0) {
		tree->left = tree_add(tree->left, item, compare, context);
	} else if (cmp > 0) {
		tree->right = tree_add(tree->right, item, compare, context);
	} else {
		return tree;
	}
	tree->count = 1 + tree_size(tree->left) + tree_size(tree->right);
	return tree;
}

struct tree * tree_del (struct tree *tree, void *key, int (*compare) (const void *a, const void *b, void *context), void *context)
{
	/* hibbard deletion
	 *
	 * the problem is, lack of symmetry. taking the successor all the time tree is going to be much less balanced.
	 * is not suitable for dynamic situations
	 *
	 * complexity of sqrt(n)
	 */
	int cmp;
	struct tree *node;
	if (tree == NULL) {
		return NULL;
	}
	/* search key */
	cmp = compare(key, tree, context);
	if (cmp < 0) {
		tree->left = tree_del(tree->left, key, compare, context);
	} else if (cmp > 0) {
		tree->right = tree_del(tree->right, key, compare, context);
	} else {
		/* found key */
		if (tree->right == NULL) {
			return tree->left;
		}
		/* replace with successor */
		node = tree;
		tree = tree_min(node->right);
		tree->right = tree_del_min(node->right);
		tree->left = node->left;
	}
	/* update subtree counts */
	tree->count = 1 + tree_size(tree->left) + tree_size(tree->right);
	return tree;
}

struct tree * tree_del_min (struct tree *tree)
{
	if (tree == NULL) {
		return NULL;
	}
	if (tree->left == NULL) {
		return tree->right;
	}
	tree->left = tree_del_min(tree->left);
	tree->count = 1 + tree_size(tree->left) + tree_size(tree->right);
	return tree;
}

struct tree * tree_del_max (struct tree *tree)
{
	if (tree == NULL) {
		return NULL;
	}
	if (tree->right == NULL) {
		return tree->left;
	}
	tree->right = tree_del_max(tree->right);
	tree->count = 1 + tree_size(tree->left) + tree_size(tree->right);
	return tree;
}

int tree_rank (struct tree *tree, void *key, int (*compare) (const void *a, const void *b, void *context), void *context)
{
	int cmp;
	if (tree == NULL) {
		return 0;
	}
	cmp = compare(key, tree, context);
	if (cmp < 0) {
		return tree_rank(tree->left, key, compare, context);
	} else if (cmp > 0) {
		return 1 + tree_size(tree->left) + tree_rank(tree->right, key, compare, context);
	} else {
		return tree_size(tree->left);
	}
}

struct tree * tree_find (struct tree *tree, void *key, int (*compare) (const void *a, const void *b, void *context), void *context)
{
	int cmp;
	struct tree *root;
	if (tree == NULL) {
		return NULL;
	}
	if (compare == NULL) {
		return NULL;
	}
	root = tree;
	while (root != NULL) {
		cmp = compare(key, root, context);
		if (cmp < 0) {
			root = root->left;
		} else if (cmp > 0) {
			root = root->right;
		} else {
			return root;
		}
	}
	return NULL;
}

struct tree * tree_max (struct tree *tree)
{
	while (tree != NULL) {
		if (tree->right == NULL) {
			break;
		}
		tree = tree->right;
	}
	return tree;
}

struct tree * tree_min (struct tree *tree)
{
	while (tree != NULL) {
		if (tree->left == NULL) {
			break;
		}
		tree = tree->left;
	}
	return tree;
}

struct tree * tree_floor (struct tree *tree, void *key, int (*compare) (const void *a, const void *b, void *context), void *context)
{
	int cmp;
	struct tree *item;
	if (tree == NULL) {
		return NULL;
	}
	cmp = compare(key, tree, context);
	if (cmp == 0) {
		return tree;
	}
	if (cmp < 0) {
		return tree_floor(tree->left, key, compare, context);
	}
	item = tree_floor(tree->right, key, compare, context);
	if (item != NULL) {
		return item;
	} else {
		return tree;
	}
}

struct tree * tree_ceiling (struct tree *tree, void *key, int (*compare) (const void *a, const void *b, void *context), void *context)
{
	int cmp;
	struct tree *item;
	if (tree == NULL) {
		return NULL;
	}
	cmp = compare(key, tree, context);
	if (cmp == 0) {
		return tree;
	}
	if (cmp > 0) {
		return tree_ceiling(tree->left, key, compare, context);
	}
	item = tree_ceiling(tree->right, key, compare, context);
	if (item != NULL) {
		return item;
	} else {
		return tree;
	}
}

int tree_iterate (struct tree *tree, int (*iterator) (void *context, struct tree *item), void *context)
{
	if (tree == NULL) {
		return -1;
	}
	tree_iterate(tree->left, iterator, context);
	iterator(context, tree);
	tree_iterate(tree->right, iterator, context);
	return 0;
}

int tree_iterate_range (struct tree *tree, void *lo, void *hi, int (*compare) (const void *a, const void *b, void *context), int (*iterator) (void *context, struct tree *item), void *context)
{
	int cmp_lo;
	int cmp_hi;
	if (tree == NULL) {
		return -1;
	}
	cmp_lo = compare(lo, tree, context);
	cmp_hi = compare(hi, tree, context);
	if (cmp_lo < 0) {
		tree_iterate_range(tree->left, lo, hi, compare, iterator, context);
	}
	if ((cmp_lo <= 0) &&
	    (cmp_hi >= 0)) {
		iterator(context, tree);
	}
	if (cmp_hi > 0) {
		tree_iterate_range(tree->right, lo, hi, compare, iterator, context);
	}
	return 0;
}

int tree_iterate_safe (struct tree *tree, int (*iterator) (void *context, struct tree *item), void *context)
{
	if (tree == NULL) {
		return -1;
	}
	tree_iterate_safe(tree->left, iterator, context);
	tree_iterate_safe(tree->right, iterator, context);
	iterator(context, tree);
	return 0;
}
