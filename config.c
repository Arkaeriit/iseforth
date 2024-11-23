#include <SEForth.h>
#include <string.h>
#include <stdlib.h>
#include "config.h"
#include <stdio.h>

static forth_state_t* state_copy;

extern const char* default_config;

static void read_constant_from_forth(forth_state_t* fs, const char* constant_name, void* dest) {
    if (sef_is_compiling(fs)) {
        return;
    }
    sef_parse_string(fs, constant_name);
    sef_parse_char(fs, ' ');
    sef_int_t value = sef_pop_data(fs);
    memcpy(dest, &value, sizeof(sef_int_t));
}

static char* get_prompt(void) {
    static char* prompt;
    read_constant_from_forth(state_copy, "isef_prompt" ,&prompt);
    return prompt;
}

static char* get_compiling_prompt(void) {
    static char* prompt;
    read_constant_from_forth(state_copy, "isef_compiling_prompt" ,&prompt);
    return prompt;
}

static char* get_prompt_color(void) {
    static char* prompt;
    read_constant_from_forth(state_copy, "isef_prompt_color" ,&prompt);
    return prompt;
}

static char* get_code_color(void) {
    static char* prompt;
    read_constant_from_forth(state_copy, "isef_code_color" ,&prompt);
    return prompt;
}

static char* get_current_prompt(void) {
    return sef_is_compiling(state_copy) ?
        get_compiling_prompt() :
        get_prompt();
}

void config_init(forth_state_t* fs) {
    state_copy = fs;
    sef_parse_string(fs, default_config);
    get_prompt();
    config_get_prompt();
    get_compiling_prompt();
    config_get_history_size();
}

char* config_get_prompt(void) {
    static char* prompt = NULL;
    free(prompt);
    size_t needed_size = 1 + strlen(get_current_prompt()) + strlen(get_prompt_color()) + strlen(get_code_color());
    prompt = malloc(needed_size);
    sprintf(prompt, "%s%s%s", get_prompt_color(), get_current_prompt(), get_code_color());
    return prompt;
}

int config_get_history_size(void) {
    static sef_int_t history_file_size;
    read_constant_from_forth(state_copy, "isef_history_file_size", &history_file_size);
    return (int) history_file_size;
}

char* config_get_history_file(void) {
    static char* history_file;
    read_constant_from_forth(state_copy, "isef_history_file", &history_file);
    return history_file;
}

