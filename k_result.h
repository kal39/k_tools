//====================================================================================================================//
// k_result
//====================================================================================================================//

//--------------------------------------------------------------------------------------------------------------------//
// interface
//--------------------------------------------------------------------------------------------------------------------//

#pragma once

#include <stdint.h>
#include <stdio.h>

typedef enum k_Bool {
    k_TRUE = 1,
    k_FALSE = 0,
} k_Bool;

typedef struct k_Result {
    k_Bool ok;
    char* file;
    int32_t line;
    char* message;
} k_Result;

#define K_OK() k_ok_(__FILE__, __LINE__)
#define K_ERROR(message) k_error_(__FILE__, __LINE__, (message))

k_Result k_ok_(char* file, int32_t line);
k_Result k_error_(char* file, int32_t line, char* message);

#define k_ASSERT_DO(cond, action)                                              \
    do {                                                                       \
        if (!(cond)) {                                                         \
            do {                                                               \
                action;                                                        \
            } while (0);                                                       \
        }                                                                      \
    } while (0)

#define k_ASSERT_DO_RET(cond, action, value)                                   \
    k_ASSERT_DO((cond), action; return (value));

#define k_ASSERT_DO_ERR(cond, action, message)                                 \
    k_ASSERT_DO_RET((cond), (action), K_ERROR(message))

#define k_ASSERT_RET(cond, value) k_ASSERT_DO((cond), return (value))
#define k_ASSERT_ERR(cond, message) k_ASSERT_RET((cond), K_ERROR(message))
#define k_ASSERT(cond) k_ASSERT_ERR((cond), ERROR("k_ASSERT failed"))

#define k_RESULT_PRINT(result)                                                 \
    do {                                                                       \
        printf(                                                                \
            "\e[1m%s:%d\e[0m, \e[1m%s\e[0m: %s\n",                             \
            (result).file,                                                     \
            (result).line,                                                     \
            (result).ok ? "\e[32mOK\e[39m" : "\e[31mERROR\e[39m",              \
            (result).message                                                   \
        );                                                                     \
    } while (0)

//--------------------------------------------------------------------------------------------------------------------//
// implementation
//--------------------------------------------------------------------------------------------------------------------//

#ifdef K_RESULT_IMPLEMENTATION

#ifdef K_RESULT_USE_K_LOG
#include "k_log.h"
#endif

k_Result k_ok_(char* file, int32_t line) {
    return (k_Result){k_TRUE, __FILE__, __LINE__, "no errors here :)"};
}

k_Result k_error_(char* file, int32_t line, char* message) {
#ifdef K_RESULT_USE_K_LOG
    k_log_(file, line, k_SEVERITY_HIGH, "\e[1mk_result\e[0m: %s", message);
#endif
    return (k_Result){k_FALSE, __FILE__, __LINE__, message};
}

#endif