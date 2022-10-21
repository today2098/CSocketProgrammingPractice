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
    if(!(3 <= argc && argc <= 4)) {
        fprintf(stderr,
                "UDP Echo Client\n\n"
                "Usage:\n\t%s <hostname> <echo word> [port]\n\n"
                "Example:\n\t%s localhost HELLO 12345\n",
                argv[0], argv[0]);
        return 1;
    }

    const size_t BUF_SIZE = 1024;
    char *hostname = argv[1];
    char *portnum = (argc >= 4 ? argv[3] : "7");
    char *word = argv[2];
    int ret, tmp;

    if(strlen(word) > BUF_SIZE - 1) {
        fprintf(stderr, "length of word must be less than %lu\n", BUF_SIZE - 1);
        return 1;
    }

    // 名前解決を行う．
    struct addrinfo hints, *res0;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = PF_UNSPEC;  // IPv4/IPv6の両方に対応させる．
    hints.ai_socktype = SOCK_DGRAM;
    ret = getaddrinfo(hostname, portnum, &hints, &res0);
    if(ret != 0) {
        tmp = errno;
        fprintf(stderr, "%s\n", gai_strerror(ret));
        DieWithSystemMessage2("getaddrinfo()", tmp);
    }

    struct addrinfo *res;
    int sock;
    for(res = res0; res; res = res->ai_next) {
        // ソケットを作成する．
        sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if(sock == -1) continue;  // 失敗したら次を試す．

        // データを送信する．
        int n = sendto(sock, word, strlen(word), 0, res->ai_addr, res->ai_addrlen);
        if(n < 1) DieWithSystemMessage("sendto()");

        // debug.
        char buf0[64];
        GetAddressFromSockaddr_in((struct sockaddr_in *)(res->ai_addr), buf0, sizeof(buf0));
        printf("send message to %s\n", buf0);

        // データを受信する．
        char buf[BUF_SIZE];
        memset(buf, 0, sizeof(buf));
        struct sockaddr_in senderinfo;
        socklen_t addrlen = sizeof(senderinfo);
        n = recvfrom(sock, buf, sizeof(buf) - 1, 0, (struct sockaddr *)&senderinfo, &addrlen);
        if(n == -1) DieWithSystemMessage("recvfrom()");

        // debug.
        GetAddressFromSockaddr_in(&senderinfo, buf0, sizeof(buf0));
        printf("receive message from %s\n", buf0);
        printf("message: %s\n", buf);
        printf("size: %d Byte\n", n);

        // ソケットを終了する．
        close(sock);
        break;  // 送信する宛先は一つだけ．
    }

    freeaddrinfo(res0);

    return 0;
}
