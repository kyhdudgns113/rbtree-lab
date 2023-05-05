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

void lvr(node_t *);
void vlr(node_t *);
void lrv(node_t *);

static node_t *find_uncle(node_t *);
static node_t *find_grand(node_t *);
static node_t *create_node(rbtree *, node_t *, const int *, const key_t);
static node_t *find_parent_node(const rbtree *, const key_t, int *);

static node_t *insert_1_father_red_no_uncle_or_uncle_black(rbtree *, node_t *);
static node_t *insert_2_father_red_uncle_red(rbtree *, node_t *);

#endif  // _RBTREE_H_
