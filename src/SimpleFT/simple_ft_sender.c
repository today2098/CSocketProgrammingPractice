#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "../my_library/my_library.h"
#include "simple_ft.h"

int main(int argc, char *argv[]) {
    if(argc != 2) {
        fprintf(stderr,
                "Simple File Transfer (Sender)\n\n"
                "Usage:  %s <filename>\n",
                argv[0]);
        return 1;
    }

    int fd;
    int sock;
    char buf[65536];
    int n;
    int ret, tmp;

    // 送信するファイルを開く．
    fd = open(argv[1], O_RDONLY);
    if(fd == -1) DieWithSystemMessage("open()");

    // ソケットを作成する．
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1) DieWithSystemMessage("socket()");

    // 接続先指定用構造体の準備．
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    ret = inet_pton(AF_INET, "127.0.0.1", &server.sin_addr.s_addr);
    if(ret <= 0) DieWithSystemMessage("inet_pton()");

    // サーバに接続する．
    ret = connect(sock, (struct sockaddr *)&server, sizeof(server));
    if(ret == -1) DieWithSystemMessage("connect()");

    // debug.
    char buf0[MY_ADDRSTRLEN];
    GetAddressFromSockaddr_in(&server, buf0, sizeof(buf0));
    printf("connect to %s\n", buf0);

    // データを送信する．
    int cnt = 1;
    memset(buf, 0, sizeof(buf));
    while((n = read(fd, buf, sizeof(buf))) > 0) {
        ret = write(sock, buf, n);
        if(ret < 1) DieWithSystemMessage("write()");
        printf("[%d] send %d Byte\n", cnt++, n);
        fflush(stdout);
    }
    if(n == -1) DieWithSystemMessage("read()");
    printf("transmission complete\n");
    fflush(stdout);

    // TCPセッションを終了する．
    close(sock);

    return 0;
}
