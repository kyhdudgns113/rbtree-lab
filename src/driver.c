#include "rbtree.h"
#include <stdio.h>

int test_array[] = {
    1, 2, 3, 0, -2, -1, -3
};

int main(int argc, char *argv[]) {
    int i = 0;

    rbtree *rbt = new_rbtree();

    for (i = 0; i < sizeof(test_array); i++) {
        rbtree_insert(rbt, test_array[i]);
    }

    lvr(rbt->root);
    printf("\n");
    vlr(rbt->root);


    delete_rbtree(rbt);
}