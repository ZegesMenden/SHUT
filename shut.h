#pragma once

#ifndef SHUT_PRINTF
#define SHUT_PRINTF(...) printf(__VA_ARGS__)
#endif

typedef struct {

    const char* name;

    int n_tests;
    int n_test_failures;
    int exit_code;

} __SHUT_SECTION_DATA_T;

__SHUT_SECTION_DATA_T* __section_cur;
__SHUT_SECTION_DATA_T __shut_tests = {
    .name = "",
    .n_tests = 0,
    .n_test_failures = 0,
    .exit_code = 0
};

#define SHUT_TEST(secname) \
    __SHUT_SECTION_DATA_T __SHUT_test_##secname = { \
        .name = #secname, \
        .n_tests = 0, \
        .n_test_failures = 0, \
        .exit_code = 0 \
    }; \
    void __SHUT_TEST_##secname(void)

#define SHUT_RUN_TEST(secname) \
    __section_cur = &(__SHUT_test_##secname); \
    __section_cur->exit_code = 1; \
    __SHUT_TEST_##secname(); \
    __SHUT_TEST_DIAGNOSTICS(); \
    __section_cur = NULL;

#define SHUT_CASE(testparam) \
    if ( __section_cur == NULL ) { \
        SHUT_PRINTF("SHUT ERROR: tests must run within a testing section.\n"); \
    } \
    else { \
        __section_cur->n_tests++; \
        if ( ! (testparam) ) { \
            if ( __section_cur->n_test_failures == 0 ) { \
                SHUT_PRINTF("SHUT - Test %s failed:\n", __section_cur->name); \
            } \
            SHUT_PRINTF("\tAt %s:%i %s\n", __FILE__, __LINE__, #testparam); \
            __section_cur->n_test_failures++; \
            __section_cur->exit_code = -1; \
        } \
    }    

void __SHUT_TEST_DIAGNOSTICS() {
    if (__section_cur == NULL) {
        SHUT_PRINTF("SHUT ERROR: test diagnostic cannot find a test section.\n");
        return;
    }

    SHUT_PRINTF("SHUT - Test %s complete:\n", __section_cur->name);
    SHUT_PRINTF("\tTests passed: %i\n", __section_cur->n_tests - __section_cur->n_test_failures);
    SHUT_PRINTF("\tTests failed: %i\n", __section_cur->n_test_failures);
    SHUT_PRINTF("\tTest pass rate: %.1f%%\n", 100.f * ( 1.f - ((float)__section_cur->n_test_failures / (float)(__section_cur->n_tests + (__section_cur->n_tests == 0 && __section_cur->n_test_failures == 0))) ));

    __shut_tests.n_tests++;
    if ( __section_cur->n_test_failures ) {
        __shut_tests.n_test_failures++;
    }
}

void SHUT_TEST_INFO() {

    SHUT_PRINTF("SHUT - Global test information:\n");
    SHUT_PRINTF("\tTests passed: %i\n", __shut_tests.n_tests - __shut_tests.n_test_failures);
    SHUT_PRINTF("\tTests failed: %i\n", __shut_tests.n_test_failures);
    SHUT_PRINTF("\tTest pass rate: %.1f%%\n", 100.f * ( 1.f - ((float)__shut_tests.n_test_failures / (float)(__shut_tests.n_tests + (__shut_tests.n_tests == 0 && __shut_tests.n_test_failures == 0))) ));

}
