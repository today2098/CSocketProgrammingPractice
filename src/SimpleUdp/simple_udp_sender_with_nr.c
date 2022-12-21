#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "../my_library/my_library.h"

int main(int argc, char *argv[]) {
    if(argc != 3) {
        fprintf(stderr,
                "Simple UDP Sender whit Name Resolution\n\n"
                "Usage:\n\t%s <hostname> <port>\n\n"
                "Example:\n\t%s localhost 12345\n",
                argv[0], argv[0]);
        return 1;
    }

    char *hostname = argv[1];
    char *portnum = argv[2];
    struct addrinfo hints, *res0, *res;
    int sock;
    int n;
    int ret, tmp;

    // 名前解決を行う．
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = PF_UNSPEC;  // IPv4/IPv6の両方に対応させる．
    hints.ai_socktype = SOCK_DGRAM;
    ret = getaddrinfo(hostname, portnum, &hints, &res0);
    if(ret != 0) {
        tmp = errno;
        fprintf(stderr, "%s\n", gai_strerror(ret));
        DieWithSystemMessage2("getaddrinfo()", tmp);
    }

    for(res = res0; res; res = res->ai_next) {
        // ソケットを作成する．
        sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if(sock == -1) continue;  // 失敗したら次を試す．

        // メッセージを送信する．
        n = sendto(sock, "HELLO", 5, 0, res->ai_addr, res->ai_addrlen);
        if(n < 1) DieWithSystemMessage("sendto()");

        // debug.
        char buf[MY_ADDRSTRLEN];
        GetAddressFromSockaddr_in((struct sockaddr_in *)res->ai_addr, buf, sizeof(buf));
        printf("send message to %s\n", buf);
        fflush(stdout);

        close(sock);
        break;
    }

    freeaddrinfo(res0);

    return 0;
}
