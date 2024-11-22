#include "dynamic_array.h"
#include <stdio.h>
#include <string.h>

int main(void) {
    dynamic_array_t* da = da_new();
    const char* str = "Hello, world!\n";
    for (size_t i=0; i<strlen(str); i++) {
        da_append(da, str[i]);
    }
    printf("%s", (char*) da_get(da));
    da_free(da);
    return 0;
}

