#include <readline/readline.h>
#include <readline/history.h>
#include "dynamic_array.h"
#include "output_text.h"
#include "completion.h"
#include <SEForth.h>
#include "config.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    output_init();
    forth_state_t* fs = sef_init();
    completion_init(fs);
    config_init(fs);
    rl_completion_entry_function = completion_generator;
    while (sef_is_running(fs)) {
        char* line = readline(sef_is_compiling(fs) ? config_get_compiling_prompt() : config_get_prompt());
        if (line == NULL) {
            break;
        }
        if (strlen(line) > 0) {
            add_history(line);
        }
        sef_parse_string(fs, line);
        sef_parse_char(fs, '\n');
        output_display();
        free(line);
    }
    completion_deinit();
    sef_free(fs);
    output_deinit();
    return 0;
}

