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
    int sock0, sock;
    struct sockaddr_in addr, client;
    char buf[INET_ADDRSTRLEN];
    int ret, tmp;

    // Listen用のソケットを作成する．
    sock0 = socket(PF_INET, SOCK_STREAM, 0);
    if(sock0 == -1) DieWithSystemMessage("socket()");

    // Listen用ソケットの設定を行う．
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    addr.sin_addr.s_addr = INADDR_ANY;  // ワイルドカード．
    // ローカルIPアドレスとローカルポートをソケットに割り当てる．
    ret = bind(sock0, (struct sockaddr *)&addr, sizeof(addr));
    if(ret == -1) DieWithSystemMessage("bind()");

    // TCPクライアントからの接続要求を待ち受けている状態にする．
    ret = listen(sock0, 5);
    if(ret == -1) DieWithSystemMessage("listen()");

    printf("listen\n");
    fflush(stdout);

    // TCPクライアントからの接続要求を受け付ける．
    socklen_t len = sizeof(client);
    sock = accept(sock0, (struct sockaddr *)&client, &len);
    if(sock == -1) DieWithSystemMessage("accept()");

    // debug.
    memset(buf, 0, sizeof(buf));
    inet_ntop(AF_INET, &client.sin_addr, buf, sizeof(buf));
    if(buf == NULL) {
        tmp = errno;
        close(sock);
        DieWithSystemMessage2("inet_ntop()", tmp);
    }
    printf("accept from %s:%u\n", buf, ntohs(client.sin_port));
    fflush(stdout);

    // メッセージを送信する．
    ret = write(sock, "HELLO", 5);
    if(ret == -1) {
        tmp = errno;
        close(sock);
        DieWithSystemMessage2("write()", tmp);
    }

    printf("send mssage\n");
    fflush(stdout);

    // TCPセッションを終了する．
    close(sock);

    // Listen用ソケットを終了する．
    close(sock0);

    return 0;
}
