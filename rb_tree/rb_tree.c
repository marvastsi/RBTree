#include <stdbool.h>
#include "rb_tree.h"

rb_tree_t *rb_tree_node(rb_tree_t *parent, int value, rb_color_t color)
{
	rb_tree_t *new;

	new = malloc(sizeof(rb_tree_t));
	if (!new)
		return (NULL);

	new->parent = parent;
	new->n = value;
	new->color = color;
	new->left = NULL;
	new->right = NULL;

	return (new);
}

bool rb_tree_is_valid(const rb_tree_t *tree)
{
	int height;

	if (!tree)
		return (false);

	if (tree->color != BLACK)
		return (false);

	if (!is_bst(tree, 0, 0))
		return (false);

	if (!correct_colors(tree))
		return (false);

	height = find_height(tree);

	if (!check_rb_height(tree, height, 0))
		return (false);

	return (true);
}

bool is_bst(const rb_tree_t *tree, int min, int max)
{
	if (!tree)
		return (true);

	if ((min && tree->n < min) || (max && tree->n > max))
		return (false);

	return (is_bst(tree->left, 0, tree->n) &&
		 is_bst(tree->right, tree->n, 0));
}

bool correct_colors(const rb_tree_t *tree)
{
	if (!tree)
		return (true);

	if (tree->color != RED && tree->color != BLACK)
		return (false);
	if (tree->color == RED)
	{
		if (tree->left && (tree->left)->color == RED)
			return (false);
		if (tree->right && (tree->right)->color == RED)
			return (false);
	}
	return (correct_colors(tree->left) && correct_colors(tree->right));
}

int find_height(const rb_tree_t *tree)
{
	int height;

	if (!tree)
		return (0);

	height = MAXIMUM(find_height(tree->left), find_height(tree->right));

	if (tree->color == BLACK)
		height++;
	return (height);
}

int check_rb_height(const rb_tree_t *tree, int blk_height, int height)
{
	if (!tree)
	{
		if (height == blk_height)
			return (1);
		return (0);
	}
	if (tree->color == BLACK)
		height++;

	return (check_rb_height(tree->left, blk_height, height) &&
		check_rb_height(tree->right, blk_height, height));
}

rb_tree_t *rb_tree_insert(rb_tree_t **tree, int value)
{
	rb_tree_t *x = NULL;
	rb_tree_t *y = NULL;
	rb_tree_t *new = NULL;

	if (!tree)
		return (NULL);

	x = *tree;
	while (x != NULL)
	{
		y = x;
		if (x->n == value)
			return (NULL);
		if (value < x->n)
			x = x->left;
		else if (value > x->n)
			x = x->right;
	}
	new = rb_tree_node(y, value, RED);
	if (!new)
		return (NULL);

	if (y == NULL)
		*tree = new;
	else if (new->n < y->n)
		y->left = new;
	else
		y->right = new;
	rb_insert_fixup(tree, new);
	return (new);
}

void rb_insert_fixup(rb_tree_t **tree, rb_tree_t *new)
{
	while (new->parent && new->parent->color == RED)
	{
		if (new->parent->parent && new->parent == new->parent->parent->left)
			new = rb_insert_fixup_RIGHT(tree, new);
		else
			new = rb_insert_fixup_LEFT(tree, new);
	}
	(*tree)->color = BLACK;
}

rb_tree_t *rb_insert_fixup_LEFT(rb_tree_t **tree, rb_tree_t *new)
{
	rb_tree_t *y = NULL;

	y = new->parent->parent->left;
	if (y && y->color == RED)
	{
		new->parent->color = BLACK;
		y->color = BLACK;
		new->parent->parent->color = RED;
		new = new->parent->parent;
	}
	else
	{
		if (new == new->parent->left)
		{
			new = new->parent;
			rb_rotate_right(tree, new);
		}
		new->parent->color = BLACK;
		new->parent->parent->color = RED;
		rb_rotate_left(tree, new->parent->parent);
	}
	return (new);
}

rb_tree_t *rb_insert_fixup_RIGHT(rb_tree_t **tree, rb_tree_t *new)
{
	rb_tree_t *y = NULL;

	y = new->parent->parent->right;
	if (y && y->color == RED)
	{
		new->parent->color = BLACK;
		y->color = BLACK;
		new->parent->parent->color = RED;
		new = new->parent->parent;
	}
	else
	{
		if (new == new->parent->right)
		{
			new = new->parent;
			rb_rotate_left(tree, new);
		}
		new->parent->color = BLACK;
		new->parent->parent->color = RED;
		rb_rotate_right(tree, new->parent->parent);
	}
	return (new);
}

void rb_rotate_left(rb_tree_t **tree, rb_tree_t *x)
{
	rb_tree_t *y = NULL;

	y = x->right;
	x->right = y->left;
	if (y->left)
		y->left->parent = x;
	y->parent = x->parent;
	if (x->parent == NULL)
		*tree = y;
	else if (x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;
	y->left = x;
	x->parent = y;
}

void rb_rotate_right(rb_tree_t **tree, rb_tree_t *y)
{
	rb_tree_t *x = NULL;

	x = y->left;
	y->left = x->right;
	if (x->right)
		x->right->parent = y;
	x->parent = y->parent;
	if (y->parent == NULL)
		*tree = x;
	else if (y == y->parent->left)
		y->parent->left = x;
	else
		y->parent->right = x;
	x->right = y;
	y->parent = x;
}

rb_tree_t *rb_tree_remove(rb_tree_t *root, int n)
{
	rb_tree_t *remove = root;

	while (remove && remove->n != n)
	{
		if (remove->n > n)
			remove = remove->left;
		else
			remove = remove->right;
	}
	return (rb_tree_delete(root, remove));
}

rb_tree_t *rb_tree_delete(rb_tree_t *root, rb_tree_t *remove)
{
	rb_tree_t *x = NULL;
	rb_tree_t *y = remove;
	rb_color_t og_color = remove->color;

	if (remove->left == NULL)
	{
		x = remove->right;
		rb_transplant(&root, remove, remove->right);
	}
	else if (remove->right == NULL)
	{
		x = remove->left;
		rb_transplant(&root, remove, remove->left);
	}
	else
	{
		y = tree_min(remove->right);
		if (y->color)
			og_color = y->color;
		x = y->right;
		if (y->parent && y->parent == remove)
			x->parent = y;
		else
		{
			rb_transplant(&root, y, y->right);
			y->right = remove->right;
			y->right->parent = y;
		}
		rb_transplant(&root, remove, y);
		y->left = remove->left;
		y->left->parent = y;
		y->color = remove->color;
	}
	if (og_color == BLACK)
		rb_delete_fixup(root, x);
	return (root);
}

void rb_transplant(rb_tree_t **root, rb_tree_t *x, rb_tree_t *y)
{
	if (x->parent == NULL)
		*root = y;
	else if (x->parent->left && x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;
	if (y)
		y->parent = x->parent;
}

rb_tree_t *rb_delete_fixup(rb_tree_t *root, rb_tree_t *x)
{
	while (x != NULL && x->color == BLACK)
	{
		if (x == x->parent->left)
			x = rb_delete_fix_right(&root, x);
		else
			x = rb_delete_fix_left(&root, x);
	}
	if (x)
		x->color = BLACK;
	return (root);
}

rb_tree_t *rb_delete_fix_left(rb_tree_t **root, rb_tree_t *x)
{
	rb_tree_t *y;

	y = x->parent->left;
	/* CASO 1 */
	if (y && y->color == RED)
	{
		y->color = BLACK;
		x->parent->color = RED;
		rb_rotate_right(root, x->parent);
		y = x->parent->left;
	}
	/* CASO 2 */
	if (y && y->right->color == BLACK && y->left->color == BLACK)
	{
		y->color = RED;
		x = x->parent;
	}
	/* CASO 3 */
	else if (y && y->left->color == BLACK)
	{
		y->right->color = BLACK;
		y->color = RED;
		rb_rotate_left(root, y);
		y = x->parent->left;
	}
	/* CASO 4 */
	y->color = x->parent->color;
	x->parent->color = BLACK;
	y->right->color = BLACK;
	rb_rotate_right(root, x->parent);
	x = *root;

	return (x);
}

rb_tree_t *rb_delete_fix_right(rb_tree_t **root, rb_tree_t *x)
{
	rb_tree_t *y = NULL;

	y = x->parent->right;
	/* CASO 1 */
	if (y && y->color == RED)
	{
		y->color = BLACK;
		x->parent->color = RED;
		rb_rotate_left(root, x->parent);
		y = x->parent->right;
	}
	/* CASO 2 */
	if (y && y->left->color == BLACK && y->right->color == BLACK)
	{
		y->color = RED;
		x = x->parent;
	}
	/* CASO 3 */
	else if (y && y->right->color == BLACK)
	{
		y->left->color = BLACK;
		y->color = RED;
		rb_rotate_right(root, y);
		y = x->parent->right;
	}
	/* CASO 4 */
	y->color = x->parent->color;
	x->parent->color = BLACK;
	y->right->color = BLACK;
	rb_rotate_left(root, x->parent);
	x = *root;

	return (x);
}

rb_tree_t *tree_min(rb_tree_t *root)
{
	while (root->left)
		root = root->left;
	return (root);
}

bool rb_tree_search(rb_tree_t *tree, int value)
{
    if (tree == NULL) {
        return false;
    }

    if (value == tree->n) {
        return true;
    } else {
        if (value > tree->n) {
            return rb_tree_search(tree->right, value);
        } else {
            return rb_tree_search(tree->left, value);
        }
    }
}