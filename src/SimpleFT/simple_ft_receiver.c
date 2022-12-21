#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../my_library/my_library.h"
#include "simple_ft.h"

int main(int argc, char *argv[]) {
    if(argc != 2) {
        fprintf(stderr,
                "Simple File Transfer (Receiver)\n\n"
                "Usage:  %s <outputfilename>\n",
                argv[0]);
        return 1;
    }

    const char OUTPUT_DIR[] = "output/";
    int sock0, sock;
    int fd;
    char buf[65536];
    int n, ret;

    // Listen用のソケットを作成する．
    sock0 = socket(PF_INET, SOCK_STREAM, 0);
    if(sock0 == -1) DieWithSystemMessage("socket()");

    // ソケットの設定を行う．
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;
    ret = bind(sock0, (struct sockaddr *)&addr, sizeof(addr));
    if(ret == -1) DieWithSystemMessage("bind()");

    // TCPクライアントからの接続要求を待ち受けている状態にする．
    ret = listen(sock0, 5);
    if(ret == -1) DieWithSystemMessage("listen()");

    // TCPクライアントからの接続要求を受け付ける．
    struct sockaddr_in client;
    memset(&client, 0, sizeof(client));
    socklen_t len = sizeof(client);
    sock = accept(sock0, (struct sockaddr *)&client, &len);
    if(sock == -1) DieWithSystemMessage("accept()");

    // debug.
    char buf0[MY_ADDRSTRLEN];
    GetAddressFromSockaddr_in(&client, buf0, sizeof(buf0));
    printf("accept from %s\n", buf0);

    // 空ファイルを作成する．
    char filepath[256];
    memset(filepath, 0, sizeof(filepath));
    snprintf(filepath, sizeof(filepath), "%s%s", OUTPUT_DIR, argv[1]);
    MakeDirectory(filepath);
    fd = open(filepath, O_WRONLY | O_CREAT, 0600);
    if(fd == -1) DieWithSystemMessage("open()");

    // データを受信する．
    int cnt = 1;
    memset(buf, 0, sizeof(buf));
    while((n = read(sock, buf, sizeof(buf))) > 0) {
        ret = write(fd, buf, n);
        if(ret < 1) DieWithSystemMessage("write()");
        printf("[%d] receive %d Byte\n", cnt++, n);
    }
    if(n == -1) DieWithSystemMessage("read()");

    // TCPセッションを終了する．
    close(sock);

    // Listen用socketを終了する．
    close(sock0);

    return 0;
}
