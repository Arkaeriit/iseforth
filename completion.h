#ifndef COMPLETION_H
#define COMPLETION_H

#include <SEForth.h>

void test(forth_state_t* fs);
void completion_init(forth_state_t* fs);
char* completion_generator(const char* input, int state);
void completion_deinit(void);

#endif

