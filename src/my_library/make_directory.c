#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "./my_library.h"

void MakeDirectory(const char *dirpath) {
    int i;

    char tmp[256];
    memset(tmp, 0, sizeof(tmp));
    snprintf(tmp, sizeof(tmp), "%s", dirpath);
    size_t len = strlen(dirpath);
    for(i = len - 1; i >= 0; --i) {
        if(tmp[i] == '/') break;
        tmp[i] = 0;
    }
    for(i = 0; i < (int)len && tmp[i]; ++i) {
        if(tmp[i] == '/') {
            tmp[i] = 0;
            mkdir(tmp, 0777);
            tmp[i] = '/';
        }
    }
}
