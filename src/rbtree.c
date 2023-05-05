#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  p->root = NULL;
  p->nil = NULL;
  // TODO: initialize struct if needed
  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *parent_node = NULL;
  node_t *new_node = NULL;
  node_t *uncle = NULL;
  node_t *grand = NULL;
  node_t *now_node = NULL;

  int is_left = 1;

  if (t == NULL)
    return NULL;
  parent_node = rbtree_find(t, key);
  if (parent_node)
    return parent_node;
  parent_node = find_parent_node(t, key, &is_left);
  // new_node 가 root 인 경우이다.
  if (parent_node == NULL) {
    new_node = create_node(t, NULL, &is_left, key);
    return new_node;
  }
  new_node = create_node(t, parent_node, &is_left, key);
  uncle = find_uncle(new_node);
  grand = find_grand(new_node);
  if (parent_node->color == RBTREE_RED) {
    if (uncle == NULL || uncle->color == RBTREE_BLACK) {
      insert_1_father_red_no_uncle_or_uncle_black(t, new_node);
      return new_node;
    }

    insert_2_father_red_uncle_red(t, new_node);
  }
  return new_node;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  
  node_t *now_node = t->root;

  while (now_node != NULL) {
    if (now_node->key == key)
      break;
    else if (key < now_node->key)
      now_node = now_node->left;
    else
      now_node = now_node->right;
  }

  return now_node;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}

void lvr(node_t *node) {
  if (node->left)
    lvr(node->left);
  printf("LVR %d is %s\n", node->key, node->color == RBTREE_BLACK ? "BLACK" : "RED");
  if (node->right)
    lvr(node->right);
}

void vlr(node_t *node) {
  printf("VLR %d is %s\n", node->key, node->color == RBTREE_BLACK ? "BLACK" : "RED");
  if (node->left)
    vlr(node->left);
  if (node->right)
    vlr(node->right);
}

void lrv(node_t *node) {
  if (node->left)
    lrv(node->left);
  if (node->right)
    lrv(node->right);
  printf("LRV %d is %s\n", node->key, node->color == RBTREE_BLACK ? "BLACK" : "RED");
}

static node_t *find_uncle(node_t *now_node) {
  node_t *grand = find_grand(now_node);

  if (grand == NULL)
    return NULL;

  if (now_node->parent == grand->left)
    return grand->right;

  return grand->left;
}

static node_t *find_grand(node_t *now_node) {
  if (now_node == NULL || now_node->parent == NULL || now_node->parent->parent == NULL)
    return NULL;

  return now_node->parent->parent;
}

//
//  입력 :
//    t : 트리
//    parent : node 를 넣어줄 parent
//    is_left : 왼쪽에 넣을지 여부를 결젖ㅇ
//    key : new node 의 key 값
//
//  출력 :
//    새로 만들어진 노드
//
static node_t *create_node(rbtree *t, node_t *parent, const int *is_left, const key_t key) {
  node_t *new_node = (node_t *)malloc(sizeof(node_t));
  new_node->color = RBTREE_RED;
  new_node->key = key;
  new_node->parent = parent;
  new_node->left = new_node->right = NULL;

  if (parent) {
    if (*is_left) {
      parent->left = new_node;
    }
    else {
      parent->right = new_node;
    }
  }
  else {
    new_node->color = RBTREE_BLACK;
    t->root = new_node;
  }
  
  return new_node;
}

//  입력 :
//    t 는 NULL 이 아니다.
//    또한 key 값이 같은 경우도 존재하지 않는다.
//
//  출력 :
//    key 값이 들어갈 곳의 부모노드를 리턴한다.
//    is_left 에 부모노드의 좌측인지 우측인지를 저장한다.
//
static node_t *find_parent_node(const rbtree *t, const key_t key, int *is_left) {
  
  node_t *now_node = t->root;

  while (now_node != NULL) {
    if (key < now_node->key) {
      *is_left = 1;

      if (now_node->left)
        now_node = now_node->left;
      else
        return now_node;
    }
    else {
      *is_left = 0;

      if (now_node->right)
        now_node = now_node->right;
      else
        return now_node;
    }
  }

  return now_node;
}

//
//  입력 :
//    father 랑 grand 가 NULL 일리는 없다.
//      father 는 RED 이며, 자연스럽게 grand 가 존재한다.
//    father 가 root 일리도 없다. (논리상 그렇다)
//
//  출력 :
//    일단 NULL 출력하게 해놨엉...
//
static node_t *insert_1_father_red_no_uncle_or_uncle_black(rbtree *t, node_t *child) {
  node_t *parent = child->parent;
  node_t *grand = find_grand(child);

  if (grand->left == parent && parent->left == child) {
    // printf("Case 1_%d\n", child->key);
    grand->left = parent->right;
    parent->parent = grand->parent;
    grand->parent = parent;
    parent->right = grand;

    parent->color = RBTREE_BLACK;
    grand->color = RBTREE_RED;

    if (t->root == grand)
      t->root = parent;
    else {
      if (grand == parent->parent->left)
        parent->parent->left = parent;
      else
        parent->parent->right = parent;
    }
  }
  else if (grand->left == parent && parent->right == child) {
    // printf("Case 2_%d\n", child->key);
    child->parent = grand->parent;
    grand->parent = child;
    parent->parent = child;
    child->right = grand;
    child->left = parent;
    parent->right = NULL;
    grand->left = NULL;

    child->color = RBTREE_BLACK;
    grand->color = RBTREE_RED;

    if (t->root == grand)
      t->root = child;
    else {
      if (grand == child->parent->left)
        child->parent->left = child;
      else
        child->parent->right = child;
    }
  }
  else if (grand->right == parent && parent->left == child) {
    // printf("Case 3_%d\n", child->key);
    child->parent = grand->parent;
    child->left = grand;
    child->right = parent;
    grand->parent = child;
    parent->parent = child;
    grand->right = NULL;
    parent->left = NULL;

    child->color = RBTREE_BLACK;
    grand->color = RBTREE_RED; 

    if (t->root == grand)
      t->root = child;
    else {
      if (grand == child->parent->left)
        child->parent->left = child;
      else
        child->parent->right = child;
    }
  }
  else {
    // printf("Case 4_%d\n", child->key);
    parent->parent = grand->parent;
    grand->right = parent->left;
    parent->left = grand;
    grand->parent = parent;

    parent->color = RBTREE_BLACK;
    grand->color = RBTREE_RED;

    if (t->root == grand)
      t->root = parent;
    else {
      if (grand == parent->parent->left)
        parent->parent->left = parent;
      else
        parent->parent->right = parent;
    }
  }

  return NULL;
}

static node_t *insert_2_father_red_uncle_red(rbtree *t, node_t *child) {
  node_t *grand = find_grand(child);
  node_t *grand_uncle = NULL;
  node_t *uncle = find_uncle(child);
  node_t *parent = child->parent;

  uncle->color = RBTREE_BLACK;
  parent->color = RBTREE_BLACK;
  grand->color = RBTREE_RED;

  if (t->root == grand) {
    grand->color = RBTREE_BLACK;
  }
  else if (grand->parent && grand->parent->color == RBTREE_RED) {
    grand_uncle = find_uncle(grand);

    if (grand_uncle == NULL || grand_uncle->color == RBTREE_BLACK)
      return insert_1_father_red_no_uncle_or_uncle_black(t, grand);
    return insert_2_father_red_uncle_red(t, grand);
  }
}

