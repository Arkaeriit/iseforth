#include "dynamic_array.h"
#include <SEForth.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static dynamic_array_t* txt_output_da;

void sef_output(char c) {
    da_append(txt_output_da, c);
}

void output_init(void) {
    txt_output_da = da_new();
}

char* output_get(void) {
    char* ret = strdup(da_get(txt_output_da));
    da_empty(txt_output_da);
    return ret;
}

void output_display(void) {
    char* all_output = output_get();
    bool end_on_new_line = strlen(all_output) != 0 && all_output[strlen(all_output)-1] == '\n';
    const char* end = end_on_new_line ? "" : "\n";
    printf("%s%s", all_output, end);
    free(all_output);
}

void output_deinit(void) {
    da_free(txt_output_da);
}

