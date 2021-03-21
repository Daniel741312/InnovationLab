#include "wrap.h"

void perr_exit(const char* str){
	perror(str);
	exit(1);
}

int Accept(int sockfd, struct sockaddr* addr, socklen_t* addrlen){

	int n;

again:
	if((n=accept(sockfd,addr,addrlen))<0){
		/*If the error is caused by a  singal,not due to the accept itself,try again*/
		if((errno==EINTR)||(errno==ECONNABORTED))
			goto again;
		else
			perr_exit("accept error");
	}
	return n;
}

int Bind(int sockfd,const struct sockaddr* addr,socklen_t addrlen){
	int n;

	if((n=bind(sockfd,addr,addrlen))<0)
		perr_exit("bind error");

	return n;
}

int Connect(int sockfd,const struct sockaddr* addr,socklen_t addrlen){
	int n=connect(sockfd,addr,addrlen);
	if(n<0)
		perr_exit("connect error");
	return n;
}

int Listen(int sockfd, int backlog){
	int n;
	if((n=listen(sockfd,backlog))<0)
		perr_exit("listen error");
	return n;
}

int Socket(int domain,int type,int protocol){
	int n;
	if((n=socket(domain,type,protocol))<0)
		perr_exit("listen error");
	return n;
}
