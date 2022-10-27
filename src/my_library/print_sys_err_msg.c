#include "./print_sys_err_msg.h"

#include <errno.h>   // for errno.
#include <stdio.h>   // for stderr, fprintf(), fflush() and perror().
#include <stdlib.h>  // for exit().
#include <string.h>  // strerror().

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
