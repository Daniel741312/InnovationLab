#ifndef __WRAP_H__
#define __WRAP_H__

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <signal.h>
#include <pthread.h>

#define SERVER_PORT 9526 
#define SERVER_IP   "127.0.0.1"
#define MAXLEN		128

void perr_exit(const char* str);
int Accept(int sockfd, struct sockaddr* addr, socklen_t* addrlen);
int Bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen);
int Connect(int sockfd, const struct sockaddr* addr, socklen_t addrlen);
int Listen(int sockfd, int backlog);
int Socket(int domain, int type, int protocol);

#endif
