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
  delete_node_lrv(t->root);
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
  // parent_node = rbtree_find(t, key);
  // if (parent_node)
  //   return parent_node;

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
    else {
      insert_2_father_red_uncle_red(t, new_node);
    }
  }
  return new_node;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  
  node_t *now_node = t->root;

  while (now_node != NULL) {
    if (now_node->key == key)
      break;
    else if (key < now_node->key) {
      now_node = now_node->left;
    }
    else {
      now_node = now_node->right;
    }
  }

  return now_node;
}

node_t *rbtree_min(const rbtree *t) {
  node_t *now_node = NULL;

  if (t == NULL || t->root == NULL)
    return NULL;

  now_node = t->root;
  while (now_node->left)
    now_node = now_node->left;

  return now_node;
}

node_t *rbtree_max(const rbtree *t) {
  node_t *now_node = NULL;

  if(t == NULL || t->root == NULL)
    return NULL;

  now_node = t->root;
  while (now_node->right)
    now_node = now_node->right;

  return now_node;
}

int rbtree_erase(rbtree *t, node_t *p) {
  int debug_counter = 0;
  node_t *deleted_node = p;
  node_t *deleted_sibling = NULL;

  // printf("\nBefore %d is removed\n", p->key);
  // test_vlr(t->root);
  // printf("%d is removing...\n", p->key);

  if (t == NULL || t->root == NULL || p == NULL)
    return 0;

  if (p->left == NULL && p->right == NULL) {
    erase_0_has_no_children(t, deleted_node);
    return 0;
  }

  if (p->left) {
    deleted_node = p->left;
    while (deleted_node->right)
      deleted_node = deleted_node->right;

    // printf("Removing %d, Removed %d\n", p->key, deleted_node->key);
    p->key = deleted_node->key;
    deleted_sibling = find_sibling(deleted_node);
    
    
    // Case 0 : 본인이나 자손이 빨간색인 경우 -> 포인터만 바꿔주면 됨
    if (deleted_node->color == RBTREE_RED || (deleted_node->left && deleted_node->left->color == RBTREE_RED)) {
      // printf("Case 0\n");
      erase_left_0_m_is_red_or_son_is_red(deleted_node);
      return 0;
    }
      
    // Case 1-1 : 부모가 빨강, 형제와 그 자손들이 검정
    if (deleted_sibling == NULL ||
        deleted_node->parent->color == RBTREE_RED && 
        deleted_sibling && deleted_sibling->color == RBTREE_BLACK && 
        (deleted_sibling->left == NULL || deleted_sibling->left->color == RBTREE_BLACK) &&
        (deleted_sibling->right == NULL || deleted_sibling->right->color == RBTREE_BLACK)) {
      // printf("Case 1-1\n");
      erase_left_1_1_m_is_black_and_sibling_and_its_son_blacks(deleted_node);
      free(deleted_node);
      return 0;
    }
      
    // Case x_2 : 형제가 검정, 형제의 왼쪽이 빨강
    if (deleted_sibling && 
        deleted_sibling->left && deleted_sibling->left->color == RBTREE_RED) {
      if (deleted_node->parent->right == deleted_node) {
        erase_left_x_2_s_is_black_and_its_left_is_red(t, deleted_node);
      }
      else {
        erase_right_x_2_s_is_black_and_its_right_is_red(t, deleted_node);
      }
      
      free(deleted_node);
      return 0;
    }

    //  Case x_3 : 형제가 검정, 형제의 오른쪽이 빨강
    if (deleted_sibling &&
        deleted_sibling->right && deleted_sibling->right->color == RBTREE_RED) {
      erase_left_x_3_s_is_black_and_its_lr_is_br(t, deleted_node);
      free(deleted_node);
      return 0;
    }

    //  Case 2_1 : 부모가 검정, 형제가 검정
    if (deleted_node->parent->color == RBTREE_BLACK &&
        (deleted_sibling == NULL || deleted_sibling->color == RBTREE_BLACK)) {
      // printf("Case 2-1\n");
      erase_left_2_1_p_is_black_s_is_black(t, deleted_node);
      free(deleted_node);
      return 0;
    }

    //  Case 2_4 : 부모가 검정, 형제가 빨강
    if (deleted_node->parent->color == RBTREE_BLACK &&
        (deleted_sibling && deleted_sibling->color == RBTREE_RED)) {
      // printf("Case 2-4\n");
      erase_left_2_4_p_is_black_s_is_red(t, deleted_node);
      free(deleted_node);
      return 0;
    }
  }
  else {
    deleted_node = p->right;
    while (deleted_node->left)
      deleted_node = deleted_node->left;

    p->key = deleted_node->key;
    deleted_sibling = find_sibling(deleted_node);
    
    // Case 0 : 본인이나 자손이 빨간색인 경우 -> 포인터만 바꿔주면 됨
    if (deleted_node->color == RBTREE_RED || (deleted_node->right && deleted_node->right->color == RBTREE_RED)) {
      erase_right_0_m_is_red_or_son_is_red(deleted_node);
      return 0;
    }
      
    // Case 1-1 : 부모가 빨강, 형제와 그 자손들이 검정
    if (deleted_node->parent->color == RBTREE_RED && 
        deleted_sibling && deleted_sibling->color == RBTREE_BLACK && 
        (deleted_sibling->left == NULL || deleted_sibling->left->color == RBTREE_BLACK) &&
        (deleted_sibling->right == NULL || deleted_sibling->right->color == RBTREE_BLACK)) {
      erase_right_1_1_m_is_black_and_sibling_and_its_son_blacks(deleted_node);
      free(deleted_node);
      return 0;
    }
    
    // Case x_2 : 형제가 검정, 형제의 오른쪽이 빨강
    if (deleted_sibling && 
        deleted_sibling->right && deleted_sibling->right->color == RBTREE_RED) {
      if (deleted_node->parent->left == deleted_node) {
        erase_right_x_2_s_is_black_and_its_right_is_red(t, deleted_node);
      }
      else {
        erase_left_x_2_s_is_black_and_its_left_is_red(t, deleted_node);
      }
      
      free(deleted_node);
      return 0;
    }

    //  Case x_3 : 형제가 검정, 형제의 왼쪽이 빨강
    if (deleted_sibling &&
        deleted_sibling->left && deleted_sibling->left->color == RBTREE_RED) {
      erase_right_x_3_s_is_black_and_its_lr_is_rb(t, deleted_node);
      free(deleted_node);
      return 0;
    }

    //  Case 2_1 : 부모가 검정, 형제가 검정
    if (deleted_node->parent->color == RBTREE_BLACK &&
        (deleted_sibling == NULL || deleted_sibling->color == RBTREE_BLACK)) {
      erase_right_2_1_p_is_black_s_is_black(t, deleted_node);
      free(deleted_node);
      return 0;
    }
    
    //  Case 2_4 : 부모가 검정, 형제가 빨강
    if (deleted_node->parent->color == RBTREE_BLACK &&
        (deleted_sibling && deleted_sibling->color == RBTREE_RED)) {
      erase_right_2_4_p_is_black_s_is_red(t, deleted_node);
      free(deleted_node);
      return 0;
    }
  }
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  int now_size = 0;

  if (t == NULL || t->root == NULL) {
    return 0;
  }

  to_array_lvr(arr, t->root, n, &now_size);

  return now_size;
}


void test_lvr(node_t *node) {
  if (node->left)
    test_lvr(node->left);
  printf("LVR %d is %s\n", node->key, node->color == RBTREE_BLACK ? "BLACK" : "RED");
  if (node->right)
    test_lvr(node->right);
}

void test_vlr(node_t *node) {
  printf("VLR %d(%d) is %s\n", node->key, node->parent ? node->parent->key : 777, node->color == RBTREE_BLACK ? "BLACK" : "RED");
  if (node->left)
    test_vlr(node->left);
  if (node->right)
    test_vlr(node->right);
}

void test_lrv(node_t *node) {
  if (node->left)
    test_lrv(node->left);
  if (node->right)
    test_lrv(node->right);
  printf("LRV %d is %s\n", node->key, node->color == RBTREE_BLACK ? "BLACK" : "RED");
}


static void to_array_lvr(key_t *output_array, node_t *now_node, const size_t max_size, int *now_size) {

  if (max_size <= *now_size)
    return;

  if (now_node->left && (max_size > *now_size)) {
    to_array_lvr(output_array, now_node->left, max_size, now_size);
  }

  if (max_size <= *now_size)
    return;
  output_array[*now_size] = now_node->key;
  *now_size += 1;

  if(now_node->right && (max_size > *now_size)) {
    to_array_lvr(output_array, now_node->right, max_size, now_size);
  }

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

static node_t *find_sibling(node_t *now_node) {
  if (now_node == NULL || now_node->parent == NULL)
    return NULL;

  if (now_node == now_node->parent->left)
    return now_node->parent->right;
  return now_node->parent->left;
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
    if (*is_left)
      parent->left = new_node;
    else
      parent->right = new_node;
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
    parent->right = child->left;
    if (child->left)
      child->left->parent = parent;
    grand->left = child->right;
    if (child->right)
      child->right->parent = grand;
    child->right = grand;
    child->left = parent;

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
    grand->right = child->left;
    if (child->left)
      child->left->parent = grand;
    parent->left = child->right;
    if (child->right)
      child->right->parent = parent;
    child->left = grand;
    child->right = parent;
    grand->parent = child;
    parent->parent = child;

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
    else if (grand_uncle && grand_uncle->color == RBTREE_RED)
      return insert_2_father_red_uncle_red(t, grand);
  }

  return child;
}

static int erase_0_has_no_children(rbtree *t, node_t *now_node) {
  if (t->root == now_node)
    t->root = NULL;
  else {
    if (now_node->parent->left == now_node)
      now_node->parent->left = NULL;
    else
      now_node->parent->right = NULL;
  }
  free(now_node);
  return 0;
}

static int erase_right_0_m_is_red_or_son_is_red(node_t *now_node) {
  if (now_node->parent->left == now_node)
    now_node->parent->left = now_node->right;
  else
    now_node->parent->right = now_node->right;

  if (now_node->right) {
    now_node->right->color = RBTREE_BLACK;
    now_node->right->parent = now_node->parent;
  }
    
  free(now_node);
  return 0;
}

static int erase_left_0_m_is_red_or_son_is_red(node_t *now_node) {
  if (now_node->parent->right == now_node)
    now_node->parent->right = now_node->left;
  else
    now_node->parent->left = now_node->left;

  if (now_node->left) {
    now_node->left->color = RBTREE_BLACK;
    now_node->left->parent = now_node->parent;
  }
  free(now_node);
  return 0;
}

static int erase_right_1_1_m_is_black_and_sibling_and_its_son_blacks(node_t *now_node) {
  node_t *sibling = find_sibling(now_node);

  now_node->parent->color = RBTREE_BLACK;

  if(sibling)
    sibling->color = RBTREE_RED;
  chain_right(now_node);
  return 0;
}

static int erase_left_1_1_m_is_black_and_sibling_and_its_son_blacks(node_t *now_node) {
  node_t *sibling = find_sibling(now_node);

  now_node->parent->color = RBTREE_BLACK;
  
  if(sibling)
    sibling->color = RBTREE_RED;
  
  chain_left(now_node);
  return 0;
}

static int erase_right_x_2_s_is_black_and_its_right_is_red(rbtree *t, node_t *now_node) {
  node_t *parent = now_node->parent;
  node_t *sibling = find_sibling(now_node);
  node_t *sibling_right = sibling ? sibling->right : NULL;

  //  Rotate Left
  parent->right = sibling->left;
  sibling->parent = parent->parent;
  parent->parent = sibling;
  sibling->left = parent;

  //  Change sibling's right son's color black
  sibling->color = parent->color;
  parent->color = RBTREE_BLACK;
  if (sibling_right)
    sibling_right->color = RBTREE_RED;

  //  Root Processing
  if (t->root == parent)
    t->root = sibling;
  if (sibling->parent) {
    if (sibling->parent->left == parent)
      sibling->parent->left = sibling;
    else
      sibling->parent->right = sibling;
  }

  chain_right(now_node);
  return 0;
}

static int erase_left_x_2_s_is_black_and_its_left_is_red(rbtree *t, node_t *now_node) {
  node_t *parent = now_node->parent;
  node_t *sibling = find_sibling(now_node);
  node_t *sibling_left = sibling ? sibling->left : NULL;

  //  Rotate Right
  parent->left = sibling->right;
  sibling->parent = parent->parent;
  parent->parent = sibling;
  sibling->right = parent;

  //  Change sibling's left son's color black
  sibling->color = parent->color;
  parent->color = RBTREE_BLACK;
  if (sibling_left)
    sibling_left->color = RBTREE_RED;

  //  Root Processing
  if (t->root == parent)
    t->root = sibling;
  if (sibling->parent) {
    if (sibling->parent->left == parent)
      sibling->parent->left = sibling;
    else
      sibling->parent->right = sibling;
  }
  
  
  chain_left(now_node);
  return 0;
}

static int erase_right_x_3_s_is_black_and_its_lr_is_rb(rbtree *t, node_t *now_node) {
  int is_chain_left = 0;

  node_t *sibling = find_sibling(now_node);
  node_t *sl = sibling->left;
  node_t *slr = sibling->left->right;

  if (now_node->parent->left == now_node)
    is_chain_left = 1;

  now_node->parent->right = sl;
  sl->parent = now_node->parent;
  sl->color = RBTREE_BLACK;
  sl->right = sibling;
  sibling->parent = sl;
  sibling->color = RBTREE_RED;
  sibling->left = slr;
  if (slr)
    slr->parent = sibling;
    
  if (is_chain_left)
    chain_left(now_node);
  else
    chain_right(now_node);
  erase_right_x_2_s_is_black_and_its_right_is_red(t, now_node);
}

static int erase_left_x_3_s_is_black_and_its_lr_is_br(rbtree *t, node_t *now_node) {
  int is_chain_left = 0;

  node_t *sibling = find_sibling(now_node);
  node_t *sr = sibling->right;
  node_t *srl = sibling->right->left;

  if (now_node->parent->left == now_node)
    is_chain_left = 1;

  now_node->parent->left = sr;
  sr->parent = now_node->parent;
  sr->color = RBTREE_BLACK;
  sr->left = sibling;
  sibling->parent = sr;
  sibling->color = RBTREE_RED;
  sibling->right = srl;
  if (srl)
    srl->parent = sibling;
  if (now_node->parent->left == now_node)

  if (is_chain_left)
    chain_left(now_node);
  else
    chain_right(now_node);
  erase_left_x_2_s_is_black_and_its_left_is_red(t, now_node);
}

static int erase_right_2_1_p_is_black_s_is_black(rbtree *t, node_t *now_node) {
  node_t *sibling = find_sibling(now_node);
  node_t *p_sibling = NULL;
  node_t *parent = NULL;

  if (sibling == NULL)
    return 0;

  sibling->color = RBTREE_RED;
  parent = now_node->parent;

  if (parent && parent->parent) {
    p_sibling = find_sibling(parent);
    
  }
  chain_right(now_node);
}

static int erase_left_2_1_p_is_black_s_is_black(rbtree *t, node_t *now_node) {
  node_t *sibling = find_sibling(now_node);
  node_t *p_sibling = NULL;
  node_t *parent = NULL;

  if (sibling == NULL)
    return 0;

  sibling->color = RBTREE_RED;
  parent = now_node->parent;

  if (parent && parent->parent) {
    p_sibling = find_sibling(parent);
    
  }
  chain_left(now_node);
}

static int erase_right_2_4_p_is_black_s_is_red(rbtree *t, node_t *now_node) {
  node_t *parent = now_node->parent;
  node_t *sibling = find_sibling(now_node);

  parent->right = sibling->left;
  if (parent->right)
    parent->right->parent = parent;
  sibling->left = parent;
  sibling->parent = parent->parent;
  parent->parent = sibling;

  parent->color = RBTREE_RED;
  sibling->color = RBTREE_BLACK;

  if (t->root == parent) {
    t->root = sibling;
  }
  else {
    if (sibling->parent->left == parent)
      sibling->parent->left = sibling;
    else
      sibling->parent->right = sibling;
  }
  chain_right(now_node);
  sibling = find_sibling(now_node);
  if (sibling == NULL || 
        (sibling->color == RBTREE_BLACK && 
        (sibling->left == NULL || sibling->left->color == RBTREE_BLACK) &&
        (sibling->right == NULL || sibling->right->color == RBTREE_BLACK))) {
    erase_right_1_1_m_is_black_and_sibling_and_its_son_blacks(now_node);
    return 0;
  }
  if (sibling->right && sibling->right->color == RBTREE_RED) {
    erase_right_x_2_s_is_black_and_its_right_is_red(t, now_node);
    return 0;
  }
  if (sibling->left && sibling->left->color == RBTREE_RED) {
    erase_right_x_3_s_is_black_and_its_lr_is_rb(t, now_node);
    return 0;
  }
  return 0;
}

static int erase_left_2_4_p_is_black_s_is_red(rbtree *t, node_t *now_node) {
  node_t *parent = now_node->parent;
  node_t *sibling = find_sibling(now_node);

  parent->left = sibling->right;
  if (parent->left)
    parent->left->parent = parent;
  sibling->right = parent;
  sibling->parent = parent->parent;
  parent->parent = sibling;

  parent->color = RBTREE_RED;
  sibling->color = RBTREE_BLACK;

  if (t->root == parent) {
    t->root = sibling;
  }
  else {
    if (sibling->parent->left == parent)
      sibling->parent->left = sibling;
    else
      sibling->parent->right = sibling;
  }

  chain_left(now_node);
  sibling = find_sibling(now_node);
  if (sibling == NULL || 
        (sibling->color == RBTREE_BLACK && 
        (sibling->left == NULL || sibling->left->color == RBTREE_BLACK) &&
        (sibling->right == NULL || sibling->right->color == RBTREE_BLACK))) {
    erase_left_1_1_m_is_black_and_sibling_and_its_son_blacks(now_node);
    return 0;
  }
  if (sibling->right && sibling->right->color == RBTREE_RED) {
    erase_left_x_2_s_is_black_and_its_left_is_red(t, now_node);
    return 0;
  }
  if (sibling->left && sibling->left->color == RBTREE_RED) {
    erase_left_x_3_s_is_black_and_its_lr_is_br(t, now_node);
    return 0;
  }
  return 0;
}


//
//  p_col, s_col, sl_col, sr_col
//
int check_erase_condition(rbtree *t, node_t *now_node, color_t p_col, color_t s_col, color_t sl_col, color_t sr_col) {
  node_t *p = NULL;
  node_t *s = NULL;
  node_t *sl = NULL;
  node_t *sr = NULL;
  
  // printf("Debug 1\n");
  if (t == NULL || t->root == NULL)
    return 0;

  // printf("Debug 2\n");
  if (now_node == NULL)
    return 0;

  p = now_node->parent;
  s = find_sibling(now_node);

  // printf("Debug 3\n");
  if (p == NULL && (p_col == RBTREE_RED || s_col == RBTREE_RED || sl_col == RBTREE_RED || sr_col == RBTREE_RED))
    return 0;
  else if (p == NULL)
    return 1;

  // printf("Debug 4\n");
  if (s == NULL && (s_col == RBTREE_RED || sl_col == RBTREE_RED || sr_col == RBTREE_RED))
    return 0;
  else if (p->color == p_col && s == NULL && s_col == RBTREE_BLACK)
    return 1;
  else if (p->color != p_col)
    return 0;

  //  이제 p 나 s 는 NULL 이 아니다.
  sl = s->left;
  sr = s->right;
  // printf("%d, %d, %d, %d\n", p->key, s->key, sl->key, sr->key);
  // printf("Debug 5\n");
  if (p->color == p_col && 
      s->color == s_col &&
      ((sl == NULL && sl_col == RBTREE_BLACK) || (sl && sl->color == sl_col)) &&
      ((sr == NULL && sr_col == RBTREE_BLACK) || (sr && sr->color == sr_col)) )
    return 1;

  // printf("Debug 6\n");
  return 0;
  
}

static int chain_right(node_t *now_node) {
  if (now_node->parent->left == now_node)
    now_node->parent->left = now_node->right;
  else
    now_node->parent->right = now_node->right;
  if (now_node->right)
    now_node->right->parent = now_node->parent;

  return 0;
}

static int chain_left(node_t *now_node) {
  if (now_node->parent->left == now_node)
    now_node->parent->left = now_node->left;
  else
    now_node->parent->right = now_node->left;
  if (now_node->left)
    now_node->left->parent = now_node->parent;
}



static void delete_node_lrv(node_t *now_node) {
  if (now_node == NULL)
    return;

  if (now_node->left)
    delete_node_lrv(now_node->left);
  if (now_node->right)
    delete_node_lrv(now_node->right);
  free(now_node);
}


