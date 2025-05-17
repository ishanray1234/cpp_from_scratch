#include "../include/container.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Instantiate type-specific vectors
DEFINE_VECTOR_FUNCTIONS(int, vector_int)
DEFINE_VECTOR_FUNCTIONS(float, vector_float)
DEFINE_VECTOR_FUNCTIONS(char*, vector_str)

int main() {
    // === TESTING vector_int ===
    printf("Testing vector_int:\n");
    Vector *vi = vector_int_create();
    for (int i = 0; i < 5; i++) {
        int *val = (int*)malloc(sizeof(int));
        *val = i * 10;
        vector_int_push_back(vi, *val);
    }

    for (size_t i = 0; i < vec_size((Vector *)vi); i++) {
        printf("vi[%zu] = %d\n", i, vector_int_get(vi, i));
    }

    vec_remove((Vector *)vi, 2); // remove the 3rd element
    printf("After removal:\n");
    for (size_t i = 0; i < vec_size((Vector *)vi); i++) {
        printf("vi[%zu] = %d\n", i, vector_int_get(vi, i));
    }

    vector_int_destroy(vi);


    // === TESTING vector_float ===
    printf("\nTesting vector_float:\n");
    Vector *vf = vector_float_create();
    for (int i = 0; i < 3; i++) {
        float *fval = (float*)malloc(sizeof(float));
        *fval = i * 1.1f;
        vector_float_push_back(vf, *fval);
    }

    for (size_t i = 0; i < vec_size((Vector *)vf); i++) {
        printf("vf[%zu] = %.2f\n", i, vector_float_get(vf, i));
    }

    vector_float_destroy(vf);


    // === TESTING vector_str ===
    printf("\nTesting vector_str:\n");
    Vector *vs = vector_str_create();

    const char *words[] = {"hello", "world", "C", "vectors"};
    for (int i = 0; i < 4; i++) {
        char *str = strdup(words[i]); // allocate copy of string
        vector_str_push_back(vs, str);
    }

    for (size_t i = 0; i < vec_size((Vector *)vs); i++) {
        printf("vs[%zu] = %s\n", i, vector_str_get(vs, i));
    }

    vec_remove((Vector *)vs, 1); // remove "world"
    printf("After removal:\n");
    for (size_t i = 0; i < vec_size((Vector *)vs); i++) {
        printf("vs[%zu] = %s\n", i, vector_str_get(vs, i));
    }

    vector_str_destroy(vs);

    return 0;
}
