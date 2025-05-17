#include <stdio.h>
#include "../include/container.h"


int main() {
    Stack *s = stack_new();

    stack_push(s, 1);
    stack_push(s, 2);
    stack_push(s, 3);

    while (!stack_is_empty(s)) {
        printf("Popped: %d\n", stack_pop(s));
    }

    stack_free(s);
    return 0;
}
