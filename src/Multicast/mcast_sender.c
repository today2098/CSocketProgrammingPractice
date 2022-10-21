#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "../my_library/my_library.h"

int main() {
    int sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock == -1) DieWithSystemMessage("socket()");

    // 宛先の情報を指定する．
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    inet_pton(AF_INET, "239.118.155.1", &addr.sin_addr.s_addr);

    // データを送信する．
    int n = sendto(sock, "HELLO", 5, 0, (struct sockaddr *)&addr, sizeof(addr));
    if(n < 1) DieWithSystemMessage("sendto()");

    // debug.
    char buf[64];
    GetAddressFromSockaddr_in(&addr, buf, sizeof(buf));
    printf("send to %s\n", buf);

    close(sock);

    return 0;
}
