#include "dynamic_array.h"
#include <string.h>
#include <stdlib.h>

struct dynamic_array {
    size_t size;
    size_t space;
    char* content;
};

dynamic_array_t* da_new(void) {
    dynamic_array_t* ret = malloc(sizeof(dynamic_array_t));
    ret->content = NULL;
    da_empty(ret);
    return ret;
}

void da_empty(dynamic_array_t* da) {
    da->size = 0;
    da->space = 1;
    da->content = realloc(da->content, da->space);
    memset(da->content, 0, da->space);
}

static void resize(dynamic_array_t* da) {
    da->content = realloc(da->content, da->space * 2);
    memset(da->content + da->space, 0, da->space);
    da->space *= 2;
}

void da_append_array(dynamic_array_t* da, void* data, size_t size) {
    while (da->size + size >= da->space) {
        resize(da);
    }
    memmove(da->content + da->size, data, size);
    da->size += size;
}

void da_free(dynamic_array_t* da) {
    free(da->content);
    free(da);
}

void* da_get(dynamic_array_t* da) {
    return (void*) da->content;
}

