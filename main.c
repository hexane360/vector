#include <stdio.h>

#define TYPE char
#include "vector.h"
#undef TYPE
#define TYPE size_t
#include "vector.h"
#undef TYPE

int cmp_char(char a, char b) {
    // positive if a > b, negative if b > a, zero if a == b
    return (int)a - (int)b;
}

int main(void) {
    vector_char v1 = vector_new_char();
    vector_size_t v2 = vector_new_size_t();

    vector_push_char(&v1, 'c');
    vector_push_char(&v1, 'h');
    vector_push_char(&v1, 'a');
    vector_push_char(&v1, 'r');
    vector_push_char(&v1, ' ');
    vector_push_char(&v1, 's');
    vector_push_char(&v1, 't');
    vector_push_char(&v1, 'r');

    vector_printf_char(&v1, "'%c'");
    v1.data[7] = '\0';
    printf("char: %s\n", v1.data);
    printf("size: %zu  cap: %zu\n", v1.size, v1.cap);

    vector_push_size_t(&v2, 1);
    vector_push_size_t(&v2, 2);
    vector_push_size_t(&v2, 3);
    vector_push_size_t(&v2, 4);

    vector_insert_size_t(&v2, 5, 1);

    size_t out;
    vector_pop_size_t(&v2, &out);

    vector_printf_size_t(&v2, "'%zu'");
    printf("size: %zu  cap: %zu\n", v2.size, v2.cap);

    vector_char v3 = vector_lit_char(11, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', '\0');
    printf("char: %s\n", v3.data);
    printf("size: %zu  cap: %zu\n", v3.size, v3.cap);

    char out_c;
    vector_remove_char(&v3, &out_c, 5);
    printf("removed '%c' from '%s'\n", out_c, v3.data);
    printf("size: %zu  cap: %zu\n", v3.size, v3.cap);

    vector_replace_char(&v3, &out_c, 0);
    printf("swapped '%c' from '%s'\n", out_c, v3.data);
    printf("size: %zu  cap: %zu\n", v3.size, v3.cap);

    vector_swap_char(&v3, 0, 1);
    printf("swapped 0 and 1 in '%s'\n", v3.data);

    vector_append_char(&v3, &v1);
    vector_printf_char(&v3, "'%c'");
    printf("dst size: %zu  cap: %zu\n", v3.size, v3.cap);
    printf("src size: %zu  cap: %zu\n", v1.size, v1.cap);

    vector_sort_inplace_char(&v3, cmp_char);
    vector_printf_char(&v3, "'%c'");

    vector_free_char(&v1);
    vector_free_size_t(&v2);
    vector_free_char(&v3);

    return 0;
}