#include "dynamic_array.h"
#include <stdio.h>
#include <string.h>
#include <SEForth.h>

static dynamic_array_t* txt_output_da;

void sef_output(char c) {
    da_append(txt_output_da, c);
}

void output_init(void) {
    txt_output_da = da_new();
}

void output_display(void) {
    char* all_output = da_get(txt_output_da);
    bool end_on_new_line = strlen(all_output) != 0 && all_output[strlen(all_output)-1] == '\n';
    const char* end = end_on_new_line ? "" : "\n";
    printf("%s%s", all_output, end);
    da_empty(txt_output_da);
}

void output_deinit(void) {
    da_free(txt_output_da);
}

