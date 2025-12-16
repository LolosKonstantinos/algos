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
    uint32_t height;
    cmp_f cmp;          // the function based on which we do struct comparison
};

struct tree_node_t {
    void *data;
    tree_node_t *left;
    tree_node_t *right;
    unsigned char height;
    char zero[7];
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
    tree_node_t *node, *temp, *stack = NULL, *top;
    int cmp_res;

    if (!t) return -1;
    if (!(t->priv)) return -1;
    priv = t->priv;

    node = new_node();
    if (!node) {
        return 1;
    }

    stack = malloc(sizeof(tree_node_t *) * priv->height);
    if (!stack) {
        free(node);
        return 1;
    }
    top = stack;

    if (!priv->root) {
        priv->root = node;
        return 0;
    }

    temp = priv->root;

    while (1) {
        cmp_res = priv->cmp(data, temp->data);
        if (cmp_res == 0) {
            //if the node already exists we do not re-insert it
            free(node);
            return 0;
        }

        //add the current node to the stack, so that after the insertion we have the stack trace of the new node
        top = node;
        ++top;

        if (cmp_res > 0) {
            if (temp->right == NULL) {
                temp->right = node;
                break;
            }
            temp = temp->right;
        }
        else {
            if (temp->left == NULL) {
                temp->left = node;
                break;
            }
            temp = temp->left;
        }
    }
    //check the stack trace and check if we need to rotate
    while (top != stack) {

    }
    node->data = data; //copy the data at the end so that we are sure there is no duplicate
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