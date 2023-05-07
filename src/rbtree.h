#ifndef _RBTREE_H_
#define _RBTREE_H_

#include <stddef.h>
#include <stdio.h>

typedef enum { RBTREE_RED, RBTREE_BLACK } color_t;

typedef int key_t;

typedef struct node_t {
  color_t color;
  key_t key;
  struct node_t *parent, *left, *right;
} node_t;

typedef struct {
  node_t *root;
  node_t *nil;  // for sentinel
} rbtree;

rbtree *new_rbtree(void);
void delete_rbtree(rbtree *);

node_t *rbtree_insert(rbtree *, const key_t);
node_t *rbtree_find(const rbtree *, const key_t);
node_t *rbtree_min(const rbtree *);
node_t *rbtree_max(const rbtree *);
int rbtree_erase(rbtree *, node_t *);

int rbtree_to_array(const rbtree *, key_t *, const size_t);

void test_lvr(node_t *);
void test_vlr(node_t *);
void test_lrv(node_t *);

static void to_array_lvr(key_t *, node_t *, const size_t, int *);

static node_t *find_uncle(node_t *);
static node_t *find_grand(node_t *);
static node_t *find_sibling(node_t *);
static node_t *create_node(rbtree *, node_t *, const int *, const key_t);
static node_t *find_parent_node(const rbtree *, const key_t, int *);

static node_t *insert_1_father_red_no_uncle_or_uncle_black(rbtree *, node_t *);
static node_t *insert_2_father_red_uncle_red(rbtree *, node_t *);

static int erase_0_has_no_children(rbtree *, node_t *);
static int erase_right_0_m_is_red_or_son_is_red(node_t *);
static int erase_left_0_m_is_red_or_son_is_red(node_t *);
static int erase_right_1_1_m_is_black_and_sibling_and_its_son_blacks(node_t *);
static int erase_left_1_1_m_is_black_and_sibling_and_its_son_blacks(node_t *);
static int erase_right_x_2_s_is_black_and_its_right_is_red(rbtree *, node_t *);
static int erase_left_x_2_s_is_black_and_its_left_is_red(rbtree *, node_t *);
static int erase_right_x_3_s_is_black_and_its_lr_is_rb(rbtree *, node_t *);
static int erase_left_x_3_s_is_black_and_its_lr_is_br(rbtree *, node_t *);
static int erase_right_2_1_p_is_black_s_is_black(rbtree *, node_t *);
static int erase_left_2_1_p_is_black_s_is_black(rbtree *, node_t *);
static int erase_right_2_4_p_is_black_s_is_red(rbtree *, node_t *);
static int erase_left_2_4_p_is_black_s_is_red(rbtree *, node_t *);

int check_erase_condition(rbtree *, node_t *, color_t , color_t , color_t, color_t);
static int chain_right(node_t *);
static int chain_left(node_t *);

static void delete_node_lrv(node_t *);

#endif  // _RBTREE_H_
