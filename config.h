#ifndef CONFIG_H
#define CONFIG_H

#include <SEForth.h>

void config_init(forth_state_t* fs);
char* config_get_prompt(void);
int config_get_history_size(void);

#endif

