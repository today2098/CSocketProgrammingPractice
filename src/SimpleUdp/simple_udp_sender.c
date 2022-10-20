#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "../my_library/my_library.h"  // for DieWithSystemMessage().

int main(int argc, char *argv[]) {
    if(argc != 2) {
        fprintf(stderr,
                "Simple UDP Sender\n\n"
                "Usage:\n\t%s [dstipaddr]\n\n"
                "Example:\n\t%s 127.0.0.1\n",
                argv[0], argv[0]);
        return 1;
    }

    // IPv4のUDPソケットを作成する．
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock == -1) DieWithSystemMessage("socket()");

    // 宛先の情報を指定する．
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    inet_pton(AF_INET, argv[1], &addr.sin_addr.s_addr);

    // データを送信する．
    int n = sendto(sock, "HELLO", 5, 0, (struct sockaddr *)&addr, sizeof(addr));
    if(n < 1) DieWithSystemMessage("sendto()");

    close(sock);

    return 0;
}
