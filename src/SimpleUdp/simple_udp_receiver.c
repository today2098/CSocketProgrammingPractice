#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "../my_library/my_library.h"  // for DieWithSystemMessage().

int main() {
    int ret;

    // IPv4のUDPソケットを作成する．
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock == -1) DieWithSystemMessage("socket()");

    // ソケットを設定する．
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    addr.sin_addr.s_addr = INADDR_ANY;
    ret = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
    if(ret == -1) DieWithSystemMessage("bind()");

    // データを受信する．
    struct sockaddr_in senderinfo;
    socklen_t addrlen = sizeof(senderinfo);
    char buf[2048];
    int n = recvfrom(sock, buf, sizeof(buf) - 1, 0, (struct sockaddr *)&senderinfo, &addrlen);
    if(n == -1) DieWithSystemMessage("recvfrom()");

    // write(fileno(stdout), buf, n);
    printf("%s\n", buf);

    close(sock);

    return 0;
}
