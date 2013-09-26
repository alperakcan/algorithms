
#include <stdlib.h>

#include "tree-rb.h"

static int tree_rb_is_red (struct tree_rb *tree_rb)
{
	if (tree_rb == NULL) {
		return 0;
	}
	if (tree_rb->color == tree_rb_color_red) {
		return 1;
	}
	return 0;
}

static struct tree_rb * tree_rb_rotate_left (struct tree_rb *h)
{
	struct tree_rb *x;
	x = h->right;
	h->right = x->left;
	x->left = h;
	x->color = h->color;
	h->color = tree_rb_color_red;
	return x;
}

static struct tree_rb * tree_rb_rotate_right (struct tree_rb *h)
{
	struct tree_rb *x;
	x = h->left;
	h->left = x->right;
	x->right = h;
	x->color = h->color;
	h->color = tree_rb_color_red;
	return x;
}

static void tree_rb_flip_colors (struct tree_rb *h)
{
	h->color = tree_rb_color_red;
	h->left->color = tree_rb_color_black;
	h->right->color = tree_rb_color_black;
}

int tree_rb_init (struct tree_rb *tree_rb)
{
	if (tree_rb == NULL) {
		return -1;
	}
	tree_rb->left = NULL;
	tree_rb->right = NULL;
	tree_rb->count = 1;
	tree_rb->color = tree_rb_color_red;
	return 0;
}

int tree_rb_size (struct tree_rb *tree_rb)
{
	if (tree_rb == NULL) {
		return 0;
	}
	return tree_rb->count;
}

int tree_rb_range_size (struct tree_rb *tree_rb, void *lo, void *hi, int (*compare) (const void *a, const void *b, void *context), void *context)
{
	struct tree_rb *node;
	node = tree_rb_find(tree_rb, hi, compare, context);
	if (node != NULL) {
		return tree_rb_rank(tree_rb, hi, compare, context) - tree_rb_rank(tree_rb, lo, compare, context) + 1;
	} else {
		return tree_rb_rank(tree_rb, hi, compare, context) - tree_rb_rank(tree_rb, lo, compare, context);
	}
}

struct tree_rb * tree_rb_add (struct tree_rb *tree_rb, struct tree_rb *item, int (*compare) (const void *a, const void *b, void *context), void *context)
{
	int cmp;
	if (tree_rb == NULL) {
		return item;
	}
	if (compare == NULL) {
		return NULL;
	}
	tree_rb_init(item);
	cmp = compare(item, tree_rb, context);
	if (cmp < 0) {
		tree_rb->left = tree_rb_add(tree_rb->left, item, compare, context);
	} else if (cmp > 0) {
		tree_rb->right = tree_rb_add(tree_rb->right, item, compare, context);
	} else {
		return tree_rb;
	}
	tree_rb->count = 1 + tree_rb_size(tree_rb->left) + tree_rb_size(tree_rb->right);
	if (tree_rb_is_red(tree_rb->right) &&
	    !tree_rb_is_red(tree_rb->left)) {
		/* lean left */
		tree_rb = tree_rb_rotate_left(tree_rb);
	}
	if (tree_rb_is_red(tree_rb->left) &&
	    tree_rb_is_red(tree_rb->left->left)) {
		/* balance 4 node */
		tree_rb = tree_rb_rotate_right(tree_rb);
	}
	if (tree_rb_is_red(tree_rb->right) &&
	    tree_rb_is_red(tree_rb->left)) {
		/* split 4 node */
		tree_rb_flip_colors(tree_rb);
	}
	tree_rb->count = 1 + tree_rb_size(tree_rb->left) + tree_rb_size(tree_rb->right);
	return tree_rb;
}

struct tree_rb * tree_rb_del (struct tree_rb *tree_rb, void *key, int (*compare) (const void *a, const void *b, void *context), void *context)
{
	/* hibbard deletion
	 *
	 * the problem is, lack of symmetry. taking the successor all the time tree_rb is going to be much less balanced.
	 * is not suitable for dynamic situations
	 *
	 * complexity of sqrt(n)
	 */
	int cmp;
	struct tree_rb *node;
	if (tree_rb == NULL) {
		return NULL;
	}
	/* search key */
	cmp = compare(key, tree_rb, context);
	if (cmp < 0) {
		tree_rb->left = tree_rb_del(tree_rb->left, key, compare, context);
	} else if (cmp > 0) {
		tree_rb->right = tree_rb_del(tree_rb->right, key, compare, context);
	} else {
		/* found key */
		if (tree_rb->right == NULL) {
			return tree_rb->left;
		}
		/* replace with successor */
		node = tree_rb;
		tree_rb = tree_rb_min(node->right);
		tree_rb->right = tree_rb_del_min(node->right);
		tree_rb->left = node->left;
	}
	/* update subtree_rb counts */
	tree_rb->count = 1 + tree_rb_size(tree_rb->left) + tree_rb_size(tree_rb->right);
	return tree_rb;
}

struct tree_rb * tree_rb_del_min (struct tree_rb *tree_rb)
{
	if (tree_rb == NULL) {
		return NULL;
	}
	if (tree_rb->left == NULL) {
		return tree_rb->right;
	}
	tree_rb->left = tree_rb_del_min(tree_rb->left);
	tree_rb->count = 1 + tree_rb_size(tree_rb->left) + tree_rb_size(tree_rb->right);
	return tree_rb;
}

struct tree_rb * tree_rb_del_max (struct tree_rb *tree_rb)
{
	if (tree_rb == NULL) {
		return NULL;
	}
	if (tree_rb->right == NULL) {
		return tree_rb->left;
	}
	tree_rb->right = tree_rb_del_max(tree_rb->right);
	tree_rb->count = 1 + tree_rb_size(tree_rb->left) + tree_rb_size(tree_rb->right);
	return tree_rb;
}

int tree_rb_rank (struct tree_rb *tree_rb, void *key, int (*compare) (const void *a, const void *b, void *context), void *context)
{
	int cmp;
	if (tree_rb == NULL) {
		return 0;
	}
	cmp = compare(key, tree_rb, context);
	if (cmp < 0) {
		return tree_rb_rank(tree_rb->left, key, compare, context);
	} else if (cmp > 0) {
		return 1 + tree_rb_size(tree_rb->left) + tree_rb_rank(tree_rb->right, key, compare, context);
	} else {
		return tree_rb_size(tree_rb->left);
	}
}

struct tree_rb * tree_rb_find (struct tree_rb *tree_rb, void *key, int (*compare) (const void *a, const void *b, void *context), void *context)
{
	int cmp;
	struct tree_rb *root;
	/* is gonna run much faster because of the balance */
	if (tree_rb == NULL) {
		return NULL;
	}
	if (compare == NULL) {
		return NULL;
	}
	root = tree_rb;
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

struct tree_rb * tree_rb_max (struct tree_rb *tree_rb)
{
	while (tree_rb != NULL) {
		if (tree_rb->right == NULL) {
			break;
		}
		tree_rb = tree_rb->right;
	}
	return tree_rb;
}

struct tree_rb * tree_rb_min (struct tree_rb *tree_rb)
{
	while (tree_rb != NULL) {
		if (tree_rb->left == NULL) {
			break;
		}
		tree_rb = tree_rb->left;
	}
	return tree_rb;
}

struct tree_rb * tree_rb_floor (struct tree_rb *tree_rb, void *key, int (*compare) (const void *a, const void *b, void *context), void *context)
{
	int cmp;
	struct tree_rb *item;
	if (tree_rb == NULL) {
		return NULL;
	}
	cmp = compare(key, tree_rb, context);
	if (cmp == 0) {
		return tree_rb;
	}
	if (cmp < 0) {
		return tree_rb_floor(tree_rb->left, key, compare, context);
	}
	item = tree_rb_floor(tree_rb->right, key, compare, context);
	if (item != NULL) {
		return item;
	} else {
		return tree_rb;
	}
}

struct tree_rb * tree_rb_ceiling (struct tree_rb *tree_rb, void *key, int (*compare) (const void *a, const void *b, void *context), void *context)
{
	int cmp;
	struct tree_rb *item;
	if (tree_rb == NULL) {
		return NULL;
	}
	cmp = compare(key, tree_rb, context);
	if (cmp == 0) {
		return tree_rb;
	}
	if (cmp > 0) {
		return tree_rb_ceiling(tree_rb->left, key, compare, context);
	}
	item = tree_rb_ceiling(tree_rb->right, key, compare, context);
	if (item != NULL) {
		return item;
	} else {
		return tree_rb;
	}
}

int tree_rb_iterate (struct tree_rb *tree_rb, int (*iterator) (void *context, struct tree_rb *item), void *context)
{
	if (tree_rb == NULL) {
		return -1;
	}
	tree_rb_iterate(tree_rb->left, iterator, context);
	iterator(context, tree_rb);
	tree_rb_iterate(tree_rb->right, iterator, context);
	return 0;
}

int tree_rb_iterate_range (struct tree_rb *tree_rb, void *lo, void *hi, int (*compare) (const void *a, const void *b, void *context), int (*iterator) (void *context, struct tree_rb *item), void *context)
{
	int cmp_lo;
	int cmp_hi;
	if (tree_rb == NULL) {
		return -1;
	}
	cmp_lo = compare(lo, tree_rb, context);
	cmp_hi = compare(hi, tree_rb, context);
	if (cmp_lo < 0) {
		tree_rb_iterate_range(tree_rb->left, lo, hi, compare, iterator, context);
	}
	if ((cmp_lo <= 0) &&
	    (cmp_hi >= 0)) {
		iterator(context, tree_rb);
	}
	if (cmp_hi > 0) {
		tree_rb_iterate_range(tree_rb->right, lo, hi, compare, iterator, context);
	}
	return 0;
}

int tree_rb_iterate_safe (struct tree_rb *tree_rb, int (*iterator) (void *context, struct tree_rb *item), void *context)
{
	if (tree_rb == NULL) {
		return -1;
	}
	tree_rb_iterate_safe(tree_rb->left, iterator, context);
	tree_rb_iterate_safe(tree_rb->right, iterator, context);
	iterator(context, tree_rb);
	return 0;
}
