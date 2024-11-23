#include <readline/history.h>
#include "history.h"
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

    FILE* history_file = fopen(config_get_history_file(), "r");
    if (!history_file) {
        return;
    }
    char* line;
    size_t _;
    while (true) {
        line = NULL;
        ssize_t line_size = getline(&line, &_, history_file);
        if (line_size > 0) {
            line[strlen(line)-1] = 0; // Removing trailing new line char
            history_add(line);
        } else {
            if (line) {
                free(line);
            }
            fclose(history_file);
            return;
        }
    }
}

void history_add(char* line) {
    add_history(line);
    free(history_circular_buffer[circular_buffer_index]);
    history_circular_buffer[circular_buffer_index++] = line;
}

void history_deinit(void) {
    FILE* history_file = fopen(config_get_history_file(), "w");
    if (history_file) {
        for (int i=0; i<history_size; i++) {
            if (history_circular_buffer[i]) {
                fprintf(history_file, "%s\n", history_circular_buffer[i]);
            }
        }
        fclose(history_file);
    }
    for (int i=0; i<history_size; i++) {
        free(history_circular_buffer[i]);
    }
    free(history_circular_buffer);
}

