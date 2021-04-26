#include <json/json.h>

#include <fstream>
#include <iostream>
#include <string>

#include "wrap.h"

#define SERVER_PORT 9526
#define SERVER_IP "127.0.0.1"
#define MAXLEN 128

//{"id":0,"recyleBitMap":13,"location":[120.332, 30.3242],"usage":[0.9, 1, 1,0.5]}

struct client_info {
    struct sockaddr_in clientAddr;
    int connectFd;
};

static void* do_work(void* arg);

pthread_mutex_t mutex;

int main(int argc, char* argv[]) {
    struct sockaddr_in serverAddr, clientAddr;
    int listenFd, connectFd;

    int serverIP = 0;
    bzero(&serverAddr, sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, (void*)&serverIP);
    serverAddr.sin_addr.s_addr = serverIP;
    serverAddr.sin_port = htons(SERVER_PORT);

    listenFd = Socket(AF_INET, SOCK_STREAM, 0);

    /*设置地址复用*/
    int opt = 1;
    setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, (void*)&opt, sizeof(opt));

    Bind(listenFd, (const struct sockaddr*)&serverAddr, sizeof(serverAddr));

    Listen(listenFd, 128);

    printf("Accepting client connect...\n");

    int i = 0;
    socklen_t clientAddrLen;
    pthread_t tid[512];
    struct client_info ts[512];
    pthread_mutex_init(&mutex, nullptr);

    while (1) {
        clientAddrLen = sizeof(clientAddr);
        connectFd =
            Accept(listenFd, (struct sockaddr*)&clientAddr, &clientAddrLen);
        ts[i].clientAddr = clientAddr;
        ts[i].connectFd = connectFd;

        pthread_create(tid + i, NULL, do_work, (void*)&ts[i]);
        pthread_detach(tid[i]);
        i++;
    }

    pthread_mutex_destroy(&mutex);
    return 0;
}

static void* do_work(void* arg) {
    printf("%s-new working thread created\n", __func__);
    /*把参数中的指针接收下来，泛型指针强转成结构体指针*/
    struct client_info* ts = (struct client_info*)arg;

    /*为客户端发来字符串提供的缓冲区，并清零*/
    char item[MAXLEN];
    memset(item, 0, MAXLEN);

    // int n=read(ts->connectFd,item,MAXLEN);
    read(ts->connectFd, item, MAXLEN);
    // printf("Recive from %s at port
    // %d\n",inet_ntop(AF_INET,&((ts->clientAddr).sin_addr),str,sizeof(str)),ntohs((ts->clientAddr).sin_port));
    printf("%s-item:%s", __func__, item);

    /*进入文件访问临界区，加锁*/
    pthread_mutex_lock(&mutex);
    std::ifstream trashInfoIfstream("../allTrashesInfo.json", std::ios::binary);
    if (!trashInfoIfstream.is_open()) {
        write(STDERR_FILENO, "ifstream file error", 20);
        /*其实这里写返回值也没什么意义，调用者也不会去接收，下同*/
        return (void*)-1;
    }

    Json::Reader* reader = new Json::Reader;

    Json::Value* rootOfFile = new Json::Value;
    reader->parse(trashInfoIfstream, *rootOfFile);
    trashInfoIfstream.close();

    Json::Value* rootOfItem = new Json::Value;
    reader->parse(item, *rootOfItem);

#ifdef TEST
    // printf("%s-rootOfItem-%s",__func__,(*rootOfItem));
    std::cout << __func__ << "-(*rootOfItem)-" << (*rootOfItem) << std::endl;
#endif

    /*更新*/
    int itemId = (*rootOfItem)["id"].asInt();
    (*rootOfFile)[itemId] = (*rootOfItem);

    std::ofstream trashInfoOfstream("../allTrashesInfo.json", std::ios::out);
    if (!trashInfoOfstream.is_open()) {
        write(STDERR_FILENO, "ofstream file error", 20);
        return (void*)-1;
    }

    Json::StyledWriter* jsw = new Json::StyledWriter;
    trashInfoOfstream << jsw->write(*rootOfFile);
    trashInfoOfstream.close();
    /*退出文件访问临界区，解锁*/
    /*睡1s，假设线程执行了很长时间，测试结果加锁确实有效了，多线程挺有趣的*/
    // sleep(1);
    pthread_mutex_unlock(&mutex);

    printf("%s-id=%d trash has been modified\n", __func__, itemId);

    delete (jsw);
    delete (rootOfItem);
    delete (rootOfFile);
    delete (reader);

    /*发回给客户端的信息*/
    const char* serverRet1 = "OK\n";
    write(ts->connectFd, serverRet1, 4);

    close(ts->connectFd);
    printf("working thread exited\n\n");
    pthread_exit(0);
}