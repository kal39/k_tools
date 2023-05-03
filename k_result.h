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

#include "k_bool.h"

typedef struct k_Result {
    k_Bool ok;
    const char* file;
    int32_t line;
    const char* message;
} k_Result;

#define K_OK                                                                   \
    (k_Result) { K_TRUE, __FILE__, __LINE__, "no errors here :)" }
#define K_ERROR(message)                                                       \
    (k_Result) { K_FALSE, __FILE__, __LINE__, (message) }

#define K_ASSERT(cond, msg)                                                    \
    do {                                                                       \
        if (!(cond)) return K_ERROR(msg);                                      \
    } while (0)

#define K_TRY(action)                                                          \
    do {                                                                       \
        k_Result k_internal_ret_ = (action);                                   \
        if (!k_internal_ret_.ok) return k_internal_ret_;                       \
    } while (0)

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

#endif // K_RESULT_INCLUDE_GUARD