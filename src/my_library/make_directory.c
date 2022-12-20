#include "make_directory.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

void MakeDirectory(const char *dirpath) {
    assert(strlen(dirpath) <= 256);

    int i;
    int len = strlen(dirpath);
    char tmp[256];

    memset(tmp, 0, sizeof(tmp));
    snprintf(tmp, sizeof(tmp), "%s", dirpath);
    for(i = len - 1; i >= 0; --i) {
        if(tmp[i] == '/') break;
        tmp[i] = 0;
    }

    for(i = 0; i < (int)len && tmp[i]; ++i) {
        if(tmp[i] == '/') {
            tmp[i] = 0;
            mkdir(tmp, 0755);
            tmp[i] = '/';
        }
    }
}
