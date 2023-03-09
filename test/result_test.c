#define K_LOG_IMPLEMENTATION
#include "../k_log.h"

#define K_RESULT_USE_K_LOG
#define K_RESULT_IMPLEMENTATION
#include "../k_result.h"

k_Result test_assert_do() {
    k_ASSERT_DO(1 > 2, printf("print\n"); return K_ERROR("test_assert_do"));
    return K_OK();
}

k_Result test_assert_do_ret() {
    k_ASSERT_DO_RET(1 > 2, printf("print\n"), K_ERROR("test_assert_do_ret"));
    return K_OK();
}

k_Result test_assert_do_err() {
    k_ASSERT_DO_ERR(1 > 2, printf("print\n"), "test_assert_do_err");
    return K_OK();
}

k_Result test_assert_ret() {
    k_ASSERT_RET(1 > 2, K_ERROR("test_assert_ret"));
    return K_OK();
}

k_Result test_assert_err() {
    k_ASSERT_ERR(1 > 2, "test_assert_err");
    return K_OK();
}

int main(void) {
    k_Result res;

    res = K_ERROR("test error");
    k_RESULT_PRINT(res);

    res = K_OK();
    k_RESULT_PRINT(res);

    res = test_assert_do();
    k_RESULT_PRINT(res);

    res = test_assert_do_ret();
    k_RESULT_PRINT(res);

    res = test_assert_do_err();
    k_RESULT_PRINT(res);

    res = test_assert_ret();
    k_RESULT_PRINT(res);

    res = test_assert_err();
    k_RESULT_PRINT(res);
}