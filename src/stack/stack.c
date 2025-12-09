/*
// Created by Constantin on 15/10/2025.
*/

#include "stack.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

/*function pointers*/
typedef void (* push)(stack_t, void *);
typedef void *(* pop)(stack_t);
typedef void *(* peek)(stack_t);
typedef void *(* is_empty)(stack_t);
typedef size_t (* size)(stack_t);

struct static_stack {
    void *data;
    void *top;
    size_t size;
    size_t cell_size;
    unsigned char flags;
    char zero[7];
};

struct dynamic_stack {
    void *data;
    void *top;
    size_t size;
    size_t cell_size;
    size_t max_size;
    float growth_factor;
};

struct static_stack_tsafe {
    void *data;
    void *top;
    size_t size;
    size_t cell_size;
    pthread_spinlock_t lock;
    pthread_cond_t cond;
};

struct dynamic_stack_tsafe {
    void *data;
    void *top;
    size_t size;
    size_t cell_size;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};



stack_t *new_stack(stack_attr_t *attr) {
    stack_t *stack = NULL;

    if (attr == NULL) return NULL;

    stack = (stack_t *)malloc(sizeof(stack_t));
    if (!stack) return NULL;

    if (attr->flags & ALGOS_STACK_FLAG_STATIC) {
        if (attr->flags & ALGOS_STACK_FLAG_OVERALLOCATED) {
            free(stack);
            return NULL;
        }
        if (attr->flags & ALGOS_STACK_FLAG_THREAD_SAFE) {
            free(NULL); /*remove before flight*/
        }
        else {

        }
    }

    return stack;
}

void free_stack(stack_t **stack) {

}

/*////////////////////////////////////////////
//                                          //
//               STATIC STACK               //
//                                          //
////////////////////////////////////////////*/

void static_stack_push_thread_unsafe(const stack_t * const stack, const void * const restrict data) {
    struct static_stack *priv;
    if (stack == NULL || data == NULL) return;

    priv = stack->priv;

    priv->top += priv->cell_size;
    memcpy(priv->top, data, priv->cell_size);
}
void *static_stack_pop_thread_unsafe(const stack_t * const stack) {
    void *ret_node;
    struct static_stack *priv;
    if (stack == NULL) return NULL;
    priv = stack->priv;
    ret_node = priv->top;
    if (priv->top != NULL) priv->top -= priv->cell_size;
    return ret_node;
}
void *static_stack_peek_thread_unsafe(const stack_t * const stack) {
    void *ret_node;
    struct static_stack *priv;
    if (stack == NULL) return NULL;
    priv = stack->priv;
    ret_node = priv->top;
    return ret_node;
}
int static_stack_is_empty_thread_unsafe(const stack_t *stack) {
    struct static_stack *priv;

    if (stack == NULL) return 0;
    priv = stack->priv;
    return priv->top == NULL;
}
size_t static_stack_size_thread_unsafe(const stack_t *stack) {
    return 0;
}
