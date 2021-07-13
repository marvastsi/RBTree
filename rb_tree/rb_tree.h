#ifndef __RB_TREE_H__
#define __RB_TREE_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define MAXIMUM(X, Y) ((X >= Y) ? X : Y)

typedef enum rb_color_e
{
	RED = 0,
	BLACK
} rb_color_t;

typedef struct rb_tree_s
{
	int n;
	rb_color_t color;
	struct rb_tree_s *parent;
	struct rb_tree_s *left;
	struct rb_tree_s *right;
} rb_tree_t;

/* Implementação de acordo com o livro de algoritmos CORMEN et. al. */
rb_tree_t *rb_tree_node(rb_tree_t *parent, int value, rb_color_t color);

bool rb_tree_is_valid(const rb_tree_t *tree);
bool is_bst(const rb_tree_t *tree, int min, int max);
bool correct_colors(const rb_tree_t *tree);
int find_height(const rb_tree_t *tree);
int check_rb_height(const rb_tree_t *tree, int blk_height, int height);

rb_tree_t *rb_tree_insert(rb_tree_t **tree, int value);
void rb_insert_fixup(rb_tree_t **tree, rb_tree_t *new);
rb_tree_t *rb_insert_fixup_LEFT(rb_tree_t **tree, rb_tree_t *new);
rb_tree_t *rb_insert_fixup_RIGHT(rb_tree_t **tree, rb_tree_t *new);
void rb_rotate_left(rb_tree_t **tree, rb_tree_t *new);
void rb_rotate_right(rb_tree_t **tree, rb_tree_t *new);

rb_tree_t *rb_tree_remove(rb_tree_t *root, int n);
rb_tree_t *rb_tree_delete(rb_tree_t *root, rb_tree_t *remove);
void rb_transplant(rb_tree_t **root, rb_tree_t *x, rb_tree_t *y);
rb_tree_t *rb_delete_fixup(rb_tree_t *root, rb_tree_t *x);
rb_tree_t *rb_delete_fix_left(rb_tree_t **root, rb_tree_t *x);
rb_tree_t *rb_delete_fix_right(rb_tree_t **root, rb_tree_t *x);
rb_tree_t *tree_min(rb_tree_t *root);

bool rb_tree_search(rb_tree_t *tree, int value);

#endif /*__RB_TREE_H__ */
