#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "../my_library/my_library.h"

int main() {
    int ret;

    // IPv4のUDPソケットを作成する．
    int sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock == -1) DieWithSystemMessage("socket()");

    // ソケットを設定する．
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    addr.sin_addr.s_addr = INADDR_ANY;
    ret = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
    if(ret == -1) DieWithSystemMessage("bind()");

    while(1) {
        // データを受信する．
        char buf[1024];
        memset(buf, 0, sizeof(buf));
        struct sockaddr_in senderinfo;
        socklen_t addrlen = sizeof(senderinfo);
        int n = recvfrom(sock, buf, sizeof(buf) - 1, 0, (struct sockaddr *)&senderinfo, &addrlen);
        if(n == -1) DieWithSystemMessage("recvfrom()");

        // debug.
        char buf0[64];
        GetAddressFromSockaddr_in(&senderinfo, buf0, sizeof(buf0));
        printf("receive form %s\n", buf0);
        printf("message: %s\n", buf);
        printf("size: %d Byte\n", n);

        // データを返送する．
        sendto(sock, buf, n, 0, (struct sockaddr *)&senderinfo, addrlen);
        printf("return message\n");
    }

    close(sock);

    return 0;
}
