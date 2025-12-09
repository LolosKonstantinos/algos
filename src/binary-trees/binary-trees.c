#include <stdint.h>
#include "binary-trees.h"

#include <stdlib.h>
//
// Created by Constantin on 15/10/2025.
//


struct tree_priv_t {
    void *root;
    size_t node_size;
    size_t node_count;
    cmp_f cmp;          // the function based on which we do struct comparison
};

struct tree_node_t {
    void *data;
    unsigned char height;
    tree_node_t *left;
    tree_node_t *right;
};

int new_tree(tree_t t, tree_attr_t attr){}
void free_tree(tree_t t){}

tree_node_t *new_node() {
    tree_node_t *node = (tree_node_t *)calloc(1,sizeof(tree_node_t));
    if(node == NULL) {
        return NULL;
    }
    return node;
}

/*________________________________________AVL TREE FUNCTIONS________________________________________*/
int avl_insert(tree_t* t, void* data) {
    tree_priv_t *priv;
    tree_node_t *node;

    if (!t) return -1;

    node = new_node();
    if (!node) {
        return 1;
    }
    node->data = data;
    priv = t->priv;

    while (1) {
        //todo here lies the logit to insertg
        break;
    }
    return 0;
}

int avl_insert_copy(tree_t t, void* data){}

int avl_delete(tree_t t, void* data){}

int avl_search(tree_t t, void* data){}


/*AVL HELPERS*/
int  avl_is_balanced(tree_t t){}
void rotate_left(tree_t t, tree_node_t * node) {}
void rotate_right(tree_t t, tree_node_t *node) {}
void rotate_left_left(tree_t t, tree_node_t *node) {}
void rotate_right_right(tree_t t, tree_node_t *node) {}
void rotate_left_right(tree_t t, tree_node_t *node) {}
void rotate_right_left(tree_t t, tree_node_t *node) {}



/*________________________________________RED BLACK TREE FUNCTIONS________________________________________*/