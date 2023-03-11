#include "../k_result.h"

k_Result test_assert_do() {
    K_ASSERT_DO(1 > 2, printf("print\n"); return K_ERROR("test_assert_do"));
    return K_OK;
}

k_Result test_assert_do_ret() {
    K_ASSERT_DO_RET(1 > 2, printf("print\n"), K_ERROR("test_assert_do_ret"));
    return K_OK;
}

k_Result test_assert_do_err() {
    K_ASSERT_DO_ERR(1 > 2, printf("print\n"), "test_assert_do_err");
    return K_OK;
}

k_Result test_assert_ret() {
    K_ASSERT_RET(1 > 2, K_ERROR("test_assert_ret"));
    return K_OK;
}

k_Result test_assert_err() {
    K_ASSERT_ERR(1 > 2, "test_assert_err");
    return K_OK;
}

int main(void) {
    k_Result res;

    res = K_ERROR("test error");
    K_RESULT_PRINT(res);

    res = K_OK;
    K_RESULT_PRINT(res);

    res = test_assert_do();
    K_RESULT_PRINT(res);

    res = test_assert_do_ret();
    K_RESULT_PRINT(res);

    res = test_assert_do_err();
    K_RESULT_PRINT(res);

    res = test_assert_ret();
    K_RESULT_PRINT(res);

    res = test_assert_err();
    K_RESULT_PRINT(res);
}