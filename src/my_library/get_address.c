#include "get_address.h"

#include <arpa/inet.h>
#include <assert.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>

const unsigned MY_ADDRSTRLEN = 64;

void GetAddressFromSockaddr_in(const struct sockaddr_in *src, char *dst, size_t size) {
    assert(size >= MY_ADDRSTRLEN);

    char buf[MY_ADDRSTRLEN];
    memset(buf, 0, sizeof(buf));
    inet_ntop(src->sin_family, &src->sin_addr, buf, sizeof(buf));

    memset(dst, 0, size);
    if(src->sin_family == AF_INET6) snprintf(dst, size, "[%s]:%d", buf, ntohs(src->sin_port));
    else snprintf(dst, size, "%s:%d", buf, ntohs(src->sin_port));
}
