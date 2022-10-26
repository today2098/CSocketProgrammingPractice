#include "./get_address.h"

#include <arpa/inet.h>   // for inet_ntop().
#include <assert.h>      // for assert().
#include <netinet/in.h>  // for sockaddr_in and ntohs().
#include <stdio.h>       // snprintf().
#include <string.h>      // for memset().

void GetAddressFromSockaddr_in(struct sockaddr_in *addr, char *buf, size_t len) {
    assert(len >= 64);
    char buf2[64];
    memset(buf2, 0, sizeof(buf2));
    inet_ntop(addr->sin_family, &(addr->sin_addr), buf2, sizeof(buf2));
    memset(buf, 0, len);
    snprintf(buf, len, "%s:%d", buf2, ntohs(addr->sin_port));
}
