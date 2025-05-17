#include "../include/container.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

DEFINE_QUEUE_FUNCTIONS(int, queue_int)
DEFINE_QUEUE_FUNCTIONS(float, queue_float)

int main() {
    Queue *qi = queue_int_create();
    for (int i = 1; i <= 5; ++i) {
        queue_int_enqueue(qi, i);
    }
    while (!queue_int_is_empty(qi)) {
        printf("Front: %d\n", queue_int_front(qi));
        queue_int_dequeue(qi);
    }
    queue_int_destroy(qi);

    Queue *qf = queue_float_create();
    for (int i = 0; i < 3; ++i) {
        float x = i * 1.5f;
        queue_float_enqueue(qf, x);
    }
    while (!queue_float_is_empty(qf)) {
        printf("Front: %.2f\n", queue_float_front(qf));
        queue_float_dequeue(qf);
    }
    queue_float_destroy(qf);

    return 0;
}
