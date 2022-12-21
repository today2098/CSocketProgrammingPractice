#include "get_address.h"

#include <arpa/inet.h>
#include <assert.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>

#include "print_sys_err_msg.h"

const size_t MY_ADDRSTRLEN = 64;

int GetAddressFromSockaddr_in(const struct sockaddr_in *src, char *dst, size_t size) {
    assert(size >= MY_ADDRSTRLEN);
    int res;

    char buf[INET6_ADDRSTRLEN];
    memset(buf, 0, sizeof(buf));
    inet_ntop(src->sin_family, &src->sin_addr, buf, sizeof(buf));
    if(buf == NULL) {
        PrintSystemErrorMessage("inet_ntop()");
        return -1;
    }

    memset(dst, 0, size);
    if(src->sin_family == AF_INET) snprintf(dst, size, "%s:%d", buf, ntohs(src->sin_port));
    else if(src->sin_family == AF_INET6) snprintf(dst, size, "[%s]:%d", buf, ntohs(src->sin_port));
    else fprintf(stderr, "adress family is unsupported");
    return 0;
}
