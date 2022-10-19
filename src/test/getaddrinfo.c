#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

int main() {
    char *hostname = "localhost";
    int ret;

    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if((ret = getaddrinfo(hostname, NULL, &hints, &res)) != 0) {
        printf("error: %d\n", ret);
        return 1;
    }

    struct in_addr addr;
    addr.s_addr = ((struct sockaddr_in *)(res->ai_addr))->sin_addr.s_addr;

    char buf[16];
    inet_ntop(AF_INET, &addr, buf, sizeof(buf));
    printf("IP address : %s\n", buf);

    freeaddrinfo(res);

    return 0;
}
