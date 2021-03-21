#include "wrap.h"
#include <jsoncpp/json/json.h>
#include <iostream>

struct client_info{
	struct sockaddr_in clientAddr;
	int connectFd;
};

static void* do_work(void* arg){
	struct client_info* ts=(struct client_info*)arg;
	char buf[MAXLEN];
	const char* serverRet="OK";

	int n=read(ts->connectFd,buf,MAXLEN);
	//printf("Recive from %s at port %d\n",inet_ntop(AF_INET,&((ts->clientAddr).sin_addr),str,sizeof(str)),ntohs((ts->clientAddr).sin_port));
	write(ts->connectFd,serverRet,2);
	write(STDOUT_FILENO,buf,n);
	
	

	close(ts->connectFd);
	pthread_exit(0);
}

int main(int argc,char* argv[]){
	struct sockaddr_in serverAddr,clientAddr;
	int listenFd,connectFd;

	int serverIP=0;
	bzero(&serverAddr,sizeof(serverAddr));

	serverAddr.sin_family=AF_INET;
	inet_pton(AF_INET,SERVER_IP,(void*)&serverIP);
	serverAddr.sin_addr.s_addr=serverIP;
	serverAddr.sin_port=htons(SERVER_PORT);

	listenFd=Socket(AF_INET,SOCK_STREAM,0);

	/*Set the address can be reused*/
	int opt=1;
	setsockopt(listenFd,SOL_SOCKET,SO_REUSEADDR,(void*)&opt,sizeof(opt));

	Bind(listenFd,(const struct sockaddr*)&serverAddr,sizeof(serverAddr));

	Listen(listenFd,128);

	printf("Accepting client connect...\n");

	int i=0;
	socklen_t clientAddrLen;
	pthread_t tid;
	struct client_info ts[512];
	while(1){
		clientAddrLen=sizeof(clientAddr);
		connectFd=Accept(listenFd,(struct sockaddr*)&clientAddr,&clientAddrLen);
		ts[i].clientAddr=clientAddr;
		ts[i].connectFd=connectFd;

		pthread_create(&tid,NULL,do_work,(void*)&ts[i]);
		pthread_detach(tid);

		i++;
	}

	return 0;
}
