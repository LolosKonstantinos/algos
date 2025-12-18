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
    unsigned char direction;
    unsigned char height;
    char bf;
    char zero[5];
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
                node->direction = TREE_RIGHT;
                break;
            }
            temp = temp->right;
        }
        else {
            if (temp->left == NULL) {
                temp->left = node;
                node->direction = TREE_LEFT;
                break;
            }
            temp = temp->left;
        }
    }
    //check the stack trace and check if we need to rotate
    while (top != stack) {
//todo there are functions for this
    }
    node->data = data; //copy the data at the end so that we are sure there is no duplicate
    return 0;
}

int avl_insert_copy(tree_t t, void* data){return 0;}

int avl_delete(tree_t t, void* data){return 0;}

int avl_search(tree_t t, void* data){return 0;}


/*AVL HELPERS*/
tree_node_t  *avl_is_balanced(tree_node_t *stack, tree_node_t *top) {
    tree_node_t *curr;

    if (!stack | !top) return NULL;

    curr = top;

    do {
        //update-heights and calculate balance factors
        curr = --curr; //go down the stack

        if (curr->right == NULL && curr->left == NULL) {
            curr->height = 0;
            curr->bf = 0;
        }
        else if (curr->right == NULL && curr->left != NULL) {
            curr->height = curr->left->height + 1;
            curr->bf = -curr->left->height;
        }
        else if (curr->right != NULL && curr->left == NULL) {
            curr->height = curr->right->height + 1;
            curr->bf = curr->right->height;
        }
        else {
            curr->height = 1 + (curr->right->height > curr->left->height ? curr->right->height : curr->left->height);
            curr->bf = curr->right->height - curr->left->height;
        }

        if (abs(curr->bf) > 1) return curr; //return the first node we find is unbalanced

    }while (curr != stack);
}

void avl_balance(tree_node_t *node, tree_node_t *stack, tree_node_t *top) {
    tree_node_t *heavy_child;
    if (!node || !stack || !top) return;
    if (abs(node->bf) < 2) return ; //node is balanced

    heavy_child = node->right->height > node->left->height ? node->right : node->left;

    if (heavy_child->direction == TREE_RIGHT) {
        if (node->bf >= 0) {
            //right-right
            rotate_left(node, 0);
        }
        else {
            //right-left
            rotate_right_left(node, 0);
        }
    }
    else {
        if (node->bf <= 0) {
            //left-left
            rotate_right(node, 0);
        }
        else {
            //left-right
            rotate_left_right(node, 0);
        }
    }

    do {
        //todo: recalculate heights for the rest of the tree
    }while (top != stack);
}

void rotate_left(tree_node_t *node, tree_node_t *root) {
    tree_node_t *b;

    if (!node || !root) return;
    if (!node->right) return;

    b = node->right;

    //node->left is the new root
    if (node->direction == TREE_RIGHT) root->right = b;
    else root->left = b;

    node->right = b->left;
    b->left = node;
}
void rotate_right(tree_node_t *node, tree_node_t* root) {
    tree_node_t *b;

    if (!node || !root) return;
    if (!node->right) return;

    b = node->left;

    //b is the new root
    if (node->direction == TREE_RIGHT) root->right = b;
    else root->left = b;

    node->left = b->right;
    b->right = node;

}
void rotate_left_right(tree_node_t *node, tree_node_t* root) {
    tree_node_t *a,*b;

    if (!node || !root) return;

    //rotate right the right child
    a = node->right;
    b = a->left;
    //b is the new root of the subtree
    node->right = b;
    //change children
    a->left = b->right;
    b->right = a;

    //rotate left
    b = node->left;

    //node->left is the new root
    if (node->direction == TREE_RIGHT) root->right = b;
    else root->left = b;

    node->right = b->left;
    b->left = node;
}
void rotate_right_left(tree_node_t *node, tree_node_t* root) {
    tree_node_t *a,*b;
    if (!node || !root) return;

    //rotate left the left child
    a = node->left;
    b = a->right;

    //b is the new root of the subtree
    node->left = b;

    a->right = b->left;
    b->left = a;

    //rotate right
    b = node->left;

    //b is the new root
    if (node->direction == TREE_RIGHT) root->right = b;
    else root->left = b;

    node->left = b->right;
    b->right = node;
}



/*________________________________________RED BLACK TREE FUNCTIONS________________________________________*/