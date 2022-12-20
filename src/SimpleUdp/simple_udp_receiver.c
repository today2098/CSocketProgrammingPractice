#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "../my_library/my_library.h"

int main() {
    int sock;
    struct sockaddr_in addr;
    struct sockaddr_in senderinfo;
    socklen_t addrlen = sizeof(senderinfo);
    char buf[2048];
    int n;
    int ret;

    // IPv4のUDPソケットを作成する．
    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock == -1) DieWithSystemMessage("socket()");

    // ソケットを設定する．
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    addr.sin_addr.s_addr = INADDR_ANY;
    ret = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
    if(ret == -1) DieWithSystemMessage("bind()");

    // メッセージを受信する．
    n = recvfrom(sock, buf, sizeof(buf) - 1, 0, (struct sockaddr *)&senderinfo, &addrlen);
    if(n == -1) DieWithSystemMessage("recvfrom()");

    // debug.
    char buf0[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &senderinfo.sin_addr, buf0, sizeof(buf0));
    printf("receive message from %s:%d\n", buf0, ntohs(senderinfo.sin_port));
    // write(fileno(stdout), buf, n);
    printf("message: %s\n", buf);
    printf("size: %d Byte\n", n);
    fflush(stdout);

    close(sock);

    return 0;
}
