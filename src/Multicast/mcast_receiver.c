#include <errno.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "../my_library/my_library.h"

int main() {
    int ret, tmp;

    // ソケットを作成する．
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = PF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    // hints.ai_flagsにAI_PASSIVEフラグが指定され，かつnodeがNULLの場合，返されるソケットアドレスにはワイルドガードアドレスが入る．
    hints.ai_flags = AI_PASSIVE;
    ret = getaddrinfo(NULL, "12345", &hints, &res);
    if(ret != 0) {
        tmp = errno;
        fprintf(stderr, "%s\n", gai_strerror(ret));
        DieWithSystemMessage2("getaddrinfo()", tmp);
    }

    int sock = socket(res->ai_family, res->ai_socktype, 0);
    if(sock == -1) DieWithSystemMessage("socket()");
    ret == bind(sock, res->ai_addr, res->ai_addrlen);
    if(ret == -1) DieWithSystemMessage("bind()");
    freeaddrinfo(res);

    // マルチキャストグループを設定する．
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = PF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    ret = getaddrinfo("239.118.155.1", NULL, &hints, &res);
    if(ret != 0) {
        tmp = errno;
        fprintf(stderr, "%s\n", gai_strerror(ret));
        DieWithSystemMessage2("getaddrinfo()", tmp);
    }

    struct group_req greq;
    memset(&greq, 0, sizeof(greq));
    greq.gr_interface = 0;  // 任意のネットワークインターフェースを利用する．
    // Mac OSの場合は，ipv4アドレスが割り当てられているインターフェス（例えばwifiで，そのインターフェース名がen0の場合）以下の様にネットワークインターフェースを指定する．
    // greq.gr_interface = if_nametoindex("en0");
    memcpy(&greq.gr_group, res->ai_addr, res->ai_addrlen);  // getaddrinfo()の結果をgroup_req構造体へコピーする．
    freeaddrinfo(res);

    // MCAST_JOIN_GROUPを利用してマルチキャストグループへジョインする．
    ret = setsockopt(sock, IPPROTO_IP, MCAST_JOIN_GROUP, (char *)&greq, sizeof(greq));
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
