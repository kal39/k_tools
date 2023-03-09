//====================================================================================================================//
// k_log
//====================================================================================================================//

//--------------------------------------------------------------------------------------------------------------------//
// interface
//--------------------------------------------------------------------------------------------------------------------//

#pragma once

#include <stdint.h>

typedef enum k_LogSeverity {
    k_SEVERITY_HIGH = 3,
    k_SEVERITY_MEDIUM = 2,
    k_SEVERITY_LOW = 1,
    k_SEVERITY_INFO = 0,
} k_LogSeverity;

typedef struct k_LogEntry {
    k_LogSeverity severity;
    char* file;
    int32_t line;
    char* message;
} k_LogEntry;

typedef void(k_log_cb_function)(k_LogEntry entry);

#define k_LOG_H(...) k_log_(__FILE__, __LINE__, k_SEVERITY_HIGH, __VA_ARGS__);
#define k_LOG_M(...) k_log_(__FILE__, __LINE__, k_SEVERITY_MEDIUM, __VA_ARGS__);
#define k_LOG_L(...) k_log_(__FILE__, __LINE__, k_SEVERITY_LOW, __VA_ARGS__);
#define k_LOG_I(...) k_log_(__FILE__, __LINE__, k_SEVERITY_INFO, __VA_ARGS__);

void k_log_(char* file, int32_t line, k_LogSeverity severity, char* fmt, ...);

void k_log_print_log_level(k_LogSeverity minLogLevel);
void k_log_set_cb_function(k_log_cb_function cb_function);
void k_set_max_filename_len(uint32_t maxLen);

//--------------------------------------------------------------------------------------------------------------------//
// implementation
//--------------------------------------------------------------------------------------------------------------------//

#ifdef K_LOG_IMPLEMENTATION

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static k_LogSeverity G_minLogLevel;
static k_log_cb_function* G_cb_function;
static uint32_t g_maxFilenameLen;

static char* format_sev(k_LogSeverity sev) {
    switch (sev) {
        case k_SEVERITY_HIGH: return "\e[31mHIGH  \e[39m";
        case k_SEVERITY_MEDIUM: return "\e[33mMEDIUM\e[39m";
        case k_SEVERITY_LOW: return "\e[35mLOW   \e[39m";
        case k_SEVERITY_INFO: return "\e[34mINFO  \e[39m";
        default: return "      ";
    }
}

static char* format_pos(char* file, int32_t line) {
    uint32_t maxFilenameLen = g_maxFilenameLen == 0 ? 50 : g_maxFilenameLen;

    uint32_t strLen = snprintf(NULL, 0, "%s:%d", file, line) + 1;
    char* fmtPos = malloc(strLen);
    snprintf(fmtPos, strLen, "%s:%d", file, line);

    if (strLen - 1 < maxFilenameLen) {
        fmtPos = realloc(fmtPos, maxFilenameLen + 1);
        for (uint32_t i = strLen - 1; i < maxFilenameLen; i++) fmtPos[i] = ' ';

    } else if (strLen - 1 > maxFilenameLen) {
        fmtPos = realloc(fmtPos, maxFilenameLen + 1);
        for (uint32_t i = 0; i < 3; i++) fmtPos[maxFilenameLen - 3 + i] = '.';
    }

    fmtPos[maxFilenameLen] = '\0';
    return fmtPos;
}

static char* format_msg(char* msg) {
    int padding = (g_maxFilenameLen == 0 ? 50 : g_maxFilenameLen) + 12;
    char* fmtMsg = malloc(strlen(msg) + 1);
    memcpy(fmtMsg, msg, strlen(msg) + 1);

    for (int i = 0; fmtMsg[i] != '\0'; i++) {
        if (fmtMsg[i] == '\n') {
            int lenOld = strlen(fmtMsg);
            int lenNew = lenOld + padding;
            fmtMsg = realloc(fmtMsg, lenNew + 1);
            memcpy(&fmtMsg[i + 1 + padding], &fmtMsg[i + 1], lenOld - i);
            for (int j = 0; j < padding; j++) fmtMsg[i + 1 + j] = ' ';
            fmtMsg[lenNew] = '\0';
        }
    }

    return fmtMsg;
}

void k_log_(char* file, int32_t line, k_LogSeverity severity, char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int len = vsnprintf(NULL, 0, fmt, args) + 1;
    va_end(args);
    char* msg = malloc(len);
    va_start(args, fmt);
    vsnprintf(msg, len, fmt, args);
    va_end(args);

    if (severity >= G_minLogLevel) {
        char* fmtSev = format_sev(severity);
        char* fmtPos = format_pos(file, line);
        char* fmtMsg = format_msg(msg);

        printf(
            "\e[1m%s\e[0m | \e[1m%s\e[0m | %s\e[0m\n",
            fmtSev,
            fmtPos,
            fmtMsg
        );

        free(fmtPos);
        free(fmtMsg);
    }

    if (G_cb_function != NULL)
        G_cb_function((k_LogEntry){severity, file, line, msg});

    free(msg);
}

void k_log_print_log_level(k_LogSeverity minLogLevel) {
    G_minLogLevel = minLogLevel;
}

void k_log_set_cb_function(k_log_cb_function cb_function) {
    G_cb_function = cb_function;
    k_LOG_I("callback function set to %p", cb_function);
}

void k_set_max_filename_len(uint32_t maxLen) {
    g_maxFilenameLen = maxLen;
}

#endif