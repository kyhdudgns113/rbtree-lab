#include "rbtree.h"
#include <stdlib.h>
#include <stdio.h>

int test_array[] = {
    11
    , 5
   , 9
    , 340
    , 670
    , 230
    , 1560
    , 260
    , 2
    , 13
    , 280, 360, 9900, 300
    ,15, 7, 1, 4, 6, 8, 10, 12, 14, 16
};

enum {R, B};


int main(int argc, char *argv[]) {
    int i = 0;
    int ret = 0;
    node_t *erased_node = NULL;
    node_t *test_node = NULL;

    rbtree *rbt = new_rbtree();

    for (i = 0; i < sizeof(test_array)/sizeof(int); i++) {
        rbtree_insert(rbt, test_array[i]);
    }
    for (i = 1; i < 33; i++) {
        test_node = rbtree_find(rbt, i);
        if (test_node)
            continue;
        rbtree_insert(rbt, i);
    }
    test_vlr(rbt->root);
    printf("\n");
    // for (i = 0; i < sizeof(test_array)/sizeof(int); i++) {
    //     test_node = rbtree_find(rbt, test_array[i]);
    //     printf("%d's B B B B is %s\n", test_node->key, check_erase_condition(rbt, test_node, B, B, B, B) ? "T" : " ");
    //     printf("%d's B B B R is %s\n", test_node->key, check_erase_condition(rbt, test_node, B, B, B, R) ? "T" : " ");
    //     printf("%d's B B R B is %s\n", test_node->key, check_erase_condition(rbt, test_node, B, B, R, B) ? "T" : " ");
    //     printf("%d's B B R R is %s\n", test_node->key, check_erase_condition(rbt, test_node, B, B, R, R) ? "T" : " ");
    //     printf("%d's B R B B is %s\n", test_node->key, check_erase_condition(rbt, test_node, B, R, B, B) ? "T" : " ");
    //     printf("%d's B R B R is %s\n", test_node->key, check_erase_condition(rbt, test_node, B, R, B, R) ? "T" : " ");
    //     printf("%d's B R R B is %s\n", test_node->key, check_erase_condition(rbt, test_node, B, R, R, B) ? "T" : " ");
    //     printf("%d's B R R R is %s\n", test_node->key, check_erase_condition(rbt, test_node, B, R, R, R) ? "T" : " ");
    //     printf("%d's R B B B is %s\n", test_node->key, check_erase_condition(rbt, test_node, R, B, B, B) ? "T" : " ");
    //     printf("%d's R B B R is %s\n", test_node->key, check_erase_condition(rbt, test_node, R, B, B, R) ? "T" : " ");
    //     printf("%d's R B R B is %s\n", test_node->key, check_erase_condition(rbt, test_node, R, B, R, B) ? "T" : " ");
    //     printf("%d's R B R R is %s\n", test_node->key, check_erase_condition(rbt, test_node, R, B, R, R) ? "T" : " ");
    //     printf("%d's R R B B is %s\n", test_node->key, check_erase_condition(rbt, test_node, R, R, B, B) ? "T" : " ");
    //     printf("%d's R R B R is %s\n", test_node->key, check_erase_condition(rbt, test_node, R, R, B, R) ? "T" : " ");
    //     printf("%d's R R R B is %s\n", test_node->key, check_erase_condition(rbt, test_node, R, R, R, B) ? "T" : " ");
    //     printf("%d's R R R R is %s\n", test_node->key, check_erase_condition(rbt, test_node, R, R, R, R) ? "T" : " ");
    //     printf("\n");
    // }

    // printf("\n");

    for (i = 0; i < sizeof(test_array)/sizeof(int); i++) {
        printf("%d is finding...\n", test_array[i]);
        test_vlr(rbt->root);
        // test_lvr(rbt->root);
        erased_node = rbtree_find(rbt, test_array[i]);
        if (erased_node == NULL) {
            printf("NULL POINTER ERROR\n");
            break;
        }
        rbtree_erase(rbt, erased_node);
        printf("\n");
    }

    delete_rbtree(rbt);
}