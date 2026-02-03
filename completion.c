#include "dynamic_array.h"
#include "output_text.h"
#include <SEForth.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static dynamic_array_t* list_of_words;
static sef_forth_state_t* state_copy;

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

static void read_words(void) {
    if (sef_is_compiling(state_copy)) {
        return;
    }
    clear_list();
    sef_eval_string(state_copy, "words ");
    char* word_list = output_get();
    // TODO: cache size to optimize
    char* word_list_copy = word_list;
    while (*word_list) {
        char* word_start = word_list;
        reach_after_space(&word_list);
        char* word_end = word_list - 2;
        size_t word_len = word_end - word_start + 1;
        char word[word_len];
        memcpy(word, word_start, word_len);
        word[word_len] = 0;
        char* word_owned = strdup(word);
        da_append(list_of_words, word_owned);

    }
    free(word_list_copy);
}

/* ---------------------------- String comparison --------------------------- */

static char to_lower_case(char in) {
    if ('A' <= in && in <= 'Z') {
        return in + ('a' - 'A');
    } else {
        return in;
    }
}

static bool case_insensitive_string_match(const char* a, const char* b, size_t size_of_a) {
    for (size_t i=0; i<size_of_a; i++) {
        if (!b[i]) {
            return false;
        }
        if (to_lower_case(a[i]) != to_lower_case(b[i])) {
            return false;
        }
    }
    return true;
}

/* ----------------------------- Completion API ----------------------------- */

void completion_init(sef_forth_state_t* fs) {
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
        if (case_insensitive_string_match(input, words[current_index], strlen(input))) {
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

/* ------------------------------- Misc helper ------------------------------ */

bool is_word_defined(const char* word) {
    read_words();
    char** words = da_get(list_of_words);
    bool ret = false;
    while (*words) {
        if (!strcmp(word, *words++)) {
            ret = true;
            break;
        }
    }
    return ret;
}

