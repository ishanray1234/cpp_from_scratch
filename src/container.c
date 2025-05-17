#include "../include/container.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*****************************************VECTOR*****************************************/

// Vector implementation
// This is a simple dynamic array implementation in C.
/*  
    vec_create: Creates a new vector with an initial capacity.
    vec_push_back: Adds an item to the end of the vector, resizing if necessary.
    vec_get: Retrieves an item from the vector at a specified index.
    vec_destroy: Frees the memory allocated for the vector.
*/
// DEFINE_VECTOR_FUNCTIONS(int, vector_int)
// DEFINE_VECTOR_FUNCTIONS(float, vector_float)
// DEFINE_VECTOR_FUNCTIONS(char*, vector_str)

size_t vec_size(Vector *vec) {
    return vec->size;
}

void vec_remove(Vector *vec, size_t index) {
    if (index >= vec->size) return;
    free(vec->data[index]);
    memmove(&vec->data[index], &vec->data[index + 1], sizeof(void*) * (vec->size - index - 1));
    vec->size--;
}

/*****************************************QUEUE*****************************************/

// Queue implementation
// This is a simple circular queue implementation in C.
/*  
    queue_create: Creates a new queue with an initial capacity.
    queue_enqueue: Adds an item to the end of the queue, resizing if necessary.
    queue_dequeue: Removes an item from the front of the queue.
    queue_peek: Retrieves the front item without removing it.
    queue_destroy: Frees the memory allocated for the queue.
*/
Queue *queue_create(size_t elem_size) {
    Queue *q = (Queue *)malloc(sizeof(Queue));
    q->capacity = 4;
    q->front = q->rear = q->size = 0;
    q->elem_size = elem_size;
    q->data = malloc(elem_size * q->capacity);
    return q;
}

void queue_destroy(Queue *q) {
    free(q->data);
    free(q);
}

int queue_is_empty(Queue *q) {
    return q->size == 0;
}

int queue_is_full(Queue *q) {
    return q->size == q->capacity;
}

void queue_resize(Queue *q) {
    size_t new_cap = q->capacity * 2;
    void *new_data = malloc(new_cap * q->elem_size);
    for (size_t i = 0; i < q->size; i++) {
        void *src = (char *)q->data + ((q->front + i) % q->capacity) * q->elem_size;
        void *dst = (char *)new_data + i * q->elem_size;
        memcpy(dst, src, q->elem_size);
    }
    free(q->data);
    q->data = new_data;
    q->front = 0;
    q->rear = q->size;
    q->capacity = new_cap;
}

void queue_enqueue(Queue *q, void *elem) {
    if (queue_is_full(q)) {
        queue_resize(q);
    }
    memcpy((char *)q->data + q->rear * q->elem_size, elem, q->elem_size);
    q->rear = (q->rear + 1) % q->capacity;
    q->size++;
}

void *queue_front(Queue *q) {
    if (queue_is_empty(q)) return NULL;
    return (char *)q->data + q->front * q->elem_size;
}

void queue_dequeue(Queue *q) {
    if (!queue_is_empty(q)) {
        q->front = (q->front + 1) % q->capacity;
        q->size--;
    }
}


// === Stack Implementation ===

Stack *stack_new() {
    Stack *stack = malloc(sizeof(Stack));
    if (!stack) return NULL;

    stack->data = malloc(4 * sizeof(int));
    if (!stack->data) {
        free(stack);
        return NULL;
    }

    stack->capacity = 4;
    stack->top = 0;
    return stack;
}

void stack_push(Stack *stack, int value) {
    if (stack->top == stack->capacity) {
        // Need to resize
        stack->capacity *= 2;
        int *new_data = realloc(stack->data, stack->capacity * sizeof(int));
        if (!new_data) {
            perror("Failed to resize stack");
            exit(EXIT_FAILURE);
        }
        stack->data = new_data;
    }

    stack->data[stack->top++] = value;
}

int stack_pop(Stack *stack) {
    if (stack->top == 0) {
        fprintf(stderr, "Stack underflow\n");
        exit(EXIT_FAILURE);
    }
    return stack->data[--stack->top];
}

int stack_peek(const Stack *stack) {
    if (stack->top == 0) {
        fprintf(stderr, "Stack is empty\n");
        exit(EXIT_FAILURE);
    }
    return stack->data[stack->top - 1];
}

int stack_is_empty(const Stack *stack) {
    return stack->top == 0;
}

void stack_free(Stack *stack) {
    if (stack) {
        free(stack->data);
        free(stack);
    }
}

