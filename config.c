#include <readline/readline.h>
#include "completion.h"
#include <SEForth.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static forth_state_t* state_copy;

/* --------------------------- State interactions --------------------------- */

static void read_int_from_forth(forth_state_t* fs, const char* constant_name, sef_int_t* value) {
    if (sef_is_compiling(fs)) {
        return;
    }
    sef_parse_string(fs, constant_name);
    sef_parse_char(fs, ' ');
    *value = sef_pop_data(fs);
}

static size_t read_string_from_forth(forth_state_t* fs, const char* constant_name, char* dest) {
    sef_parse_string(fs, constant_name);
    sef_parse_char(fs, ' ');
    size_t size = (size_t) sef_pop_data(fs);
    char* str = (char*) sef_pop_data(fs);
    if (dest != NULL) {
        memcpy(dest, str, size);
        dest[size] = 0;
    }
    return size+1;
}

static void allocate_and_read_string_from_froth(forth_state_t* fs, const char* constant_name, char** dest) {
    if (sef_is_compiling(fs)) {
        return;
    }
    free(*dest);
    *dest = NULL;
    size_t size = read_string_from_forth(fs, constant_name, NULL);
    *dest = malloc(size);
    read_string_from_forth(fs, constant_name, *dest);
}

static char* get_prompt(void) {
    static char* prompt;
    allocate_and_read_string_from_froth(state_copy, "isef_prompt" ,&prompt);
    return prompt;
}

static char* get_compiling_prompt(void) {
    static char* prompt;
    allocate_and_read_string_from_froth(state_copy, "isef_compiling_prompt" ,&prompt);
    return prompt;
}

static char* get_prompt_color(void) {
    static char* prompt;
    allocate_and_read_string_from_froth(state_copy, "isef_prompt_color" ,&prompt);
    return prompt;
}

static char* get_code_color(void) {
    static char* prompt;
    allocate_and_read_string_from_froth(state_copy, "isef_code_color" ,&prompt);
    return prompt;
}

static char* get_current_prompt(void) {
    return sef_is_compiling(state_copy) ?
        get_compiling_prompt() :
        get_prompt();
}

/* ---------------------------- Config points API --------------------------- */

char* config_get_prompt(void) {
    static char* prompt = NULL;
    free(prompt);
    size_t needed_size = 5 + strlen(get_current_prompt()) + strlen(get_prompt_color()) + strlen(get_code_color());
    prompt = malloc(needed_size);
    sprintf(prompt, "%c%s%c%s%c%s%c", RL_PROMPT_START_IGNORE, get_prompt_color(), RL_PROMPT_END_IGNORE, get_current_prompt(), RL_PROMPT_START_IGNORE, get_code_color(), RL_PROMPT_END_IGNORE);
    return prompt;
}

int config_get_history_size(void) {
    static sef_int_t history_file_size;
    read_int_from_forth(state_copy, "isef_history_file_size", &history_file_size);
    return (int) history_file_size;
}

char* config_get_history_file(void) {
    static char* history_file;
    allocate_and_read_string_from_froth(state_copy, "isef_history_file", &history_file);
    return history_file;
}

static void read_a_config_file(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        return;
    }
    char c;
    while ((c = fgetc(f)) != EOF) {
        sef_parse_char(state_copy, c);
    }
    fclose(f);
}

/* ---------------------------- rc files reading ---------------------------- */

static void read_config_files_from_env_path(const char* env, const char* filename) {
    char* env_dir = getenv(env);
    if (env_dir) {
        size_t path_len = strlen(filename) + strlen(env_dir) + 2;
        char* path = malloc(path_len);
        sprintf(path, "%s/%s", env_dir, filename);
        read_a_config_file(path);
        free(path);
    }
}

static void read_rc_files(void) {
    read_config_files_from_env_path("XDG_CONFIG_HOME", "iseforthrc.frt");
    read_config_files_from_env_path("HOME", ".iseforthrc.frt");
    read_config_files_from_env_path("PWD", "iseforthrc.frt");
}

static const char* default_configs[][2] = {
    {"isef_prompt", "s\"  ok \""},
    {"isef_compiling_prompt", "s\" ... \""},
    {"isef_prompt_color", "isef_blue"},
    {"isef_code_color", "isef_yellow"},
    {"isef_history_file", "s\" /tmp/iseforth-history\""},
    {"isef_history_file_size", "1000"},
    {NULL, NULL},
};

static void set_default_config_if_no_value(const char* config_name, const char* default_code_to_execute) {
    if (is_word_defined(config_name)) {
        return;
    }
    sef_parse_string(state_copy, ": ");
    sef_parse_string(state_copy, config_name);
    sef_parse_string(state_copy, " ");
    sef_parse_string(state_copy, default_code_to_execute);
    sef_parse_string(state_copy, " ; ");
}

static void init_default_config(void) {
    int i = 0;
    while(default_configs[i][0]) {
        set_default_config_if_no_value(default_configs[i][0], default_configs[i][1]);
        i++;
    }
}

/* ---------------------------------- Init ---------------------------------- */

extern const char* config_literals;

void config_init(forth_state_t* fs) {
    state_copy = fs;
    sef_parse_string(fs, config_literals);
    read_rc_files();
    init_default_config();
    get_prompt();
    config_get_prompt();
    get_compiling_prompt();
    config_get_history_size();
}

