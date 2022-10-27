#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../my_library/my_library.h"

int main(int argc, char *argv[]) {
    if(argc != 4) {
        fprintf(stderr,
                "Simple FT Sender whit Name Resolution\n\n"
                "Usage:  %s <hostname> <port> <filename>\n",
                argv[0]);
        return 1;
    }

    char *hostname = argv[1];
    char *portnum = argv[2];
    char *filename = argv[3];
    int fd;
    int sock;
    char buf0[64], buf[65536];
    int n;
    int ret, tmp;

    // 名前解決を行う．
    struct addrinfo hints, *res0;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;  // IPv4, IPv6の両方に対応させる．
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

        // 送信するファイルを開く．
        fd = open(filename, O_RDONLY);
        if(fd == -1) DieWithSystemMessage("open()");

        // ソケットを作成する．
        sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if(sock == -1) {  // 失敗したら次を試す．
            printf("failed to create socket\n");
            fflush(stdout);
            close(fd);
            continue;
        }

        // サーバに接続する．
        ret = connect(sock, res->ai_addr, res->ai_addrlen);
        if(ret == -1) {
            printf("connection failed\n");
            fflush(stdout);
            close(fd);
            close(sock);
            continue;
        }
        printf("connection successful\n");
        fflush(stdout);

        // データを送信する．
        int cnt = 1;
        memset(buf, 0, sizeof(buf));
        while((n = read(fd, buf, sizeof(buf))) > 0) {
            ret = write(sock, buf, n);
            if(ret < 1) {
                tmp = errno;
                close(sock);
                DieWithSystemMessage2("write()", tmp);
            }
            printf("[%d] send %d Byte\n", cnt++, n);
            fflush(stdout);
        }
        if(n == -1) {
            tmp = errno;
            close(sock);
            DieWithSystemMessage2("read()", tmp);
        }
        printf("transmission complete\n");

        // ファイルを閉じる．
        close(fd);

        // TCPセッションを終了する．
        close(sock);

        break;
    }

    freeaddrinfo(res0);

    return 0;
}
