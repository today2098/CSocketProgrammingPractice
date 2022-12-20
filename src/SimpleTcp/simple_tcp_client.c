#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "../my_library/my_library.h"

int main() {
    int sock;
    struct sockaddr_in server;
    char buf[32];
    int n;
    int ret, tmp;

    // ソケットを作成する．
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1) DieWithSystemMessage("socket()");

    // 接続先指定用のアドレス構造体を用意する．
    server.sin_family = AF_INET;     // アドレスファミリ．
    server.sin_port = htons(12345);  // ポート番号．
    // アドレスをテキスト形式からバイナリ形式に変換する．127.0.0.1はlocalhostを指す.
    ret = inet_pton(AF_INET, "127.0.0.1", &server.sin_addr.s_addr);  // IPアドレス．
    if(ret != 1) {
        tmp = errno;
        if(ret == 0) fprintf(stderr, "uncorrect notation\n");  // アドレス表記が不適の場合．
        DieWithSystemMessage2("inet_pton", tmp);
    }

    // サーバに接続する．
    ret = connect(sock, (struct sockaddr *)&server, sizeof(server));
    if(ret == -1) DieWithSystemMessage("connect()");

    printf("connect to server\n");
    fflush(stdout);

    // サーバからメッセージを受信する．
    memset(buf, 0, sizeof(buf));
    n = read(sock, buf, sizeof(buf));
    if(n == -1) {
        tmp = errno;
        close(sock);
        DieWithSystemMessage2("read()", tmp);
    }

    printf("receive massage\n");
    printf("msg: %s\nsize: %d Byte\n", buf, n);
    fflush(stdout);

    // ソケットを終了する．
    close(sock);

    return 0;
}
