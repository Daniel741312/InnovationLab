#ifndef __TCP_SEND_H__
#define __TCP_SEND_H__

#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<errno.h>
#include<string.h>
#include<arpa/inet.h>
#include<ctype.h>

#define SERVER_IP   "127.0.0.1"
#define SERVER_PORT 9526 

/**/
int sendInfoByTCP(const char* str,size_t length);

#endif