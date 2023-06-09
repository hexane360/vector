
#include <stdlib.h>
#include <check.h>

#define TYPE char
#include "../vector.h"
#undef TYPE

START_TEST(test_vec_reserve) {
    vector_char vec = vector_new_char();
    vector_reserve_char(&vec, 11);
    ck_assert_uint_eq(vec.cap, 11);
    ck_assert_uint_eq(vec.size, 0);
    ck_assert_ptr_nonnull(vec.data);
    vector_free_char(&vec);
    ck_assert_uint_eq(vec.cap, 0);
    ck_assert_uint_eq(vec.size, 0);
    ck_assert_ptr_null(vec.data);
}
END_TEST

START_TEST(test_vec_lit) {
    vector_char vec;

    vec = vector_lit_char(0);
    ck_assert_uint_eq(vec.cap, 0);
    ck_assert_uint_eq(vec.size, 0);
    vector_free_char(&vec);

    vec = vector_lit_char(1, 'a');
    ck_assert_uint_eq(vec.cap, 1);
    ck_assert_uint_eq(vec.size, 1);
    ck_assert_ptr_nonnull(vec.data);
    ck_assert_mem_eq(vec.data, "a", 1);
    vector_free_char(&vec);

    vec = vector_lit_char(10, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j');
    ck_assert_uint_eq(vec.cap, 10);
    ck_assert_uint_eq(vec.size, 10);
    ck_assert_ptr_nonnull(vec.data);
    ck_assert_mem_eq(vec.data, "abcdefghij", 10);
    vector_free_char(&vec);
}

START_TEST(test_vec_push_pop) {
    vector_char vec;
    vector_init_char(&vec);
    ck_assert_uint_eq(vec.cap, 0);
    ck_assert_uint_eq(vec.size, 0);

    ck_assert_int_eq(0, vector_push_char(&vec, 'a'));
    ck_assert_uint_eq(vec.size, 1);
    ck_assert_uint_eq(vec.cap, 4);

    char elem;
    ck_assert_int_eq(0, vector_pop_char(&vec, &elem));
    ck_assert_uint_eq(vec.size, 0);
    ck_assert_uint_eq(vec.cap, 4);
    ck_assert_int_eq(elem, 'a');

    elem = 'z';
    ck_assert_int_eq(-1, vector_pop_char(&vec, &elem));
    ck_assert_uint_eq(vec.size, 0);
    ck_assert_uint_eq(vec.cap, 4);
    ck_assert_int_eq(elem, 'z');

    ck_assert_int_eq(0, vector_push_char(&vec, 'a'));
    ck_assert_int_eq(0, vector_push_char(&vec, 'b'));
    ck_assert_int_eq(0, vector_push_char(&vec, 'c'));
    ck_assert_int_eq(0, vector_push_char(&vec, 'd'));
    ck_assert_uint_eq(vec.cap, 4);
    ck_assert_uint_eq(vec.size, 4);

    ck_assert_int_eq(0, vector_push_char(&vec, 'e'));
    ck_assert_uint_eq(vec.cap, 8);
    ck_assert_uint_eq(vec.size, 5);

    ck_assert_int_eq(0, vector_push_char(&vec, '\0'));
    ck_assert_str_eq(vec.data, "abcde");

    vector_free_char(&vec);
}

START_TEST(test_vec_insert) {
    vector_char vec = vector_lit_char(4, 'a', 'b', 'c', 'd');

    ck_assert_int_eq(-1, vector_insert_char(&vec, 'h', 6));
    ck_assert_uint_eq(vec.size, 4);

    ck_assert_int_eq(0, vector_insert_char(&vec, '\0', 4));
    ck_assert_int_eq(0, vector_insert_char(&vec, 'e', 0));

    ck_assert_int_eq(0, vector_insert_char(&vec, 'f', 2));

    ck_assert_uint_eq(vec.size, 7);
    ck_assert_str_eq(vec.data, "eafbcd");

    vector_free_char(&vec);
}

START_TEST(test_vec_append) {
    vector_char vec1 = vector_new_copy_char("abcd", 4);
    vector_char vec2 = vector_new_copy_char("efgh", 4);

    ck_assert_int_eq(0, vector_append_copy_char(&vec1, &vec2));
    ck_assert_uint_eq(vec1.size, 8);
    ck_assert_uint_eq(vec1.cap, 8);
    ck_assert_uint_eq(vec2.size, 4);
    ck_assert_mem_eq(vec1.data, "abcdefgh", 8);
    ck_assert_mem_eq(vec2.data, "efgh", 4);

    ck_assert_int_eq(0, vector_append_char(&vec1, &vec2));
    ck_assert_uint_eq(vec1.size, 12);
    ck_assert_uint_eq(vec1.cap, 12);
    ck_assert_uint_eq(vec2.size, 0);
    ck_assert_mem_eq(vec1.data, "abcdefghefgh", 12);
}

START_TEST(test_vec_replace) {
    vector_char vec = vector_lit_char(4, 'a', 'b', 'c', 'd');

    char elem = 'f';
    ck_assert_int_eq(0, vector_replace_char(&vec, &elem, 3));
    ck_assert_mem_eq(vec.data, "abcf", 4);

    ck_assert_int_eq(-1, vector_replace_char(&vec, &elem, 4));
    ck_assert_mem_eq(vec.data, "abcf", 4);

    vector_free_char(&vec);
}

START_TEST(test_vec_swap) {
    vector_char vec = vector_new_copy_char("abcd", 4);

    ck_assert_int_eq(0, vector_swap_char(&vec, 0, 1));
    ck_assert_mem_eq(vec.data, "bacd", 4);

    ck_assert_int_eq(0, vector_swap_char(&vec, 3, 3));
    ck_assert_mem_eq(vec.data, "bacd", 4);

    ck_assert_int_eq(-1, vector_swap_char(&vec, 3, 4));
    ck_assert_mem_eq(vec.data, "bacd", 4);

    ck_assert_int_eq(0, vector_swap_char(&vec, 0, 3));
    ck_assert_mem_eq(vec.data, "dacb", 4);

    vector_free_char(&vec);
}

int cmp_char(char a, char b) {
    if ((a == 'e' && b == 'f') ||
        (a == 'f' && b == 'e')) return 0;
    return (int)a - (int)b;
}

START_TEST(test_vec_sort_inplace) {
    vector_char vec = vector_lit_char(6, 'd', 'f', 'c', 'e', 'b', 'a');

    ck_assert(!vector_is_sorted_char(&vec, cmp_char));
    vector_sort_inplace_char(&vec, cmp_char);
    ck_assert(vector_is_sorted_char(&vec, cmp_char));

    ck_assert_int_eq(0, vector_push_char(&vec, '\0'));
    // tests stability as well
    ck_assert_str_eq(vec.data, "abcdfe");
    vector_free_char(&vec);
}

START_TEST(test_vec_binary_search) {
    vector_char vec = vector_lit_char(6, 'B', 'D', 'b', 'c', 'e', 'y');

    size_t index = 0;
    ck_assert(vector_binary_search_char(&vec, 'f', &index, cmp_char));
    ck_assert_uint_eq(index, 4);

    ck_assert(!vector_binary_search_char(&vec, 'A', &index, cmp_char));
    ck_assert_uint_eq(index, 0);

    ck_assert(!vector_binary_search_char(&vec, 'z', &index, cmp_char));
    ck_assert_uint_eq(index, 6);

    ck_assert(vector_binary_search_char(&vec, 'y', &index, cmp_char));
    ck_assert_uint_eq(index, 5);

    ck_assert(vector_binary_search_char(&vec, 'B', &index, cmp_char));
    ck_assert_uint_eq(index, 0);

    vector_free_char(&vec);
}

Suite *suite() {
    Suite *s;
    TCase *tc;

    s = suite_create("vector");
    tc = tcase_create("Core");

    tcase_add_test(tc, test_vec_lit);
    tcase_add_test(tc, test_vec_reserve);
    tcase_add_test(tc, test_vec_push_pop);
    tcase_add_test(tc, test_vec_insert);
    tcase_add_test(tc, test_vec_append);
    tcase_add_test(tc, test_vec_replace);
    tcase_add_test(tc, test_vec_swap);
    tcase_add_test(tc, test_vec_sort_inplace);
    tcase_add_test(tc, test_vec_binary_search);
    suite_add_tcase(s, tc);
    return s;
}

int main() {
    Suite *s = suite();
    SRunner *sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    int n_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (n_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}