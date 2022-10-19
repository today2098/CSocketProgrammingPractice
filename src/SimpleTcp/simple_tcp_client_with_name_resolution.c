#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "../my_library/my_library.h"  // for DieWithSystemMessage().

int main(int argc, char *argv[]) {
    if(argc != 3) {
        fprintf(stderr,
                "Simple TCP Client whit Name Resolution\n\n"
                "Usage:\n\t%s <hostname> <port>\n\n"
                "Example:\n\t%s localhost 12345\n",
                argv[0], argv[0]);
        return 1;
    }

    char *hostname = argv[1];
    char *portnum = argv[2];
    int ret;

    // 名前解決を行う．
    struct addrinfo hints, *res0;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    ret = getaddrinfo(hostname, portnum, &hints, &res0);
    if(ret != 0) {
        printf("error: %s\n", gai_strerror(ret));
        return 1;
    }

    // コネクションに成功までリンクリストres0を走査する．
    struct addrinfo *res;
    int sock;
    for(res = res0; res; res = res->ai_next) {
        // ソケットを作成．
        sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if(sock == -1) continue;  // 失敗したら次を試す．

        // サーバに接続．
        ret = connect(sock, res->ai_addr, res->ai_addrlen);
        if(ret == -1) {
            close(sock);
            sock = -1;
            continue;
        }

        struct in_addr addr;
        addr.s_addr = ((struct sockaddr_in *)(res->ai_addr))->sin_addr.s_addr;
        char buf[16];
        inet_ntop(AF_INET, &addr, buf, sizeof(buf));
        int port = ntohs(((struct sockaddr_in *)(res->ai_addr))->sin_port);

        printf("connect %s:%d\n", buf, port);
        break;  // 接続に成功しているので，ループを終了．
    }

    freeaddrinfo(res0);

    if(sock < 0) {
        fprintf(stderr, "connection failed\n");
        return 1;
    }

    // サーバからデータを受信．
    char buf[32];
    memset(buf, 0, sizeof(buf));
    int n = read(sock, buf, sizeof(buf));
    if(n == -1) DieWithSystemMessage("read()");

    printf("receive message\n");
    printf("content: %s\nsize: %d Byte\n", buf, n);

    // ソケットを終了．
    close(sock);

    return 0;
}
