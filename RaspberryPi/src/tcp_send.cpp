#include "tcp_send.h"

int sendInfoByTCP(const char* str,size_t length){
	char serverRet[16];
	memset(serverRet,0,16);

	/*服务器端地址结构*/
	int serverIP=0;
	struct sockaddr_in serverAddr;
	serverAddr.sin_family=AF_INET;
	serverAddr.sin_port=htons(SERVER_PORT);
	inet_pton(AF_INET,SERVER_IP,(void*)&serverIP);
	serverAddr.sin_addr.s_addr=serverIP;
	

	int clientFd=0;
	clientFd=socket(AF_INET,SOCK_STREAM,0);
	if(clientFd==-1){
		perror("socket error\n");
		return -1;
	}

	int ret=0;
	ret=connect(clientFd,(const struct sockaddr*)&serverAddr,sizeof(serverAddr));
	if(ret==-1){
		perror("connect error");
		return -1;
	}

	/*业务逻辑*/
	write(clientFd,str,length);
	read(clientFd,serverRet,sizeof(serverRet));
	if(strcmp(serverRet,"OK")!=0){
		write(STDERR_FILENO,serverRet,strlen(serverRet));
		return -1;
	}

	close(clientFd);
	return 0;
}
