#include <arpa/inet.h>
#include <assert.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../my_library/my_library.h"

int main(int argc, char *argv[]) {
    if(!(2 <= argc && argc <= 3)) {
        fprintf(stderr,
                "Multicast File Transfer (Receiver)\n\n"
                "Usage:\n\t%s <mcast group ipaddr> [nic ipaddr]\n\n"
                "Example:\n\t%s 239.192.1.2 192.168.10.106\n",
                argv[0], argv[0]);
        return 1;
    }

    const char *OUTPUT_DIR = "output/";
    char buf0[64], buf[65536];
    int n;
    int ret;

    // ソケットを作成する．
    int sock = socket(PF_INET, SOCK_DGRAM, 0);
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
    if(argc <= 2) {
        mreq.imr_interface.s_addr = INADDR_ANY;  // 任意のインターフェイスからマルチキャストを受け入れる．
    } else {
        ret = inet_pton(AF_INET, argv[2], &mreq.imr_interface.s_addr);  // 受信インターフェイスのIPアドレスを指定する．
        if(ret <= 0) DieWithSystemMessage("inet_pton()");
    }
    ret = inet_pton(AF_INET, argv[1], &mreq.imr_multiaddr.s_addr);
    if(ret <= 0) DieWithSystemMessage("inet_pton()");

    // マルチキャストグループにジョインする．
    ret = setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mreq, sizeof(mreq));
    if(ret == -1) DieWithSystemMessage("setsockopt()");

    // ファイル名を受信する．
    memset(buf, 0, sizeof(buf));
    struct sockaddr_in senderinfo;
    socklen_t addrlen = sizeof(senderinfo);
    n = recvfrom(sock, buf, sizeof(buf) - 1, 0, (struct sockaddr *)&senderinfo, &addrlen);
    if(n == -1) DieWithSystemMessage("recvfrom()");
    assert(strlen(buf) <= 256);

    // debug.
    GetAddressFromSockaddr_in(&senderinfo, buf0, sizeof(buf0));
    printf("receive from %s\n", buf0);
    printf("file name: %s\n", buf);
    printf("size: %d Byte\n", n);

    // 受信したファイル名でファイルを作成する．
    char filepath[256];
    memset(filepath, 0, sizeof(filepath));
    snprintf(filepath, sizeof(filepath), "%s%s", OUTPUT_DIR, buf);
    MakeDirectory(filepath);
    int fd = open(filepath, O_WRONLY | O_CREAT, 0600);
    if(fd == -1) DieWithSystemMessage("open()");

    // ファイルデータを受信する．
    memset(buf, 0, sizeof(buf));
    int cnt = 1;
    while((n = recv(sock, buf, sizeof(buf), 0)) > 0) {
        ret = write(fd, buf, n);
        if(ret < 1) DieWithSystemMessage("write()");
        printf("[%d] receive %d Byte\n", cnt, n);
        cnt++;
    }
    if(n == -1) DieWithSystemMessage("recv()");
    if(n == 0) printf("receive zero packet\n");

    close(sock);

    return 0;
}
