#ifndef CONFIG_H
#define CONFIG_H

void config_init(forth_state_t* fs);
char* config_get_prompt(void);
char* config_get_compiling_prompt(void);

#endif

