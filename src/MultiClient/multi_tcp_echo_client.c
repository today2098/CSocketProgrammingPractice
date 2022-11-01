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
                "Simple TCP Client whit Name Resolution\n\n"
                "Usage:\n\t%s <hostname> <port>\n\n"
                "Example:\n\t%s localhost 12345\n",
                argv[0], argv[0]);
        return 1;
    }

    char *hostname = argv[1];
    char *portnum = argv[2];
    int sock;
    char buf0[64], buf[1024];
    int n;
    int ret, tmp;

    // 名前解決を行う．
    struct addrinfo hints, *res0;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    ret = getaddrinfo(hostname, portnum, &hints, &res0);
    if(ret != 0) {
        tmp = errno;
        fprintf(stderr, "%s\n", gai_strerror(ret));
        DieWithSystemMessage2("getaddrinfo()", tmp);
    }

    // コネクションに成功するまで，リンクリストを走査する．
    struct addrinfo *res;
    for(res = res0; res; res = res->ai_next) {
        // debug.
        GetAddressFromSockaddr_in((struct sockaddr_in *)res->ai_addr, buf0, sizeof(buf0));
        printf("ip address of server: %s\n", buf0);
        fflush(stdout);

        // ソケットを作成する．
        sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if(sock == -1) {
            printf("failed to create socket\n");
            fflush(stdout);
            continue;
        }

        // サーバに接続する．
        ret = connect(sock, res->ai_addr, res->ai_addrlen);
        if(ret == -1) {
            printf("connection failed\n");
            fflush(stdout);
            close(sock);
            sock = -1;
            continue;
        }

        // debug.
        printf("connection successful\n");
        fflush(stdout);

        // メッセージを入力する．
        memset(buf, 0, sizeof(buf));
        scanf("%s", buf);

        // メッセージを送信する．
        ret = write(sock, buf, sizeof(buf));
        if(ret == -1) {
            PrintSystemErrorMessage("write()");
            close(sock);
            freeaddrinfo(res0);
            return -1;
        }

        // debug.
        printf("send data\n");
        printf("data: %s\n", buf);
        fflush(stdout);

        // サーバーから返送されるメッセージを受信する．
        memset(buf, 0, sizeof(buf));
        n = read(sock, buf, sizeof(buf));
        if(n < 0) {
            PrintSystemErrorMessage("read()");
            close(sock);
            freeaddrinfo(res0);
            return -1;
        }

        // debug.
        printf("receive data\n");
        printf("data: %s\n", buf);
        fflush(stdout);

        // TCPセッションを終了する．
        close(sock);

        break;
    }

    freeaddrinfo(res0);

    return 0;
}
