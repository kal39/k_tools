#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define malloc(size) _k_mem_track_malloc(size, __FILE__, __LINE__)
#define realloc(ptr, size) _k_mem_track_realloc(ptr, size, __FILE__, __LINE__)
#define free(ptr) _k_mem_track_free(ptr, __FILE__, __LINE__)

void *_k_mem_track_malloc(size_t size, char *file, int line);
void *_k_mem_track_realloc(void *oldPtr, size_t size, char *file, int line);
void _k_mem_track_free(void *ptr, char *file, int line);
void k_mem_track_print(bool printFree);

#ifdef K_MEM_TRACKER_IMPLEMENTATION

typedef enum k_mem_ActionType {
	MALLOC,
	REALLOC,
	FREE,
} k_mem_ActionType;

typedef struct k_mem_Action {
	k_mem_ActionType type;
	void *ptr;
	size_t size;
	char *file;
	int line;
} k_mem_Action;

typedef struct k_mem_Entry {
	int size;
	k_mem_Action *actions;
} k_mem_Entry;

typedef struct k_mem_Tracker {
	int capacity;
	int size;
	k_mem_Entry *entries;

	long mallocs;
	long reallocs;
	long frees;
	long memUsage;
	long maxMemUsage;
} k_mem_Tracker;

#undef malloc
#undef realloc
#undef free

k_mem_Tracker tracker = (k_mem_Tracker){0, 0, NULL, 0, 0, 0, 0, 0};

static k_mem_Entry _init_entry() {
	return (k_mem_Entry){0, NULL};
}

static void *_current_ptr(k_mem_Entry entry) {
	return entry.actions[entry.size - 1].ptr;
}

static long _current_size(k_mem_Entry entry) {
	return entry.actions[entry.size - 1].size;
}

static void _add_action(k_mem_Entry *entry, k_mem_Action action) {
	entry->actions = realloc(entry->actions, sizeof(k_mem_Action) * (++entry->size));
	entry->actions[entry->size - 1] = action;
}

static void _log_malloc(void *ptr, size_t size, char *file, int line) {
	tracker.mallocs++;
	tracker.memUsage += size;
	if (tracker.memUsage > tracker.maxMemUsage) tracker.maxMemUsage = tracker.memUsage;

	if (tracker.capacity == 0) tracker.entries = malloc(sizeof(k_mem_Entry) * (tracker.capacity = 8));
	if (tracker.size == tracker.capacity)
		tracker.entries = realloc(tracker.entries, sizeof(k_mem_Entry) * (tracker.capacity *= 2));

	k_mem_Entry entry = _init_entry();
	_add_action(&entry, (k_mem_Action){MALLOC, ptr, size, file, line});

	tracker.entries[tracker.size++] = entry;
}

static void _log_realloc(void *oldPtr, void *newPtr, size_t size, char *file, int line) {
	tracker.reallocs++;

	for (int i = 0; i < tracker.size; i++) {
		if (_current_ptr(tracker.entries[i]) == oldPtr) {
			tracker.memUsage += size - _current_size(tracker.entries[i]);
			if (tracker.memUsage > tracker.maxMemUsage) tracker.maxMemUsage = tracker.memUsage;
			_add_action(&tracker.entries[i], (k_mem_Action){REALLOC, newPtr, size, file, line});
			break;
		}
	}
}

static void _log_free(void *ptr, char *file, int line) {
	tracker.frees++;

	for (int i = 0; i < tracker.size; i++) {
		if (_current_ptr(tracker.entries[i]) == ptr) {
			tracker.memUsage -= _current_size(tracker.entries[i]);
			_add_action(&tracker.entries[i], (k_mem_Action){FREE, NULL, 0, file, line});
			break;
		}
	}
}

void *_k_mem_track_malloc(size_t size, char *file, int line) {
	void *ptr = malloc(size);
	_log_malloc(ptr, size, file, line);
	return ptr;
}

void *_k_mem_track_realloc(void *oldPtr, size_t size, char *file, int line) {
	void *newPtr = realloc(oldPtr, size);
	_log_realloc(oldPtr, newPtr, size, file, line);
	return newPtr;
}

void _k_mem_track_free(void *ptr, char *file, int line) {
	free(ptr);
	_log_free(ptr, file, line);
}

void k_mem_track_print(bool printFree) {
	printf("\e[1mMemory Status\e[0m\n\n");

	printf("malloc count    : %ld\n", tracker.mallocs);
	printf("realloc count   : %ld\n", tracker.reallocs);
	printf("free count      : %ld\n\n", tracker.frees);

	printf("max memory usage: %ld\n", tracker.maxMemUsage);
	printf("allocated memory: %ld\n", tracker.memUsage);
	printf("\n");

	int maxLen = 0;
	for (int i = 0; i < tracker.size; i++) {
		k_mem_Entry entry = tracker.entries[i];
		for (int j = 0; j < entry.size; j++) {
			k_mem_Action action = entry.actions[j];
			int len = snprintf(NULL, 0, "%s:%d", action.file, action.line);
			if (len > maxLen) maxLen = len;
		}
	}

	printf(" \e[32m✓\e[0m/\e[31m✖\e[0m ┃ Start Address  ┃ History\n");
	printf("━━━━━╋━━━━━━━━━━━━━━━━╋━━━━━━━━━━━\n");

	for (int i = 0; i < tracker.size; i++) {
		k_mem_Entry entry = tracker.entries[i];

		if (printFree || _current_ptr(entry) != NULL) {
			printf(_current_ptr(entry) == NULL ? "\e[32m  ✓ \e[0m" : "\e[31m  ✖ \e[0m");
			printf(" ┃ %p ┃ ", entry.actions[0].ptr);

			for (int j = 0; j < entry.size; j++) {
				k_mem_Action action = entry.actions[j];
				switch (action.type) {
					case MALLOC: {
						printf("%6ld [", action.size);
						int len = printf("%s:%d", entry.actions[0].file, entry.actions[0].line);
						printf("] ");
						for (int i = 0; i < maxLen - len; i++) printf(" ");
						break;
					}
					case REALLOC: {
						printf("→ %6ld [", action.size);
						int len = printf("%s:%d", entry.actions[0].file, entry.actions[0].line);
						printf("] ");
						for (int i = 0; i < maxLen - len; i++) printf(" ");
						break;
					}
					case FREE: {
						printf("→ \e[32m FREE \e[0m [");
						int len = printf("%s:%d", entry.actions[0].file, entry.actions[0].line);
						printf("] ");
						for (int i = 0; i < maxLen - len; i++) printf(" ");
						break;
					}
				}
			}
			printf("\n");
		}
	}
	printf("\n");
}

#define malloc(size) _k_mem_track_malloc(size, __FILE__, __LINE__)
#define realloc(ptr, size) _k_mem_track_realloc(ptr, size, __FILE__, __LINE__)
#define free(ptr) _k_mem_track_free(ptr, __FILE__, __LINE__)

#endif