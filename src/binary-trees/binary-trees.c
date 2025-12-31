#include <stdint.h>
#include "binary-trees.h"

#include <stdlib.h>
#include <string.h>
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

int new_tree(tree_t t, tree_attr_t attr){return 0;}
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
    tree_node_t *node, *temp, **stack = NULL, **top;
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
        *top = node;
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
    avl_balance(stack, top, priv);

    node->data = data;
    return 0;
}

int avl_insert_copy(tree_t *t, void* data) {
    tree_priv_t *priv;
    tree_node_t *node, *temp, **stack = NULL, **top;
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
        *top = node;
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
    avl_balance(stack, top, priv);

    node->data = malloc(t->priv->node_size);
    if (!node->data) {
        free(node);
        return 1;
    }
    memcpy(node->data, data, priv->node_size);
    return 0;
}

int avl_delete(tree_t *t, void* data) {
    //todo need to study to implement
    tree_node_t *node, *prev = NULL, *temp, *temp_prev;
    char direction = -1, temp_direction = -1;
    int cmp_res;
    cmp_f cmp;

    if (!t || !data) return -1;
    node = t->priv->root;
    cmp = t->priv->cmp;

    while (node != NULL) {
        prev = node;
        cmp_res = cmp(data, node->data);
        if (cmp_res == 0) break;

        if (cmp_res < 0) {
            node = node->left;
            direction = TREE_LEFT;
        }
        else {
            node = node->right;
            direction = TREE_RIGHT;
        }
    }
    if (!node || !prev) return 1; //prev is always not null, just clang-tidy cries again

    if (!(node->left || node->right)) {
        //if it has no children
        free(node->data);
        free(node);
        if (direction == TREE_LEFT) prev->left = NULL;
        else if (direction == TREE_RIGHT) prev->right = NULL;
        else t->priv->root = NULL;
    }
    else if (node->left && node->right) {
        //if it has both children

        //we favor the tallest subtree, so that the resulting tree is more balanced
        temp_prev = node;
        if (node->left->height > node->right->height) {
            temp = node->left;
            temp_direction = TREE_LEFT;
            while (temp->right) {
                temp_prev = temp;
                temp = temp->right;
            }
        }
        else {
            temp = node->right;
            temp_direction = TREE_RIGHT;
            while (temp->left) {
                temp_prev = temp;
                temp = temp->left;
            }
        }

        if (temp != node->left || temp != node->right) {
            if (temp_direction == TREE_LEFT) {
                if (temp->left) temp_prev->right = temp->left;
                else temp_prev->right = NULL;
            }
            else {
                if (temp->right) temp_prev->left = temp->right;
                else temp_prev->left = NULL;
            }
            temp->left = NULL;
            temp->right = NULL;
        }

        if (direction == TREE_LEFT) prev->left = temp;
        else if (direction == TREE_RIGHT) prev->right = temp;
        else t->priv->root = temp;

        if (!temp->right) temp->right = node->right;
        if (!temp->left) temp->left = node->left;

    }

    else {
        //if it has only one child
        if (node->left) temp = node->left;
        else temp = node->right;

        free(node->data);
        free(node);
        if (direction == TREE_LEFT) prev->left = temp;
        else if (direction == TREE_RIGHT) prev->right = temp;
        else t->priv->root = temp;
    }
    return 0;
}

int avl_search(tree_t *t, void* data) {
    tree_node_t *node;
    int cmp_res;
    cmp_f cmp;

    if (!t || !data) return -1;
    node = t->priv->root;
    cmp = t->priv->cmp;

    while (node != NULL) {
        cmp_res = cmp(data, node->data);
        if (cmp_res == 0) return 1;
        if (cmp_res < 0) node = node->left;
        else node = node->right;
    }
    return 0;
}


/*AVL HELPERS*/
inline void rotate_left(tree_node_t *node, tree_node_t *root, tree_priv_t* tree) {
    tree_node_t *b;

    if (!node) return;
    if (!node->right) return;

    b = node->right;

    //node->right is the new root
    if (tree->root != node){
        if (root == NULL) return;
        if (node->direction == TREE_RIGHT) root->right = b;
        else root->left = b;
    }
    else {
        tree->root = b;
    }

    node->right = b->left;
    b->left = node;

    //update node's height and then b's height
    node->height = 1 + (node->right->height > node->left->height ? node->right->height : node->left->height);
    b->height = 1 + (b->right->height > b->left->height ? b->right->height : b->left->height);
}
inline void rotate_right(tree_node_t *node, tree_node_t* root, tree_priv_t* tree) {
    tree_node_t *b;

    if (!node) return;
    if (!node->right) return;

    b = node->left;

    //b is the new root
    if (tree->root != node){
        if (root == NULL) return;
        if (node->direction == TREE_RIGHT) root->right = b;
        else root->left = b;
    }
    else {
        tree->root = b;
    }

    node->left = b->right;
    b->right = node;

    //update the node's height and then b's height
    node->height = 1 + (node->right->height > node->left->height ? node->right->height : node->left->height);
    b->height = 1 + (b->right->height > b->left->height ? b->right->height : b->left->height);
}

inline void rotate_left_right(tree_node_t *node, tree_node_t* root, tree_priv_t* tree) {
    tree_node_t *a,*b;

    if (!node) return;

    //__rotate right the right child__//
    a = node->right;
    b = a->left;
    //b is the new root of the subtree
    node->right = b;
    //change children
    a->left = b->right;
    b->right = a;
    //update heights: first a's then b's
    a->height = 1 + (a->right->height > a->left->height ? a->right->height : a->left->height);
    b->height = 1 + (b->right->height > b->left->height ? b->right->height : b->left->height);

    //__rotate left__//
    b = node->left;

    //node->left is the new root
    if (tree->root != node){
        if (root == NULL) return;
        if (node->direction == TREE_RIGHT) root->right = b;
        else root->left = b;
    }
    else {
        tree->root = b;
    }

    node->right = b->left;
    b->left = node;

    //update the heights, first node's and then b's
    node->height = 1 + (node->right->height > node->left->height ? node->right->height : node->left->height);
    b->height = 1 + (b->right->height > b->left->height ? b->right->height : b->left->height);
}
inline void rotate_right_left(tree_node_t *node, tree_node_t* root, tree_priv_t* tree) {
    tree_node_t *a,*b;
    if (!node) return;

    //__rotate left the left child__//
    a = node->left;
    b = a->right;

    //b is the new root of the subtree
    node->left = b;

    a->right = b->left;
    b->left = a;
    //update heights: first a's then b's
    a->height = 1 + (a->right->height > a->left->height ? a->right->height : a->left->height);
    b->height = 1 + (b->right->height > b->left->height ? b->right->height : b->left->height);

    //__rotate right__//
    b = node->left;

    //b is the new root
    if (tree->root != node) {
        if (root == NULL) return;
        if (node->direction == TREE_RIGHT) root->right = b;
        else root->left = b;
    }
    else {
        tree->root = b;
    }

    node->left = b->right;
    b->right = node;

    //update the heights, first node's and then b's
    node->height = 1 + (node->right->height > node->left->height ? node->right->height : node->left->height);
    b->height = 1 + (b->right->height > b->left->height ? b->right->height : b->left->height);
}

void avl_balance(tree_node_t** stack, tree_node_t** top, tree_priv_t* tree) {
    tree_node_t *heavy_child, **node;

    if (!stack || !top || !tree) return;

    node = top;

    while (node >= stack) {
        //update-heights and calculate balance factors
        if ((*node)->right == NULL && (*node)->left == NULL) {
            (*node)->height = 0;
            (*node)->bf = 0;
        }
        else if ((*node)->right == NULL && (*node)->left != NULL) {
            (*node)->height = (*node)->left->height + 1;
            (*node)->bf = (char)(-((*node)->left->height));
        }
        else if ((*node)->right != NULL && (*node)->left == NULL) {
            (*node)->height = (*node)->right->height + 1;
            (*node)->bf = (char)((*node)->right->height);
        }
        else {
            (*node)->height = 1 + ((*node)->right->height > (*node)->left->height ? (*node)->right->height : (*node)->left->height);
            (*node)->bf = (char)((*node)->right->height - (*node)->left->height);
        }

        if (abs((*node)->bf) > 1) //the first node we find unbalanced
            break;

        node = --node; //go down the stack
    }

    if (node < stack) return; //if the node is not in the stack then the tree is already balanced

    heavy_child = (*node)->right->height > (*node)->left->height ? (*node)->right : (*node)->left;

    if (heavy_child->direction == TREE_RIGHT) {

        if ((*node)->bf >= 0) {
            //right-right
            rotate_left(*node, *(node -1), tree);
        }
        else {
            //right-left
            rotate_right_left(*node, *(node -1), tree);
        }
    }
    else {
        if ((*node)->bf <= 0) {
            //left-left
            rotate_right(*node, *(node -1), tree);
        }
        else {
            //left-right
            rotate_left_right(*node, *(node -1), tree);
        }
    }
    //node has its height updated already in the rotation functions
    //so we need to update from its root and up
    node = --node;
    while (node >= stack){
        if ((*node)->right == NULL && (*node)->left == NULL) {
            (*node)->height = 0;
        }
        else if ((*node)->right == NULL && (*node)->left != NULL) {
            (*node)->height = (*node)->left->height + 1;
        }
        else if ((*node)->right != NULL && (*node)->left == NULL) {
            (*node)->height = (*node)->right->height + 1;
        }
        else {
            (*node)->height = 1 + ((*node)->right->height > (*node)->left->height ? (*node)->right->height : (*node)->left->height);
        }
        node == --node;
    }
}





/*________________________________________RED BLACK TREE FUNCTIONS________________________________________*/