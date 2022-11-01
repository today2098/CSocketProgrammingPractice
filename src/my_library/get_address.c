#include "get_address.h"

#include <arpa/inet.h>
#include <assert.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>

const unsigned MY_ADDRSTRLEN = 64;

void GetAddressFromSockaddr_in(struct sockaddr_in *addr, char *buf, size_t len) {
    assert(len >= MY_ADDRSTRLEN);

    char buf2[MY_ADDRSTRLEN];
    memset(buf2, 0, sizeof(buf2));
    inet_ntop(addr->sin_family, &(addr->sin_addr), buf2, sizeof(buf2));

    memset(buf, 0, len);
    if(addr->sin_family == AF_INET6) snprintf(buf, len, "[%s]:%d", buf2, ntohs(addr->sin_port));
    else snprintf(buf, len, "%s:%d", buf2, ntohs(addr->sin_port));
}
