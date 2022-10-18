#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "../my_library/my_library.h"  // for DieWithSystemMessage().

int main() {
    // ソケットを作成．
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1) DieWithSystemMessage("socket()");

    // 接続先指定用アドレス構造体を用意．
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(12345);
    // アドレスをテキスト形式からバイナリ形式に変換する．127.0.0.1はlocalhost.
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr.s_addr);

    // サーバに接続．
    int ret = connect(sock, (struct sockaddr *)&server, sizeof(server));
    if(ret == -1) DieWithSystemMessage("connect()");
    
    printf("connect 127.0.0.1:12345\n");

    // サーバからデータを受信．
    char buf[32];
    memset(buf, 0, sizeof(buf));
    int n = read(sock, buf, sizeof(buf));
    if(n == -1) DieWithSystemMessage("read()");

    printf("receive masage\n");
    printf("content: %s\nsize: %d Byte\n", buf, n);

    // ソケットを終了．
    close(sock);

    return 0;
}
