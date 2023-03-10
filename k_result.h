//====================================================================================================================//
// k_result
//====================================================================================================================//

//--------------------------------------------------------------------------------------------------------------------//
// interface
//--------------------------------------------------------------------------------------------------------------------//

#ifndef K_RESULT_INCLUDE_GUARD
#define K_RESULT_INCLUDE_GUARD

#include <stdint.h>
#include <stdio.h>

typedef enum k_Bool {
    K_TRUE = 1,
    K_FALSE = 0,
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

#define K_ASSERT_DO(cond, action)                                              \
    do {                                                                       \
        if (!(cond)) {                                                         \
            do {                                                               \
                action;                                                        \
            } while (0);                                                       \
        }                                                                      \
    } while (0)

#define K_ASSERT_DO_RET(cond, action, value)                                   \
    K_ASSERT_DO((cond), action; return (value));

#define K_ASSERT_DO_ERR(cond, action, message)                                 \
    K_ASSERT_DO_RET((cond), (action), K_ERROR(message))

#define K_ASSERT_RET(cond, value) K_ASSERT_DO((cond), return (value))
#define K_ASSERT_ERR(cond, message) K_ASSERT_RET((cond), K_ERROR(message))
#define K_ASSERT(cond) K_ASSERT_ERR((cond), ERROR("K_ASSERT failed"))

#define K_RESULT_PRINT(result)                                                 \
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
#endif // K_RESULT_INCLUDE_GUARD

k_Result k_ok_(char* file, int32_t line) {
    return (k_Result){K_TRUE, __FILE__, __LINE__, "no errors here :)"};
}

k_Result k_error_(char* file, int32_t line, char* message) {
#ifdef K_RESULT_USE_K_LOG
    k_log_(file, line, K_SEVERITY_HIGH, "\e[1mk_result\e[0m: %s", message);
#endif // K_RESULT_INCLUDE_GUARD
    return (k_Result){K_FALSE, __FILE__, __LINE__, message};
}

#endif // K_RESULT_IMPLEMENTATION
#endif // K_RESULT_INCLUDE_GUARD