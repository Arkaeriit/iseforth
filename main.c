#include <readline/readline.h>
#include "dynamic_array.h"
#include "output_text.h"
#include "completion.h"
#include <SEForth.h>
#include "history.h"
#include "config.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static void clear_color(void) {
    printf("\e[m");
}

static forth_state_t* init(void) {
    output_init();
    forth_state_t* fs = sef_init();
    completion_init(fs);
    config_init(fs);
    history_init();
    rl_completion_entry_function = completion_generator;
    return fs;
}

static void deinit(forth_state_t* fs) {
    history_deinit();
    completion_deinit();
    sef_free(fs);
    output_deinit();
}

int main(void) {
    forth_state_t* fs = init();
    while (sef_is_running(fs)) {
        char* line = readline(config_get_prompt());
        clear_color();
        if (line == NULL) {
            break;
        }
        if (strlen(line) > 0) {
            history_add(line);
        }
        sef_parse_string(fs, line);
        sef_parse_char(fs, '\n');
        output_display();
        free(line);
    }
    deinit(fs);
    return 0;
}

