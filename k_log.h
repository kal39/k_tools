//====================================================================================================================//
// k_log
//====================================================================================================================//

//--------------------------------------------------------------------------------------------------------------------//
// interface
//--------------------------------------------------------------------------------------------------------------------//

#pragma once

#define k_note(...) _k_note(__FILE__, __LINE__, __VA_ARGS__)
#define k_debug(...) _k_debug(__FILE__, __LINE__, __VA_ARGS__)
#define k_error(...) _k_error(__FILE__, __LINE__, __VA_ARGS__)
#define k_panic(error_code, ...) _k_panic(error_code, __FILE__, __LINE__, __VA_ARGS__)

void k_enable_debug_logs();
void k_disable_debug_logs();

void k_enable_flush();
void k_disable_flush();

void k_set_max_filename_length(int max_length);

void k_log_to_file(char *filename);
void k_stop_file_log();

void _k_note(char *file, int line, char *format, ...);
void _k_debug(char *file, int line, char *format, ...);
void _k_error(char *file, int line, char *format, ...);
void _k_panic(int error_code, char *file, int line, char *format, ...);

//--------------------------------------------------------------------------------------------------------------------//
// implementation
//--------------------------------------------------------------------------------------------------------------------//

#ifdef K_LOG_IMPLEMENTATION

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char _k_debug_enabled = 0;
char _k_flush_enabled = 1;
int _k_max_filename_length = 25;
FILE *_k_log_file = NULL;

//---- private -------------------------------------------------------------------------------------------------------//

static char *_get_formatted_filename(char *file, int line) {
	int str_len = snprintf(NULL, 0, "%s:%d", file, line) + 1;
	char *filename = malloc(str_len);
	snprintf(filename, str_len, "%s:%d", file, line);

	if (str_len - 1 < _k_max_filename_length) {
		filename = realloc(filename, _k_max_filename_length + 1);

		for (int i = str_len - 1; i < _k_max_filename_length; i++) filename[i] = ' ';
		filename[_k_max_filename_length] = '\0';

	} else if (str_len - 1 > _k_max_filename_length) {
		filename = realloc(filename, _k_max_filename_length + 1);

		for (int i = 0; i < 3; i++) filename[_k_max_filename_length - 3 + i] = '.';
		filename[_k_max_filename_length] = '\0';
	}

	return filename;
}

static char *_get_formatted_message(char *message) {
	int padding = _k_max_filename_length + 11;

	char *formatted_message = malloc(strlen(message) + 1);
	memcpy(formatted_message, message, strlen(message) + 1);

	for (int i = 0; formatted_message[i] != '\0'; i++) {
		if (formatted_message[i] == '\n') {
			int old_len = strlen(formatted_message);
			int new_len = old_len + padding;

			formatted_message = realloc(formatted_message, new_len + 1);
			memcpy(&formatted_message[i + 1 + padding], &formatted_message[i + 1], old_len - i);

			for (int j = 0; j < padding; j++) formatted_message[i + 1 + j] = ' ';
			formatted_message[new_len] = '\0';
		}
	}

	return formatted_message;
}

static void _print_message(char *type, int color, char *file, int line, char *message) {
	char *formatted_filename = _get_formatted_filename(file, line);
	char *formatted_message = _get_formatted_message(message);

	printf("\e[1m\e[%dm%s\e[0m | \e[1m%s\e[0m | %s\n", color, type, formatted_filename, formatted_message);
	if (_k_flush_enabled) fflush(stdout);

	if (_k_log_file != NULL) fprintf(_k_log_file, "%s | %s | %s\n", type, formatted_filename, formatted_message);

	free(formatted_filename);
	free(formatted_message);
}

#define _parse_message(dest, format)                                                                                   \
	{                                                                                                                  \
		va_list args;                                                                                                  \
		va_start(args, format);                                                                                        \
		int len = vsnprintf(NULL, 0, format, args) + 1;                                                                \
		va_end(args);                                                                                                  \
                                                                                                                       \
		dest = malloc(len);                                                                                            \
                                                                                                                       \
		va_start(args, format);                                                                                        \
		vsnprintf(dest, len, format, args);                                                                            \
		va_end(args);                                                                                                  \
	}

//---- public --------------------------------------------------------------------------------------------------------//

void k_enable_debug_logs() {
	_k_debug_enabled = 1;
	k_debug("K_LOG: enabled debug logs");
}

void k_disable_debug_logs() {
	k_debug("K_LOG: disabling debug logs");
	_k_debug_enabled = 0;
}

void k_enable_flush() {
	_k_flush_enabled = 1;
	k_debug("K_LOG: enabled flush");
}

void k_disable_flush() {
	_k_flush_enabled = 0;
	k_debug("K_LOG: disabled flush");
}

void k_log_to_file(char *filename) {
	_k_log_file = fopen(filename, "a");
	k_debug("K_LOG: logging to file [%s]", filename);
}
void k_stop_file_log() {
	k_debug("K_LOG: disabling file logging");
	fclose(_k_log_file);
	_k_log_file = NULL;
}

void k_set_max_filename_length(int max_length) {
	_k_max_filename_length = max_length;
	k_debug("K_LOG: set max filename length to %d", max_length);
}

void _k_note(char *file, int line, char *format, ...) {
	char *message;
	_parse_message(message, format);
	_print_message("NOTE ", 32, file, line, message);
	free(message);
}

void _k_debug(char *file, int line, char *format, ...) {
	if (!_k_debug_enabled) return;

	char *message;
	_parse_message(message, format);
	_print_message("DEBUG", 33, file, line, message);
	free(message);
}

void _k_error(char *file, int line, char *format, ...) {
	char *message;
	_parse_message(message, format);
	_print_message("ERROR", 31, file, line, message);
	free(message);
}

void _k_panic(int error_code, char *file, int line, char *format, ...) {
	char *message;
	_parse_message(message, format);
	_print_message("PANIC", 31, file, line, message);
	free(message);

	if (_k_log_file != NULL) fclose(_k_log_file);
	exit(error_code);
}

#endif