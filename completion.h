#ifndef COMPLETION_H
#define COMPLETION_H

#include <SEForth.h>
#include <stdbool.h>

void completion_init(forth_state_t* fs);
char* completion_generator(const char* input, int state);
void completion_deinit(void);
bool is_word_defined(const char* word);

#endif

