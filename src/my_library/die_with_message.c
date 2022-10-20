#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./my_library.h"

void DieWithSystemMessage(const char *msg) {
    perror(msg);
    fprintf(stderr, "errno: %d\n", errno);
    exit(1);
}

void DieWithSystemMessage2(const char *msg, int no) {
    fprintf(stderr, "%s: %s\n", msg, strerror(no));
    fprintf(stderr, "errno: %d\n", no);
    exit(1);
}
