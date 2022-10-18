#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "../my_library/my_library.h"  // for DieWithSystemMessage().

int main() {
    int ret;

    // Listen用のソケットを作成．
    int sock0 = socket(PF_INET, SOCK_STREAM, 0);
    if(sock0 == -1) DieWithSystemMessage("socket()");

    // Listen用ソケットの設定．
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    addr.sin_addr.s_addr = INADDR_ANY;
    // ローカルIPアドレスとローカルポートをソケットに割り当てる．
    ret = bind(sock0, (struct sockaddr *)&addr, sizeof(addr));
    if(ret == -1) DieWithSystemMessage("bind()");

    // TCPクライアントからの接続要求を待ち受けている状態にする．
    ret = listen(sock0, 5);
    if(ret == -1) DieWithSystemMessage("listen()");

    printf("listen\n");

    // TCPクライアントからの接続要求を受け付ける．
    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    int sock = accept(sock0, (struct sockaddr *)&client, &len);
    if(sock == -1) DieWithSystemMessage("accept()");

    char buf[16];
    inet_ntop(AF_INET, &client.sin_addr, buf, sizeof(buf));
    printf("accept from %s:%u\n", buf, ntohs(client.sin_port));

    // 5文字送信．
    ret = write(sock, "HELLO", 5);
    if(ret == -1) DieWithSystemMessage("write()");

    printf("send mssage\n");

    // TCPセッションを終了．
    close(sock);

    // Listen用ソケットを終了．
    close(sock0);

    return 0;
}
