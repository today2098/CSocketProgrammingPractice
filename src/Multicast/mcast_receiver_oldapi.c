#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "../my_library/my_library.h"

int main() {
    int ret;

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock == -1) DieWithSystemMessage("socket()");

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    addr.sin_addr.s_addr = INADDR_ANY;
    ret = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
    if(ret == -1) DieWithSystemMessage("bind()");

    // マルチキャストグループを設定する．
    struct ip_mreq mreq;
    memset(&mreq, 0, sizeof(mreq));
    mreq.imr_interface.s_addr = INADDR_ANY;  // 任意のインターフェイスからマルチキャストを受け入れる．
    inet_pton(AF_INET, "239.118.155.1", &mreq.imr_multiaddr.s_addr);

    // マルチキャストグループにジョインする．
    // 旧APIであるIP_ADD_MEMBERSHIPはip_mreq構造体を利用し，
    // IPV6_ADD_MEMBERSHIPはipv6_mreq構造体を利用するため，プロトコル依存である．
    ret = setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mreq, sizeof(mreq));
    if(ret == -1) DieWithSystemMessage("setsockopt()");

    // データを受信する．
    char buf[2048];
    memset(buf, 0, sizeof(buf));
    struct sockaddr_in senderinfo;
    socklen_t addrlen = sizeof(senderinfo);
    // recv(sock, buf, sizeof(buf), 0);
    int n = recvfrom(sock, buf, sizeof(buf) - 1, 0, (struct sockaddr *)&senderinfo, &addrlen);
    if(n == -1) DieWithSystemMessage("recvfrom()");

    // debug.
    char buf0[64];
    GetAddressFromSockaddr_in(&senderinfo, buf0, sizeof(buf0));
    printf("receive form %s\n", buf0);
    printf("message: %s\n", buf);
    printf("size: %d Byte\n", n);

    close(sock);

    return 0;
}
