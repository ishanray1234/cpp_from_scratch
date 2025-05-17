#ifndef CONTAINER_H
#define container_H

#pragma once
#include <stdlib.h>
#include <assert.h>


/************************************VECTOR*****************************************/

// Vector implementation
// This is a simple dynamic array implementation in C.
/*  
    vec_create: Creates a new vector with an initial capacity.
    vec_push_back: Adds an item to the end of the vector, resizing if necessary.
    vec_get: Retrieves an item from the vector at a specified index.
    vec_destroy: Frees the memory allocated for the vector.
*/
typedef struct {
    void **data;
    size_t size;
    size_t capacity;
} Vector;

// Common utility functions
size_t vec_size(Vector *vec);
void vec_remove(Vector *vec, size_t index);

// Macro to declare vector struct and functions for any type
#define DEFINE_VECTOR_FUNCTIONS(TYPE, NAME)                                       \
    Vector* NAME##_create() {                                                     \
        Vector *vec = malloc(sizeof(Vector));                                     \
        vec->capacity = 4;                                                        \
        vec->size = 0;                                                            \
        vec->data = malloc(sizeof(void*) * vec->capacity);                        \
        return vec;                                                               \
    }                                                                             \
                                                                                  \
    void NAME##_push_back(Vector *vec, TYPE value) {                              \
        TYPE *copy = malloc(sizeof(TYPE));                                        \
        *copy = value;                                                            \
        if (vec->size >= vec->capacity) {                                         \
            vec->capacity *= 2;                                                   \
            vec->data = realloc(vec->data, sizeof(void*) * vec->capacity);       \
        }                                                                         \
        vec->data[vec->size++] = copy;                                            \
    }                                                                             \
                                                                                  \
    TYPE NAME##_get(Vector *vec, size_t index) {                                  \
        if (index >= vec->size) {                                                 \
            TYPE dummy = {0}; return dummy;                                       \
        }                                                                         \
        return *(TYPE*)vec->data[index];                                          \
    }                                                                             \
                                                                                  \
    void NAME##_destroy(Vector *vec) {                                            \
        for (size_t i = 0; i < vec->size; ++i) free(vec->data[i]);               \
        free(vec->data);                                                          \
        free(vec);                                                                \
    }

/****************************************QUEUE*****************************************/

// Queue implementation
// This is a simple circular queue implementation in C.
/*  
    queue_create: Creates a new queue with an initial capacity.
    queue_enqueue: Adds an item to the end of the queue, resizing if necessary.
    queue_dequeue: Removes an item from the front of the queue.
    queue_peek: Retrieves the front item without removing it.
    queue_destroy: Frees the memory allocated for the queue.
*/
typedef struct {
    void *data;
    size_t front, rear, size, capacity;
    size_t elem_size;
} Queue;

Queue *queue_create(size_t elem_size);
void queue_destroy(Queue *q);
void queue_resize(Queue *q);
int queue_is_empty(Queue *q);
int queue_is_full(Queue *q);
void queue_enqueue(Queue *q, void *elem);
void *queue_front(Queue *q);
void queue_dequeue(Queue *q);

#define DEFINE_QUEUE_FUNCTIONS(TYPE, NAME)                                        \
    Queue *NAME##_create() { return queue_create(sizeof(TYPE)); }                \
    void NAME##_destroy(Queue *q) { queue_destroy(q); }                          \
    void NAME##_enqueue(Queue *q, TYPE value) { queue_enqueue(q, &value); }      \
    TYPE NAME##_front(Queue *q) { return *(TYPE *)queue_front(q); }              \
    void NAME##_dequeue(Queue *q) { queue_dequeue(q); }                          \
    int NAME##_is_empty(Queue *q) { return queue_is_empty(q); }                  \
    int NAME##_is_full(Queue *q) { return queue_is_full(q); }

/*****************************************STACK*****************************************/

// Stack implementation
// This is a simple stack implementation in C.
/*  
    stack_new: Creates a new stack with an initial capacity.
    stack_push: Adds an item to the top of the stack, resizing if necessary.
    stack_pop: Removes the top item from the stack.
    stack_peek: Retrieves the top item without removing it.
    stack_is_empty: Checks if the stack is empty.
    stack_free: Frees the memory allocated for the stack.
*/
typedef struct {
    int *data;
    size_t capacity;
    size_t top; // Points to next empty index
} Stack;

// === Stack Functions ===
Stack *stack_new();                  // Create a new stack
void stack_push(Stack *stack, int value); // Push an element onto the stack
int stack_pop(Stack *stack);             // Pop the top element
int stack_peek(const Stack *stack);      // Peek at the top element
int stack_is_empty(const Stack *stack);  // Check if stack is empty
void stack_free(Stack *stack);          // Free stack memory

#endif

