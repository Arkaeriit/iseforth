#include <readline/history.h>
#include "config.h"
#include <stdlib.h>
#include <string.h>

static char** history_circular_buffer;
static int history_size;
static size_t circular_buffer_index;

void history_init(void) {
    history_size = config_get_history_size(); 
    circular_buffer_index = 0;
    history_circular_buffer = calloc(history_size, sizeof(char*));
    // TODO: read history file to pre-fill the buffer
    for (int i=0; i<history_size; i++) {
        if (!history_circular_buffer[i]) {
            break;
        }
        add_history(history_circular_buffer[i]);
    }
}

void history_add(const char* line) {
    free(history_circular_buffer[circular_buffer_index]);
    history_circular_buffer[circular_buffer_index++] = strdup(line);
    add_history(line);
}

void history_deinit(void) {
    // TODO: write history file
    for (int i=0; i<history_size; i++) {
        free(history_circular_buffer[i]);
    }
    free(history_circular_buffer);
}

