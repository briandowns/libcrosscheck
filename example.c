#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "crosscheck.h"

#define GEN_RAND_NUM rand() % (1024 + 1 - 0) + 0

static int undertest_x = 0;
static int undertest_y = 0;
static int expected = 0;

void
cc_setup()
{
    undertest_x = GEN_RAND_NUM;
    undertest_y = GEN_RAND_NUM;
    expected = undertest_x + undertest_y;
}

void
cc_tear_down()
{
    undertest_x = 0;
    undertest_y = 0;
    expected = 0;
}

int64_t
add(int64_t x, int64_t y)
{
    return x + y;
}

int64_t
sub(int64_t x, int64_t y)
{
    return x - y;
}

cc_result_t
test_add_fail(void)
{
    int actual = add(2, 5);
    CC_ASSERT_INT_NOT_EQUAL(actual, 5);
    CC_SUCCESS;
}

cc_result_t
test_add_success(void)
{
    int res = add(undertest_x, undertest_y); 
    CC_ASSERT_INT_EQUAL(res, expected);
    CC_SUCCESS;
}

cc_result_t
test_sub_success(void)
{
    int res = sub(undertest_x, undertest_y);
    CC_ASSERT_INT_EQUAL(res, (undertest_x-undertest_y)); 
    CC_SUCCESS;
}

cc_result_t
test_string_compare_success(void)
{
    CC_ASSERT_STRING_EQUAL("x", "x");
    CC_SUCCESS;
}

cc_result_t
test_string_compare_failure(void)
{
    CC_ASSERT_STRING_NOT_EQUAL("x", "y");
    CC_SUCCESS;
}

cc_result_t
test_assert_true(void)
{
    CC_ASSERT_TRUE(true); 
    CC_SUCCESS;
}

cc_result_t
test_assert_null(void)
{
    CC_ASSERT_NULL(NULL); 
    CC_SUCCESS;
}

cc_result_t
test_assert_not_null(void)
{
    CC_ASSERT_NOT_NULL((void*)1); 
    CC_SUCCESS;
}

int
main(void)
{
    srand(time(NULL));

    CC_INIT;
 
    CC_RUN(test_add_fail);
    CC_RUN(test_add_success);
    CC_RUN(test_sub_success);
    CC_RUN(test_string_compare_success);
    CC_RUN(test_string_compare_failure);
    CC_RUN(test_assert_true);
    CC_RUN(test_assert_null);
    CC_RUN(test_assert_not_null);

    CC_COMPLETE;
}
