#include "./die_with_message.h"

#include <errno.h>   // for errno.
#include <stdio.h>   // for stderr, fprintf(), perror().
#include <stdlib.h>  // for exit().
#include <string.h>  // strerror().

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
