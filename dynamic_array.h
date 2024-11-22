#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stddef.h>

typedef struct dynamic_array dynamic_array_t;

dynamic_array_t* da_new(void);
void da_empty(dynamic_array_t* da);
void da_append_array(dynamic_array_t* da, void* data, size_t size);
void da_free(dynamic_array_t* da);
void* da_get(dynamic_array_t* da);

#define da_append(da, element) \
    da_append_array(da, &element, sizeof(element));

#endif

