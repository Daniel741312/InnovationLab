#include "wrap.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <jsoncpp/json/json.h>

struct client_info{
	struct sockaddr_in clientAddr;
	int connectFd;
};

static void* do_work(void* arg){

	std::cout<<"new working thread created"<<std::endl;
	/*把参数中的指针接收下来，泛型指针强转成结构体指针*/
	struct client_info* ts=(struct client_info*)arg;

	/*为客户端发来字符串提供的缓冲区，并清零*/
	char item[MAXLEN];
	memset(item,0,MAXLEN);

	/*发回给客户端的信息*/
	const char* serverRet="OK";

	read(ts->connectFd,item,MAXLEN);
	//printf("Recive from %s at port %d\n",inet_ntop(AF_INET,&((ts->clientAddr).sin_addr),str,sizeof(str)),ntohs((ts->clientAddr).sin_port));
	write(ts->connectFd,serverRet,2);
	//write(STDOUT_FILENO,item,n);

	std::ifstream trashInfoR("../allTrashesInfo.json",std::ios::binary);
    if (!trashInfoR.is_open()){
        std::cerr<< "ifstream file error" << std::endl;
		/*其实这里写返回值也没什么意义，调用者也不会去接收，下同*/
        return (void*)-1;
    }

    Json::Reader* reader=new Json::Reader;

    Json::Value* rootOfFile=new Json::Value;
    reader->parse(trashInfoR,*rootOfFile);
    trashInfoR.close();

    Json::Value* rootOfItem=new Json::Value;
    reader->parse(item,*rootOfItem);

#ifdef TEST
    cout<<"rootOfFile:"<<endl;
    cout<<(*rootOfFile)<<endl;

    cout<<"item:"<<endl;
    cout<<(*rootOfItem)<<endl;
#endif

    int itemId=(*rootOfItem)["id"].asInt();
    (*rootOfFile)[itemId]=(*rootOfItem);
    
#ifdef TEST
    cout<<"after change,rootOfFile:"<<endl;
    cout<<(*rootOfFile)<<endl;
#endif

    std::ofstream trashInfoW("../allTrashesInfo.json",std::ios::out);
    if(!trashInfoW.is_open()){
        std::cerr<<"oftream file error"<<std::endl;
        return (void*)-1;
    }

    Json::StyledWriter* jsw=new Json::StyledWriter;
    trashInfoW<<(*jsw).write(*rootOfFile);
    trashInfoW.close();
	std::cout<<"id="<<itemId<<" trash modified"<<std::endl;

    delete(jsw);
    delete(rootOfItem);
    delete(rootOfFile);
    delete(reader);

	close(ts->connectFd);

	std::cout<<"working thread exited\n\n"<<std::endl;
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

	printf("Accepting client connect...\n\n");

	int i=0;
	socklen_t clientAddrLen;
	pthread_t tid;
	struct client_info ts[128];
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