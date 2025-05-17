#include <stdio.h>
#include "../include/data_structures.h"


int main() {
    Set *s = set_new();

    set_add(s, 50);
    set_add(s, 30);
    set_add(s, 70);
    set_add(s, 20);
    set_add(s, 40);
    set_add(s, 60);
    set_add(s, 80);

    printf("Contains 60: %d\n", set_contains(s, 60));
    printf("Contains 25: %d\n", set_contains(s, 25));

    set_remove(s, 70);
    printf("Contains 70 after removal: %d\n", set_contains(s, 70));

    set_free(s);
    return 0;
}
