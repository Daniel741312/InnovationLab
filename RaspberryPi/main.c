#include "include/tcp_send.h"

int sendInfoByTCP(const char* stc,size_t length);

int main(int argc,char* argv[]){
    const char* str="hello,world\n";

    int ret=sendInfoByTCP(str,sizeof(str));
    
    printf("%d\n",ret);

    return 0;
}