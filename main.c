#include <readline/readline.h>
#include "dynamic_array.h"
#include "output_text.h"
#include "completion.h"
#include <SEForth.h>
#include "history.h"
#include "config.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

static_assert(SEF_STORE_NAME == 1 && SEF_PROGRAMMING_TOOLS == 1, "iseforth needs a version of seforth compiled with programming tools and names saved.");

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

static bool try_to_read_new_file(forth_state_t* fs) {
    if (!sef_is_running(fs)) {
        return false;
    }

    sef_parse_string(fs, "next-arg ");
    size_t size = (size_t) sef_pop_data(fs);
    const char* arg = (const char*) sef_pop_data(fs);
    (void) size;

    if (arg == NULL) {
        return false;
    }

    FILE* f = fopen(arg, "r");
    if (!f) {
        fprintf(stderr, "Unable to read %s.\n", arg);
        sef_parse_string(fs, "bye ");
        return false;
    }

    char c;
    while ((c = fgetc(f)) != EOF) {
        sef_parse_char(fs, c);
    }
    fclose(f);

    output_display();
    return true;
}

static void read_all_args(forth_state_t* fs) {
    while (try_to_read_new_file(fs));
}

int main(int argc, char** argv) {
    forth_state_t* fs = init();
    sef_feed_arguments(fs, argc, argv);
    read_all_args(fs);
    while (!sef_asked_bye(fs)) {
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
        if (!sef_is_running(fs)) {
            sef_restart(fs);
        }
    }
    deinit(fs);
    return 0;
}

