#include <readline/readline.h>
#include <readline/history.h>
#include "dynamic_array.h"
#include "output_text.h"
#include "completion.h"
#include <SEForth.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    output_init();
    forth_state_t* fs = sef_init();
    test(fs);
    for (int i=0; i<10; i++) {
        char* line = readline("> ");
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
    sef_free(fs);
    output_deinit();
    return 0;
}

