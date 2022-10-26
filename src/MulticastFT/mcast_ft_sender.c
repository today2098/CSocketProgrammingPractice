#include <arpa/inet.h>
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
    if(!(3 <= argc && argc <= 4)) {
        fprintf(stderr,
                "Multicast File Transfer (Sender)\n\n"
                "Usage:\n\t%s <filename> <mcast group ipaddr> [nic ipaddr]\n\n"
                "Example:\n\t%s data.jpg 239.192.1.2 192.168.10.106\n",
                argv[0], argv[0]);
        return 1;
    }

    char buf0[64], buf[65536];
    int n;
    int ret;

    // 送信ファイルを開く．
    int fd = open(argv[1], O_RDONLY);
    if(fd == -1) DieWithSystemMessage("open()");

    // ソケットの作成．
    int sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock == -1) DieWithSystemMessage("socket()");

    // 宛先の情報を指定する．
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    ret = inet_pton(AF_INET, argv[2], &addr.sin_addr.s_addr);
    if(ret <= 0) DieWithSystemMessage("inet_pton()");

    // 出力インターフェイスのIPアドレスを指定する．
    // ※コマンド"$ ip addr show"でIPアドレスを確認する．
    if(argc >= 4) {
        in_addr_t ifsipaddr;
        ret = inet_pton(AF_INET, argv[3], &ifsipaddr);
        if(ret <= 0) DieWithSystemMessage("inet_pton()");
        ret = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_IF, (void *)&ifsipaddr, sizeof(ifsipaddr));
        if(ret == -1) DieWithSystemMessage("setsockopt()");
    }

    // ファイル名を送信する．
    n = sendto(sock, argv[1], strlen(argv[1]), 0, (struct sockaddr *)&addr, sizeof(addr));
    if(n < 1) DieWithSystemMessage("sendto()");
    usleep(30000);

    // debug.
    GetAddressFromSockaddr_in(&addr, buf0, sizeof(buf0));
    printf("send to %s\n", buf0);
    printf("file name: %s\n", argv[1]);
    printf("size: %d Byte\n", n);

    // ファイルデータを送信する．
    memset(buf, 0, sizeof(buf));
    int cnt = 1;
    while((n = read(fd, buf, 2048)) > 0) {
        n = sendto(sock, buf, n, 0, (struct sockaddr *)&addr, sizeof(addr));
        if(n < 1) DieWithSystemMessage("sendto()");
        printf("[%d] send %d Byte\n", cnt, n);
        cnt++;
        usleep(30000);  // 送信レートを下げる．
    }
    if(n == -1) DieWithSystemMessage("read()");

    // ファイル転送の終了を示すパケットを送信する．
    ret = sendto(sock, buf, 0, 0, (struct sockaddr *)&addr, sizeof(addr));
    if(ret == -1) DieWithSystemMessage("sendto()");
    printf("send zero packet\n");

    close(sock);

    return 0;
}
