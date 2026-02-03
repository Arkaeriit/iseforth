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

static_assert(SEF_ARG_AND_EXIT_CODE && SEF_PROGRAMMING_TOOLS, "iseforth needs a version of seforth compiled with the arg and exit code word set and the programming tools word set.");
static_assert(SEF_ABORT_STOP_FORTH, "iseforth should needs a version of seforth with `SEF_ABORT_STOP_FORTH` enabled.");

static void clear_color(void) {
    printf("\e[m");
}

static sef_forth_state_t* init(void) {
    output_init();
    sef_forth_state_t* fs = malloc(sizeof(sef_forth_state_t));
    sef_init(fs);
    completion_init(fs);
    config_init(fs);
    history_init();
    rl_completion_entry_function = completion_generator;
    return fs;
}

static void deinit(sef_forth_state_t* fs) {
    history_deinit();
    completion_deinit();
    free(fs);
    output_deinit();
}

void eval_file(sef_forth_state_t* fs, FILE* f) {
    rewind(f);
    fseek(f, 0L, SEEK_END);
    size_t file_size = ftell(f);
    rewind(f);

    char* content = malloc(file_size + 1);
    if (content == NULL) {
        fprintf(stderr, "Can't allocate buffer to read file.\n");
        exit(-1);
    }

    fread(content, 1, file_size, f);
    content[file_size] = 0;
    sef_eval_string(fs, content);
    free(content);
    fclose(f);
}

static bool try_to_read_new_file(sef_forth_state_t* fs) {
    if (!sef_ready_to_run(fs)) {
        return false;
    }

    sef_force_string_interpretation(fs, "next-arg");
    size_t size = (size_t) sef_pop_from_data_stack(fs);
    const char* arg = (const char*) sef_pop_from_data_stack(fs);

    if (arg == 0 || size == 0) {
        return false;
    }

    FILE* f = fopen(arg, "r");
    if (!f) {
        fprintf(stderr, "Unable to read %s.\n", arg);
        sef_force_string_interpretation(fs, "bye");
        return false;
    }

    eval_file(fs, f);

    output_display();
    return true;
}

static void read_all_args(sef_forth_state_t* fs) {
    while (try_to_read_new_file(fs));
}

int main(int argc, char** argv) {
    sef_forth_state_t* fs = init();
    sef_feed_arguments(fs, argc, argv);
    read_all_args(fs);
    while (!sef_asked_bye(fs)) {
        char* line = readline(config_get_prompt());
        clear_color();
        if (line == NULL) {
            break;
        }
        sef_eval_string(fs, line);
        if (strlen(line) > 0) {
            history_add(line);
        } else {
            free(line);
        }
        output_display();
        if (!sef_ready_to_run(fs) && !sef_asked_bye(fs)) {
            sef_restart(fs);
        }
    }
    deinit(fs);
    return 0;
}

