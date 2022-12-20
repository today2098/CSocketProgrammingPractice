#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "../my_library/my_library.h"

int main(int argc, char *argv[]) {
    if(argc != 2) {
        fprintf(stderr,
                "Simple UDP Sender\n\n"
                "Usage:\n\t%s <dstipaddr>\n\n"
                "Example:\n\t%s 127.0.0.1\n",
                argv[0], argv[0]);
        return 1;
    }

    int sock;
    struct sockaddr_in addr;
    int n;

    // IPv4のUDPソケットを作成する．
    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock == -1) DieWithSystemMessage("socket()");

    // 宛先の情報を指定する．
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    inet_pton(AF_INET, argv[1], &addr.sin_addr.s_addr);

    // メッセージを送信する．
    n = sendto(sock, "HELLO", 5, 0, (struct sockaddr *)&addr, sizeof(addr));
    if(n < 1) DieWithSystemMessage("sendto()");

    // debug.
    char buf[INET_ADDRSTRLEN];
    memset(buf, 0, sizeof(buf));
    inet_ntop(AF_INET, &addr.sin_addr, buf, sizeof(buf));
    printf("send message to %s:%d\n", buf, ntohs(addr.sin_port));

    close(sock);

    return 0;
}
