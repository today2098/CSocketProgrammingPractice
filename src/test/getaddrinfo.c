#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    if(argc != 2) {
        fprintf(stderr,
                "Name Resolution\n\n"
                "Usage:\n\t%s <hostname>\n\n"
                "Example:\n\t%s localhost\n\t%s google.com\n\t%s sony.jp\n",
                argv[0], argv[0], argv[0], argv[0]);
        return 1;
    }

    char *hostname = argv[1];
    struct addrinfo hints, *res0, *res;
    struct in_addr addr;
    char buf[16];
    int ret;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    ret = getaddrinfo(hostname, NULL, &hints, &res0);
    if(ret != 0) {
        printf("error: %s\n", gai_strerror(ret));
        return 1;
    }

    for(res = res0; res; res = res->ai_next) {
        addr.s_addr = ((struct sockaddr_in *)(res->ai_addr))->sin_addr.s_addr;
        inet_ntop(AF_INET, &addr, buf, sizeof(buf));
        printf("IP address: %s\n", buf);
    }

    freeaddrinfo(res0);

    return 0;
}
