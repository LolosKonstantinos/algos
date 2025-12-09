//
// Created by Constantin on 15/10/2025.
//

#ifndef BINARY_TREES_H
#define BINARY_TREES_H

typedef int(*cmp_f)(void *, void *);

typedef struct tree_priv_t tree_priv_t;
typedef struct tree_t tree_t;
typedef struct tree_node_t tree_node_t;

typedef int(*tree_insert)(tree_t, void *);
typedef int(*tree_remove)(tree_t, void *);
typedef int(*tree_search)(tree_t, void *);

struct tree_t{
    tree_insert  insert;
    tree_remove remove;
    tree_search search;
    tree_priv_t *priv;
};

typedef struct tree_attr {
    unsigned char type;
}tree_attr_t;

int new_tree(tree_t t, tree_attr_t attr);
void free_tree(tree_t t);
tree_node_t *new_node();
/*________________________________________AVL TREE FUNCTIONS________________________________________*/
int avl_insert(tree_t* t, void* data);
int avl_insert_copy(tree_t t, void* data);

int avl_delete(tree_t t, void* data);

int avl_search(tree_t t, void* data);

/*AVL HELPERS*/

void rotate_left(tree_t t, tree_node_t);
void rotate_right(tree_t t, tree_node_t);
void rotate_left_right(tree_t t, tree_node_t);
void rotate_right_left(tree_t t, tree_node_t);
void rotate_left_left(tree_t t, tree_node_t);
void rotate_right_right(tree_t t, tree_node_t);
#endif //BINARY_TREES_H
