#ifndef __TCP_SEND_H__
#define __TCP_SEND_H__

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <ctype.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9526

/*
    *传入一个json字符串和它的长度（是strlen不是sizeof！）
    *我负责跟Server建立TCP连接并发送过去
    *成功返回0失败返回-1
*/
int sendInfoByTCP(const char *str, size_t length);

#endif
