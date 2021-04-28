#include "tcp_send.h"
#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

/*
    #define SERVER_IP "127.0.0.1"
    #define SERVER_PORT 9526
*/

namespace tcp_send{

TCPClient::TCPClient(const std::string& serverIp,int serverPort){
    this->serverIp_=serverIp;
    this->serverPort_=serverPort;

    memset(serverRet_,0,bufferSize_);

    /*服务器端地址结构*/
    int sIP= 0;
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(static_cast<uint16_t>(serverPort_));

    inet_pton(AF_INET, serverIp_.c_str(), (void*)&sIP);
    serverAddr.sin_addr.s_addr = sIP;

    clientFd_ = 0;
    clientFd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (clientFd_ == -1) {
        perror("socket error\nexit now\n");
        exit(-1);
    }

    int ret = 0;
    ret = connect(clientFd_, (const struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (ret == -1) {
        perror("connect error\nexit now\n");
        exit(-1);
    }
    printf("Connect to %s at port %d successfully\n",serverIp_.c_str(),serverPort_);
}

TCPClient::~TCPClient(){
    close(clientFd_);
    printf("Connection closed\n");
}

int TCPClient::SendStrViaTCP(const std::string& strSent, std::string* strReturn){
    memset(serverRet_,0,bufferSize_);
    if(write(clientFd_,strSent.c_str(),strSent.length())==-1){
        write(STDERR_FILENO,"Write error\n",12);
        return -1;
    }
    /*阻塞，只读一下，读到EOF就返回*/
    if(read(clientFd_,serverRet_,bufferSize_)==-1){
        write(STDERR_FILENO,"Read error\n",12);
        return -1;
    }
    (*strReturn)=std::string(serverRet_);
    return 0;
}

int TCPClient::getBufferSize() const{
    return this->bufferSize_;
}

void TCPClient::setBufferSize(int bfsz){
    this->bufferSize_=bfsz;
    return;
}

}
