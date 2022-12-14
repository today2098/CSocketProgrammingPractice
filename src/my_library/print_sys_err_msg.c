#include "print_sys_err_msg.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void PrintSystemErrorMessage(const char *msg) {
    perror(msg);
    fprintf(stderr, "errno: %d\n", errno);
    fflush(stderr);
}

void PrintSystemErrorMessage2(const char *msg, int no) {
    fprintf(stderr, "%s: %s\n", msg, strerror(no));
    fprintf(stderr, "errno: %d\n", no);
    fflush(stderr);
}

void DieWithSystemMessage(const char *msg) {
    perror(msg);
    fprintf(stderr, "errno: %d\n", errno);
    fflush(stderr);
    exit(1);
}

void DieWithSystemMessage2(const char *msg, int no) {
    fprintf(stderr, "%s: %s\n", msg, strerror(no));
    fprintf(stderr, "errno: %d\n", no);
    fflush(stderr);
    exit(1);
}
