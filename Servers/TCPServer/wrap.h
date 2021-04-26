#ifndef __WRAP_H__
#define __WRAP_H__

#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void perr_exit(const char* str);
int Accept(int sockfd, struct sockaddr* addr, socklen_t* addrlen);
int Bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen);
int Connect(int sockfd, const struct sockaddr* addr, socklen_t addrlen);
int Listen(int sockfd, int backlog);
int Socket(int domain, int type, int protocol);

#endif
