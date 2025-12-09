/*
// Created by Constantin on 15/10/2025.
*/

#ifndef STACK_H
#define STACK_H
#include <stddef.h>


#define ALGOS_STACK_FLAG_STATIC             0x01
#define ALGOS_STACK_FLAG_THREAD_SAFE        0x02
#define ALGOS_STACK_FLAG_OVERALLOCATED      0x04
#define ALGOS_STACK_FLAG_BOUNDED            0x08

typedef struct static_stack_priv static_stack_priv;

typedef struct stack_attr {
    unsigned char flags;
    float growth_rate;
    size_t cell_size;
    size_t cell_count;
    size_t max_size;
}stack_attr_t;

typedef struct stack {
    /*insert functions here*/
    void *priv; /* the private part of the stack*/
}stack_t, STACK_T;



/*create and free stacks*/
stack_t *new_stack(stack_attr_t *attr);
void free_stack(stack_t **stack);


/*core functions*/

/*static stack thread unsafe*/
void static_stack_push_thread_unsafe(const stack_t *stack, const void *data);
void *static_stack_pop_thread_unsafe(const stack_t * stack);
void *static_stack_peek_thread_unsafe(const stack_t *stack);
int static_stack_is_empty_thread_unsafe(const stack_t *stack);
size_t static_stack_size_thread_unsafe(const stack_t *stack);

/*static stack thread safe*/
void static_stack_push_thread_safe(stack_t *stack, void *data);
void *static_stack_pop_thread_safe(stack_t *stack);
void *static_stack_peek_thread_safe(stack_t *stack);
int static_stack_is_empty_thread_safe(stack_t *stack);
size_t static_stack_size_thread_safe(stack_t *stack);


/*dynamic stack thread unsafe*/
void dynamic_stack_push_thread_unsafe(stack_t *stack, void *data);
void *dynamic_stack_pop_thread_unsafe(stack_t *stack);
void *dynamic_stack_peek_thread_unsafe(stack_t *stack);
int dynamic_stack_is_empty_thread_unsafe(stack_t *stack);
size_t dynamic_stack_size_thread_unsafe(stack_t *stack);

/*dynamic stack thread safe*/
void dynamic_stack_push_thread_safe(stack_t *stack, void *data);
void *dynamic_stack_pop_thread_safe(stack_t *stack);
void *dynamic_stack_peek_thread_safe(stack_t *stack);
int dynamic_stack_is_empty_thread_safe(stack_t *stack);
size_t dynamic_stack_size_thread_safe(stack_t *stack);
#endif //STACK_H
