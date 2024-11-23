#include <SEForth.h>
#include "string.h"
#include "config.h"

static forth_state_t* state_copy;

void config_init(forth_state_t* fs) {
    state_copy = fs;
    config_get_prompt();
}

static void read_constant_from_forth(forth_state_t* fs, const char* constant_name, void* dest) {
    if (sef_is_compiling(fs)) {
        return;
    }
    sef_parse_string(fs, constant_name);
    sef_parse_char(fs, ' ');
    sef_int_t value = sef_pop_data(fs);
    memcpy(dest, &value, sizeof(sef_int_t));
}

char* config_get_prompt(void) {
    static char* prompt;
    read_constant_from_forth(state_copy, "isef_prompt" ,&prompt);
    return prompt;
}
