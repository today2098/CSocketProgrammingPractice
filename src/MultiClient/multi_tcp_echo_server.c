#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "../my_library/my_library.h"

struct ClientData {
    int sock;
    struct sockaddr_in saddr;
};

// エコーサーバーとしてクライアントからデータを取得し返送する．
void *threadfunc(void *data) {
    if(data == NULL) return (void *)-1;

    struct ClientData *cdata = data;
    int sock = cdata->sock;
    char buf0[MY_ADDRSTRLEN], buf[1024];
    int n;
    int ret;

    // クライアントからデータを取得する．
    memset(buf, 0, sizeof(buf));
    n = read(sock, buf, sizeof(buf));
    if(n < 0) {
        PrintSystemErrorMessage("read()");
        goto Err;
    }

    // debug.
    GetAddressFromSockaddr_in(&cdata->saddr, buf0, sizeof(buf0));
    printf("%s : %s\n", buf0, buf);
    fflush(stdout);

    // データを返送する．
    n = write(sock, buf, n);
    if(n < 0) {
        PrintSystemErrorMessage("write()");
        goto Err;
    }

    // TCPセッションを終了する．
    ret = close(sock);
    if(ret != 0) DieWithSystemMessage("close()");

    // 親スレッドでmallocされた領域を開放する．
    free(data);

    return (void *)0;

Err:
    free(data);

    return (void *)-1;
}

int main() {
    int sock0;
    struct sockaddr_in addr;
    pthread_t th;
    struct ClientData *cdata;
    char buf0[MY_ADDRSTRLEN];
    int ret;

    // Listen用のソケットを作成する．
    sock0 = socket(AF_INET, SOCK_STREAM, 0);
    if(sock0 == -1) DieWithSystemMessage("socket()");

    // Listen用ソケットの設定する．
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    addr.sin_addr.s_addr = INADDR_ANY;
    ret = bind(sock0, (struct sockaddr *)&addr, sizeof(addr));
    if(ret == -1) DieWithSystemMessage("bind()");

    // TCPクライアントからの接続要求を待ち受けている状態にする．
    ret = listen(sock0, 5);
    if(ret == -1) DieWithSystemMessage("listen()");

    // debug.
    printf("listen\n");

    // 新規TCPコネクションに関する情報をclientdata構造体に格納する．
    for(;;) {
        cdata = malloc(sizeof(struct ClientData));
        if(cdata == NULL) DieWithSystemMessage("malloc()");

        // TCPクライアントからの接続要求を受け付ける．
        socklen_t len = sizeof(cdata->saddr);
        cdata->sock = accept(sock0, (struct sockaddr *)&cdata->saddr, &len);
        if(cdata->sock == -1) DieWithSystemMessage("accept()");

        // debug.
        GetAddressFromSockaddr_in(&cdata->saddr, buf0, sizeof(buf0));
        printf("accept from %s\n", buf0);
        fflush(stdout);

        // 新スレッドでthreadfunc()の処理を実行する．
        ret = pthread_create(&th, NULL, threadfunc, cdata);
        if(ret != 0) DieWithSystemMessage("pthread_create()");

        // 親スレッド側で子スレッドを切り離す．
        ret = pthread_detach(th);
        if(ret != 0) DieWithSystemMessage("pthread_detach()");
    }

    // Listen用ソケットを終了する．
    ret = close(sock0);
    if(ret != 0) DieWithSystemMessage("close()");

    return 0;
}
