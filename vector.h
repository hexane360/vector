
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

#ifndef VECTOR_H
    #define CONCAT_(a, b) a ## b
    #define CONCAT(a, b) CONCAT_(a, b)
    #define GENERIC(a) CONCAT(a, TYPE)
    #define MAX(X, Y) (((Y) > (X)) ? (Y) : (X))
    #define MIN(X, Y) (((Y) < (X)) ? (Y) : (X))

    #define VECTOR_H
#endif
#define VEC_TYPE GENERIC(vector_)

typedef struct {
    size_t size;
    size_t cap;
    TYPE *data;
} VEC_TYPE;

void GENERIC(vector_init_)(VEC_TYPE *vec) {
    *vec = (VEC_TYPE) { 0, 0, NULL };
}

int GENERIC(vector_reserve_)(VEC_TYPE *vec, size_t cap) {
    if (vec->cap >= cap) return 0;

    TYPE *ptr = realloc(vec->data, cap * sizeof(TYPE));
    if (!ptr) return -1;
    vec->cap = cap;
    vec->data = ptr;
    return 0;
}

int GENERIC(vector_init_cap_)(VEC_TYPE *vec, size_t cap) {
    *vec = (VEC_TYPE) { 0, 0, NULL };
    return GENERIC(vector_reserve_)(vec, cap);
}

VEC_TYPE GENERIC(vector_new_)() {
    VEC_TYPE vec;
    GENERIC(vector_init_)(&vec);
    return vec;
}

VEC_TYPE GENERIC(vector_new_cap_)(size_t cap) {
    VEC_TYPE vec;
    GENERIC(vector_init_cap_)(&vec, cap);
    return vec;
}

VEC_TYPE GENERIC(vector_lit_)(size_t n, ...) {
    va_list args;

    VEC_TYPE vec = GENERIC(vector_new_cap_)(n);
    if (vec.cap < n) return vec; // failed to allocate

    va_start(args, n);
    for (size_t i = 0; i < n; i++) {
        if (sizeof(TYPE) < sizeof(int)) {
            vec.data[i] = (TYPE)va_arg(args, int);
        } else {
            vec.data[i] = va_arg(args, TYPE);
        }
    }
    vec.size = n;
    va_end(args);
    return vec;
}

void GENERIC(vector_free_)(VEC_TYPE *vec) {
    if (vec->cap) {
        free(vec->data);
        vec->data = NULL;
    }
    vec->cap = vec->size = 0;
}

int GENERIC(vector_push_)(VEC_TYPE *vec, TYPE val) {
    if (vec->cap <= vec->size) {
        size_t new_cap = MAX(vec->size << 1, 4);
        if (GENERIC(vector_reserve_)(vec, new_cap)) return -1;
    }
    vec->data[vec->size] = val;
    vec->size++;
    return 0;
}

int GENERIC(vector_pop_)(VEC_TYPE *vec, TYPE *ret) {
    if (!vec->size) {
        ret = NULL;
        return -1;
    }
    *ret = vec->data[--vec->size];
    return 0;
}

int GENERIC(vector_insert_)(VEC_TYPE *vec, TYPE val, size_t index) {
    // invalid index
    if (index > vec->size) return -1;
    // just a simple push
    if (index == vec->size) return GENERIC(vector_push_)(vec, val);
    // grow if necessary
    if (vec->cap <= vec->size) {
        size_t new_cap = MAX(vec->size << 1, 4);
        if (GENERIC(vector_reserve_)(vec, new_cap)) return -1;
    }
    // shift elements forwards
    for (size_t i = vec->size; i > index; i--) {
        vec->data[i] = vec->data[i-1];
    }
    vec->data[index] = val;
    vec->size++;
    return 0;
}

int GENERIC(vector_append_)(VEC_TYPE *dest, VEC_TYPE *src) {
    if (GENERIC(vector_reserve_)(dest, dest->size + src->size)) return -1;

    for (size_t i = 0; i < src->size; i++) {
        dest->data[dest->size + i] = src->data[i];
    }
    dest->size += src->size;
    src->size = 0;
    return 0;
}

int GENERIC(vector_remove_)(VEC_TYPE *vec, TYPE *ret, size_t index) {
    // invalid index
    if (index >= vec->size) return -1;
    *ret = vec->data[index];
    // shift elements backwards
    for (size_t i = index; i + 1 < vec->size; i++) {
        vec->data[i] = vec->data[i+1];
    }
    vec->size--;
    return 0;
}

int GENERIC(vector_replace_)(VEC_TYPE *vec, TYPE *elem, size_t index) {
    if (index >= vec->size) return -1;
    TYPE temp = vec->data[index];
    vec->data[index] = *elem;
    *elem = temp;
    return 0;
}

void GENERIC(vector_swap_unchecked_)(VEC_TYPE *vec, size_t index1, size_t index2) {
    TYPE temp = vec->data[index1];
    vec->data[index1] = vec->data[index2];
    vec->data[index2] = temp;
}

int GENERIC(vector_swap_)(VEC_TYPE *vec, size_t index1, size_t index2) {
    if (index1 >= vec->size || index2 >= vec->size) return -1;
    if (index1 == index2) return 0;

    GENERIC(vector_swap_unchecked_)(vec, index1, index2);
    return 0;
}

void GENERIC(vector_printf_)(VEC_TYPE *vec, const char *elem_fmt) {
    printf("[");
    for (size_t i = 0; i < vec->size; i++) {
        printf(elem_fmt, vec->data[i]);
        if (i + 1 < vec->size) {
            printf(", ");
        }
    }
    printf("]\n");
}

void GENERIC(vector_sort_inplace_)(VEC_TYPE *vec, int (*cmp_func)(TYPE, TYPE)) {
    // insertion sort
    for (size_t i = 1; i < vec->size; i++) {
        for (size_t j = i; j > 0 && cmp_func(vec->data[j-1], vec->data[j]) > 0; j--) {
            GENERIC(vector_swap_unchecked_)(vec, j-1, j);
        }
    }
}

bool GENERIC(vector_binary_search_)(VEC_TYPE *vec, TYPE val, size_t *index, int (*cmp_func)(TYPE, TYPE)) {
    size_t search_start = 0;
    size_t search_size = vec->size;
    size_t pivot = 0;

    if (!search_size) {
        *index = 0;
        return false;
    }
    //printf("size: %zu\n", search_size);

    // this is a pretty horrible implementation
    while (search_size > 0) {
        pivot = search_size / 2;
        //printf("start: %zu size: %zu pivot: %zu\n", search_start, search_size, pivot);

        int result = cmp_func(val, vec->data[search_start + pivot]);
        if (result == 0) { // equal
            *index = search_start + pivot;
            return true;
        }
        if (result > 0) { // pivot < val
            search_start += pivot + 1;
            search_size -= pivot + 1;
        } else {
            search_size = pivot;
        }
    }
    //printf("end start: %zu size: %zu pivot: %zu\n", search_start, search_size, pivot);
    *index = search_start;
    return false;
}