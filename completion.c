#include "dynamic_array.h"
#include "output_text.h"
#include <SEForth.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static dynamic_array_t* list_of_words;
static forth_state_t* state_copy;

static void clear_list(void) {
    char** words = da_get(list_of_words);
    while (*words) {
        free(*words++);
    }
    da_empty(list_of_words);
}

/* ------------------------ Parsing output of "words" ----------------------- */

static void reach_after_x(char** str, char target) {
    char* _str = *str;
    while (*(_str++) != target) {
    }
    *str = _str;
}

static void reach_after_space(char** str) {
    reach_after_x(str, ' ');
}

static void reach_after_new_line(char** str) {
    reach_after_x(str, '\n');
}

static void read_words(void) {
    if (sef_is_compiling(state_copy)) {
        return;
    }
    clear_list();
    sef_parse_string(state_copy, "words ");
    char* word_list = output_get();
    // TODO: cache size to optimize
    char* word_list_copy = word_list;
    while (*word_list) {
        // Step 1: searching for the word in of itself
        reach_after_space(&word_list);
        reach_after_space(&word_list);
        char* word_start = word_list;
        reach_after_new_line(&word_list);
        char* word_end = word_list - 3;
        // Step 2: read type
        reach_after_space(&word_list);
        reach_after_space(&word_list);
        char* type_start = word_list;
        reach_after_space(&word_list);
        char* type_end = word_list - 2;
        // Step 3: reach next definition
        reach_after_new_line(&word_list);
        reach_after_new_line(&word_list);
        // Step 4 copy word and type
        size_t word_len = word_end - word_start + 1;
        size_t type_len = type_end - type_start + 1;
        char word[word_len+1];
        char type[type_len+1];
        memcpy(word, word_start, word_len);
        memcpy(type, type_start, type_len);
        word[word_len] = 0;
        type[type_len] = 0;
        // Step 5 is type is a string, this means we reach the end of words
        if (!strcmp(type, "string,")) {
            break;
        }
        // Step 6, add the word to the list
    char* word_owned = strdup(word);
    da_append(list_of_words, word_owned);

}
free(word_list_copy);
}

/* ----------------------------- Completion API ----------------------------- */

void completion_init(forth_state_t* fs) {
state_copy = fs;
list_of_words = da_new();
void* null = NULL;
da_append(list_of_words, null);
read_words();
}

char* completion_generator(const char* input, int state) {
static size_t current_index;
static char** words = NULL;

if (state == 0) {
        current_index = 0;
        read_words();
        words = da_get(list_of_words);
    }

    while (words[current_index]) {
        if (!strncmp(input, words[current_index], strlen(input))) {
            return strdup(words[current_index++]);
        } else {
            current_index++;
        }
    }

    return NULL;
}

void completion_deinit(void) {
    clear_list();
    da_free(list_of_words);
}

